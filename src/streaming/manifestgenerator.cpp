#include "streaming/manifestgenerator.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QUuid>

#include "util/logger.h"

namespace mixxx {
namespace kinetic {

namespace {
const Logger kLogger("ManifestGenerator");
}

QString ManifestGenerator::generate(
        const TrackMetadata& track,
        const QString& streamUrl,
        const QString& extension,
        int bitrate) {
    if (streamUrl.isEmpty()) {
        kLogger.warning() << "Cannot generate manifest for empty stream URL";
        return QString();
    }

    // 1. Prepare JSON object
    QJsonObject root;
    root["url"] = streamUrl;
    root["id"] = track.remoteId;
    root["extension"] = extension;
    root["bitrate"] = bitrate;
    root["title"] = track.title;
    root["artist"] = track.artist;

    // Add some metadata for debugging/UI
    root["generated_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // 2. Determine Output Path
    // Use a dedicated subdirectory in temp location
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir dir(tempPath);
    if (!dir.exists("mixxx/manifests")) {
        dir.mkpath("mixxx/manifests");
    }

    // Create a unique filename based on Track ID or UUID
    // Using UUID ensures no collisions if same track loaded twice (though rare issue)
    // Using TrackID is better for caching/reuse?
    // Let's use TrackID for recognizable names, but append UUID for uniqueness slightly?
    // Actually, simple is best. ID.knt
    QString filename = QString("kinetic_%1.knt").arg(track.remoteId);
    if (track.remoteId.isEmpty()) {
        filename = QString("kinetic_%1.knt").arg(QUuid::createUuid().toString());
    }

    QString fullPath = dir.filePath("mixxx/manifests/" + filename);

    // 3. Write File
    QFile file(fullPath);
    if (!file.open(QIODevice::WriteOnly)) {
        kLogger.warning() << "Failed to write manifest to" << fullPath;
        return QString();
    }

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();

    kLogger.debug() << "Generated manifest for" << track.title << "at" << fullPath;
    return fullPath;
}

} // namespace kinetic
} // namespace mixxx
