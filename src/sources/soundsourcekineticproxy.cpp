#include "soundsourcekineticproxy.h"

#include <QFileInfo>

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

    return tryOpenOn(*m_pDelegate, mode, params);
}

mixxx::ReadableSampleFrames SoundSourceKineticProxy::readSampleFramesClamped(
        const mixxx::WritableSampleFrames& sampleFrames) {
    if (!m_pDelegate) {
        return mixxx::ReadableSampleFrames(sampleFrames.frameIndexRange());
    }
    // Use helper to access protected member on delegate
    return mixxx::AudioSource::readSampleFramesClampedOn(*m_pDelegate, sampleFrames);
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
