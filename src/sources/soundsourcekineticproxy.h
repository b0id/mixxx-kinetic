#pragma once

#include <QPair>
#include <QString>
#include <QVector>
#include <atomic>
#include <memory>

#include "sources/soundsource.h" // existing mixxx base
#include "util/defs.h"           // For SINT, etc.

// Forward declaration
template<typename T, size_t Capacity>
class StreamRingBuffer;

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
    QString m_virtualPath;
    mixxx::AudioSourcePointer m_pDelegate;
};
