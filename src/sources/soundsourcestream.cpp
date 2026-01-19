#include "soundsourcestream.h"

SoundSourceStream::SoundSourceStream(const QString& virtualPath)
        : SoundSourceKineticProxy(virtualPath) {
}

SoundSourceStream::~SoundSourceStream() {
}
