#pragma once

#include <QPair>
#include <QString>
#include <QVector>
#include <atomic>
#include <memory>
#include <thread>

#include "sources/soundsource.h" // existing mixxx base
#include "sources/streamringbuffer.h"
#include "util/defs.h" // For SINT, etc.

// Forward declaration
// Forward declaration
// template<typename T> class StreamRingBuffer; // Already included

class SoundSourceKineticProxy : public mixxx::SoundSource {
  public:
    explicit SoundSourceKineticProxy(const QString& virtualPath);
    ~SoundSourceKineticProxy() override;

    // AudioSource interface
    mixxx::AudioSource::OpenResult tryOpen(mixxx::AudioSource::OpenMode mode, const mixxx::AudioSource::OpenParams& params) override;

    // IAudioSourceReader interface
    mixxx::ReadableSampleFrames readSampleFramesClamped(const mixxx::WritableSampleFrames& sampleFrames) override;

    void close() override;

    // SoundSource overrides
    // SoundSource::getType() is non-virtual.

    // Streaming extensions
    double getCachedPercentage() const;
    QVector<QPair<qint64, qint64>> getCachedRanges() const;
    bool isFullyCached() const;

  private:
    void readWorker();

    QString m_virtualPath;
    mixxx::AudioSourcePointer m_pDelegate;

    // RingBuffer for lock-free audio transfer
    std::unique_ptr<mixxx::StreamRingBuffer<CSAMPLE>> m_pRingBuffer;

    // Worker thread for reading from FUSE
    std::thread m_readThread;
    std::atomic<bool> m_stopThread{false};
};
