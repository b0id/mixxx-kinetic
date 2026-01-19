#pragma once

#include "soundsourcekineticproxy.h"

class SoundSourceStream : public SoundSourceKineticProxy {
  public:
    explicit SoundSourceStream(const QString& virtualPath);
    ~SoundSourceStream() override;

    // Implement abstract methods if any remaining
};
