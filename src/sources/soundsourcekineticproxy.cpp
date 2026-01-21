#include "soundsourcekineticproxy.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

#include "sources/soundsourceproxy.h"
#include "streaming/bridge/bridgeclient.h"
#include "util/logger.h"

namespace {
mixxx::Logger kLogger("SoundSourceKineticProxy");
}

SoundSourceKineticProxy::SoundSourceKineticProxy(const QString& virtualPath)
        : mixxx::SoundSource(QUrl::fromLocalFile(virtualPath)),
          m_virtualPath(virtualPath) {
}

SoundSourceKineticProxy::~SoundSourceKineticProxy() {
    close();
}

mixxx::AudioSource::OpenResult SoundSourceKineticProxy::tryOpen(
        mixxx::AudioSource::OpenMode mode, const mixxx::AudioSource::OpenParams& params) {
    if (m_pDelegate) {
        return mixxx::AudioSource::OpenResult::Succeeded;
    }

    // Register with Bridge
    BridgeClient client;
    if (!client.connectToServer()) {
        kLogger.warning() << "Failed to connect to Bridge Server";
        return mixxx::AudioSource::OpenResult::Failed;
    }

    // REGISTER_TRACK
    uint64_t inode = client.registerTrack(m_virtualPath, 1024 * 1024 * 10); // Dummy size
    if (inode == 0) {
        kLogger.warning() << "Failed to register track with Bridge";
        return mixxx::AudioSource::OpenResult::Failed;
    }

    // Construct FUSE path
    QFileInfo fileInfo(m_virtualPath);
    QString extension = fileInfo.suffix();

    // Append extension to FUSE path so the SoundSourceProvider can recognize it.
    QString fusePath = QString("/tmp/mountpoint/%1.%2").arg(inode).arg(extension);

    // Find Provider
    // SoundSourceProxy is in Global namespace
    auto pProvider = SoundSourceProxy::getPrimaryProviderForFileType(extension);
    if (!pProvider) {
        kLogger.warning() << "No provider found for extension" << extension;
        return mixxx::AudioSource::OpenResult::Failed;
    }

    // Create delegate
    m_pDelegate = pProvider->newSoundSource(QUrl::fromLocalFile(fusePath));
    if (!m_pDelegate) {
        return mixxx::AudioSource::OpenResult::Failed;
    }

    auto result = tryOpenOn(*m_pDelegate, mode, params);
    if (result == mixxx::AudioSource::OpenResult::Succeeded) {
        // Initialize RingBuffer (capacity ~1M samples, approx 11s stereo)
        // Adjust capacity based on need. 2^20 is a good power of two.
        m_pRingBuffer = std::make_unique<mixxx::StreamRingBuffer<CSAMPLE>>(1024 * 1024);

        // Start worker thread
        m_stopThread = false;
        m_readThread = std::thread(&SoundSourceKineticProxy::readWorker, this);
    }
    return result;
}

mixxx::ReadableSampleFrames SoundSourceKineticProxy::readSampleFramesClamped(
        const mixxx::WritableSampleFrames& sampleFrames) {
    if (!m_pRingBuffer) {
        return mixxx::ReadableSampleFrames(sampleFrames.frameIndexRange());
    }

    // 1. Detect Seek
    // The engine tells us where to read via sampleFrames.frameIndexRange().start()
    // We expect contiguous reading.
    auto startIndex = sampleFrames.frameIndexRange().start();
    if (startIndex != m_nextFrameIndex) {
        // Discontinuity Detected!
        // kLogger.info() << "Seek detected! From" << m_nextFrameIndex << "to" << startIndex;

        m_seekPos = startIndex;        // Signal worker
        m_pRingBuffer->reset();        // Flush old data
        m_nextFrameIndex = startIndex; // Update expectation

        // Return silence for this callback (Underrun intended during seek)
        // We fill dest with 0 just in case.
        if (sampleFrames.writableData()) {
            // WritableSampleFrames might not contain 0s.
            // But we are returning an empty result implicitly if we don't return ReadableSampleFrames with valid slice?
            // Actually readSampleFramesClamped typically returns what was read.
            // If we return empty, engine assumes EOF?
            // No, usually silenced.
            // Let's fill and return empty to simulate "Not Ready".
            // Or fill and return full length silence.

            // Safer: Zero fill and return full length.
            // But we rely on Worker to catch up.
            CSAMPLE* pDest = sampleFrames.writableData();
            size_t framesReq = sampleFrames.frameIndexRange().length();
            unsigned int channels = getSignalInfo().getChannelCount();
            size_t samplesReq = framesReq * channels;
            std::fill(pDest, pDest + samplesReq, 0.0f);
        }
        // Actually, if we return "index range" but valid slice is empty/null, engine plays silence.
        return mixxx::ReadableSampleFrames(sampleFrames.frameIndexRange());
    }

    // 2. Read from RingBuffer
    CSAMPLE* pDest = sampleFrames.writableData();
    // Assuming packed stereo/mono based on channel count
    unsigned int channels = getSignalInfo().getChannelCount();
    size_t framesReq = sampleFrames.frameIndexRange().length();
    size_t samplesReq = framesReq * channels;

    // Lock-free read
    size_t samplesRead = m_pRingBuffer->read(pDest, samplesReq);

    // Update next frame expectation
    // We read X samples => X/channels frames.
    // If partial frame read? (samplesRead % channels != 0)
    // Should align to frames. RingBuffer might return partial.
    // For simplicity, we assume we want full frames.
    size_t framesRead = samplesRead / channels;

    m_nextFrameIndex += framesRead;

    if (samplesRead < samplesReq) {
        // Underrun
        // kLogger.warning() << "Underrun";
        if (pDest) { // Ensure pDest is valid
            std::fill(pDest + samplesRead, pDest + samplesReq, 0.0f);
        }
        // Force update of expectation so we don't trigger "Seek" detection on next callback
        // because we technically skipped the missing frames (in silence).
        m_nextFrameIndex += (framesReq - framesRead);
    }

    return mixxx::ReadableSampleFrames(sampleFrames.frameIndexRange());
}

