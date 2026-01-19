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

class SoundSourceKineticProxy : public SoundSource {
    // Q_OBJECT // SoundSource might not be a QObject in mixxx?
    // Spec showed QObject macros in StreamingService but not explicit in SoundSourceProxy snippet
    // except for signals. SoundSource usually isn't a QObject.
    // If signals are required, we might need to inherit QObject too or use a helper.
    // Spec says: "signals: signal void bufferUnderrun();" - pseudosyntax.
    // We will assume it needs QObject or similar mechanism.
    // For now, adhering to C++ structure.
  public:
    explicit SoundSourceKineticProxy(const QString& virtualPath);
    ~SoundSourceKineticProxy() override;

    // AudioSource interface
    OpenResult tryOpen(OpenMode mode, const OpenParams& params) override;

    // IAudioSourceReader interface
    ReadableSampleFrames readSampleFramesClamped(const WritableSampleFrames& sampleFrames) override;

    void close() override;

    // SoundSource overrides (if any, typically just constructor helper stuff)
    // SoundSource::getType() is non-virtual.

    // Streaming extensions
    double getCachedPercentage() const;
    QVector<QPair<qint64, qint64>> getCachedRanges() const;
    bool isFullyCached() const;

  private:
    QString m_virtualPath;
    AudioSourcePointer m_pDelegate;
};
