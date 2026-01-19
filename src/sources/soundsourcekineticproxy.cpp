#include "soundsourcekineticproxy.h"

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
    // We need the ACTUAL backing file path (the cache path).
    // This Proxy class needs to be instantiated with it.
    // Assuming m_virtualPath is the backing path for now.
    // Note: registerTrack returns uint64_t (abstracted inode)
    uint64_t inode = client.registerTrack(m_virtualPath, 1024 * 1024 * 10); // Dummy size
    if (inode == 0) {
        kLogger.warning() << "Failed to register track with Bridge";
        return mixxx::AudioSource::OpenResult::Failed;
    }

    // Construct FUSE path
    QString fusePath = QString("/tmp/mountpoint/%1").arg(inode);

    // Create delegate
    m_pDelegate = mixxx::newSoundSourceFromUrl<mixxx::SoundSource>(QUrl::fromLocalFile(fusePath));
    if (!m_pDelegate) {
        return mixxx::AudioSource::OpenResult::Failed;
    }

    return tryOpenOn(*m_pDelegate, mode, params);
}

mixxx::ReadableSampleFrames SoundSourceKineticProxy::readSampleFramesClamped(
        const mixxx::WritableSampleFrames& sampleFrames) {
    if (!m_pDelegate) {
        return mixxx::ReadableSampleFrames(sampleFrames.frameIndexRange());
    }
    return m_pDelegate->readSampleFramesClamped(sampleFrames);
}

void SoundSourceKineticProxy::close() {
    if (m_pDelegate) {
        m_pDelegate->close();
        m_pDelegate.reset();
    }
    // Unregister? We typically unregister when the track is unloaded.
    // For now, simple client.
    BridgeClient client;
    if (client.connectToServer()) {
        // We'd need the inode again.
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