void SoundSourceKineticProxy::close() {
    m_stopThread = true;
    if (m_readThread.joinable()) {
        m_readThread.join();
    }

    if (m_pDelegate) {
        m_pDelegate->close();
        m_pDelegate.reset();
    }
    m_pRingBuffer.reset();
    m_nextFrameIndex = 0; // Reset for next open
    m_seekPos = -1;       // Reset for next open
}

void SoundSourceKineticProxy::readWorker() {
    const int kChunkFrames = 4096;
    unsigned int channels = getSignalInfo().getChannelCount();
    if (channels == 0) {
        channels = 2;
    }

    const size_t chunkSizeSamples = kChunkFrames * channels;

    // Worker's view of current frame position
    SINT workerFrameIndex = 0;

    while (!m_stopThread) {
        // 1. Handle Seek
        int64_t seekTarget = m_seekPos.exchange(-1);
        if (seekTarget != -1 && m_pDelegate) {
            // For now, we don't have a public seek method - the proxy handles
            // discontinuities by just reading from new position
            workerFrameIndex = seekTarget;
            m_pRingBuffer->reset(); // Flush old data
        }

        // 2. Read Chunk from FUSE delegate (blocking)
        if (m_pDelegate) {
            // Allocate sample buffer for this chunk
            mixxx::SampleBuffer sampleBuffer(chunkSizeSamples);

            // Create index range for the frames we want to read
            mixxx::IndexRange indexRange = mixxx::IndexRange::forward(workerFrameIndex, kChunkFrames);

            // Wrap buffer in a WritableSlice
            mixxx::SampleBuffer::WritableSlice writableSlice(sampleBuffer);

            // Create WritableSampleFrames
            mixxx::WritableSampleFrames writableFrames(indexRange, writableSlice);

            // READ FROM FUSE (Blocking)
            mixxx::ReadableSampleFrames readFrames =
                    mixxx::AudioSource::readSampleFramesClampedOn(*m_pDelegate, writableFrames);

            const SINT framesRead = readFrames.frameIndexRange().length();

            if (framesRead == 0) {
                // EOF or Error - sleep to prevent busy loop
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            // Update worker's position
            workerFrameIndex += framesRead;

            // Calculate samples read (frames * channels)
            const size_t samplesRead = framesRead * channels;

            // WRITE TO RINGBUFFER
            const CSAMPLE* pSrc = readFrames.readableData();
            size_t written = 0;

            while (written < samplesRead && !m_stopThread) {
                size_t n = m_pRingBuffer->write(pSrc + written, samplesRead - written);
                written += n;
                if (written < samplesRead) {
                    // Buffer full, yield/sleep
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            }
        }
    }
}

double SoundSourceKineticProxy::getCachedPercentage() const {
    return 0.0;
}

QVector<QPair<qint64, qint64>> SoundSourceKineticProxy::getCachedRanges() const {
    return {};
}

bool SoundSourceKineticProxy::isFullyCached() const {
    return false;
}
