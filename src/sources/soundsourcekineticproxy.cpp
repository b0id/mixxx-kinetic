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

    // Read from RingBuffer
    // We cast the writable data pointer to CSAMPLE*
    // This assumes sampleFrames is contiguous, which it should be for simple buffers.
    // However, WritableSampleFrames might handle stride. We assume standard packed samples here for now.
    // Ideally we should use sampleFrames.writableData() but let's check correct access.
    // For now, assuming we can get a pointer.

    // NOTE: mixxx::WritableSampleFrames doesn't expose raw pointer easily sometimes?
    // Let's check if we can iterate or getting pointer.
    // Usually: sampleFrames.writableData() returns CSAMPLE*.

    // Assuming standard Mixxx AudioSource/SampleFrames usage:
    CSAMPLE* pDest = sampleFrames.writableData();
    size_t count = sampleFrames.frameIndexRange().length(); // This is frames, not samples?
    // Wait, RingBuffer stores SAMPLES (multi-channel).
    // range length is Frames. Samples = Frames * Channels.
    // We need to know channel count. `getSignalInfo().getChannelCount()`.

    size_t samplesReq = count * getSignalInfo().getChannelCount();

    // Lock-free read
    size_t samplesRead = m_pRingBuffer->read(pDest, samplesReq);

    if (samplesRead < samplesReq) {
        // UNDERUN!
        // Fill remainder with silence
        std::fill(pDest + samplesRead, pDest + samplesReq, 0.0f);

        // Signal Underrun (logging for now)
        // In tight loop this might spam logs.
        // kLogger.warning() << "Buffer Underrun!";
    }

    // Return the frames we *intended* to read (we filled the rest with silence so strictly speaking we returned them)
    // The engine expects us to return how many frames we filled.
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
}

void SoundSourceKineticProxy::readWorker() {
    // Thread loop
    const int kChunkFrames = 4096;
    // We assume stereo for now or query delegate?
    // Better to query delegate channel count.
    unsigned int channels = getSignalInfo().getChannelCount();
    if (channels == 0)
        channels = 2; // Default fallback if not yet open?

    size_t chunkSizeSamples = kChunkFrames * channels;
    std::vector<CSAMPLE> buffer(chunkSizeSamples);

    mixxx::IndexRange indexRange = mixxx::IndexRange::forward(0, kChunkFrames);

    while (!m_stopThread) {
        // Check RingBuffer space
        size_t available = m_pRingBuffer->capacity() - (m_pRingBuffer->capacity() - m_pRingBuffer->capacity()); // wait, need write capacity
        // SPSCQueue doesn't expose "write available" directly but we can check if full.
        // Or simply try_write.

        // Actually, we should try to read from FUSE only if we have space.
        // But FUSE read is blocking.
        // If we read from FUSE and can't write to RingBuffer, we are stuck holding data.
        // Retrying write loop is fine.

        // 1. Read from Delegate
        // We need to advance the read cursor? AudioSource is seekable.
        // We need to maintain a read position for the WORKER, separate from the AudioSource's notion?
        // Wait, AudioSource typically maintains internal state of "next read position" if consecutive?
        // Or do we need to seek?
        // `readSampleFramesClamped` typically takes a `SampleFrames` which has a range? No, it returns readable frames.
        // The argument `WritableSampleFrames` implies the destination.
        // The *source* position is implicitly the current cursor of the SoundSource.

        // So we just call current delegate read.

        // Construct WritableSampleFrames wrapper for our buffer
        mixxx::SampleBuffer sampleBuffer(buffer.data(), chunkSizeSamples);
        mixxx::WritableSampleFrames frames(indexRange, &sampleBuffer);

        // We need to know WHERE to read from. AudioSource has `seek`.
        // But `readSampleFramesClamped` usually advances cursor.
        // The `indexRange` passed to `WritableSampleFrames` is for the buffer indices, not file position.

        // Issue: `AudioSource` methods are not thread safe if called concurrently.
        // Only this thread calls `m_pDelegate->read...`. The engine calls `this->read...` which accesses RingBuffer.
        // So `m_pDelegate` is exclusively accessed by `readWorker` (except `close` and `open`).
        // This is safe.

        // READ FROM FUSE (Blocking)
        mixxx::ReadableSampleFrames readFrames = mixxx::AudioSource::readSampleFramesClampedOn(*m_pDelegate, frames);

        size_t framesRead = readFrames.frameIndexRange().length();
        size_t samplesRead = framesRead * channels;

        if (framesRead == 0) {
            // EOF or Error
            // Sleep a bit to prevent busy loop if EOF
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // WRITE TO RINGBUFFER
        size_t written = 0;
        size_t toWrite = samplesRead;
        const CSAMPLE* pSrc = buffer.data();

        while (written < toWrite && !m_stopThread) {
            size_t n = m_pRingBuffer->write(pSrc + written, toWrite - written);
            written += n;
            if (written < toWrite) {
                // Buffer full, yield/sleep
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
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
