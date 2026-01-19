#include "soundsourcekineticproxy.h"

SoundSourceKineticProxy::SoundSourceKineticProxy(const QString& virtualPath)
        : m_virtualPath(virtualPath) {
}

SoundSourceKineticProxy::~SoundSourceKineticProxy() {
}

unsigned SoundSourceKineticProxy::read(unsigned long size, const SAMPLE* destination) {
    // Stub
    return 0;
}

void SoundSourceKineticProxy::seek(long filepos) {
    // Stub
}

long SoundSourceKineticProxy::length() const {
    return 0;
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
