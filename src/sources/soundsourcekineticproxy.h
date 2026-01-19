#pragma once

#include "sources/soundsource.h" // existing mixxx base
#include "util/defs.h" // For SINT, etc.
#include <QPair>
#include <QString>
#include <QVector>
#include <atomic>
#include <memory>

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
    virtual ~SoundSourceKineticProxy();

    // SoundSource interface - MUST return immediately
    // Note: The signature in mixxx 2.4+ might differ.
    // Found in existing proxy: ReadableSampleFrames readSampleFramesClamped(...)
    // We'll use the spec's intended override.
    // unsigned read(unsigned long size, const SAMPLE * destination) = 0; // Spec version

    // Using the one found in existing code to be safe?
    // readableSampleFrames readSampleFramesClamped(const WritableSampleFrames& writableFrames) override;

    // Spec says:
    // virtual unsigned read(unsigned long size, const SAMPLE * destination) = 0;
    // We will put the Spec version commenting the potential mismatch.
    unsigned read(unsigned long size, const SAMPLE* destination) override;

    void seek(long filepos) override;
    long length() const override;

    // Streaming extensions
    double getCachedPercentage() const;
    QVector<QPair<qint64, qint64>> getCachedRanges() const;
    bool isFullyCached() const;

    // Mock signals (until we verify SoundSource inherits QObject)
    // signal void bufferUnderrun();
    // signal void cacheProgressChanged(double percentage);

  private:
    // std::unique_ptr<StreamRingBuffer> m_buffer;
    QString m_virtualPath;
    qint64 m_totalFrames;
    std::atomic<bool> m_underrunState;
};
