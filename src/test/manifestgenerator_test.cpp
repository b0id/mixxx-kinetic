#include "streaming/manifestgenerator.h"

#include <gtest/gtest.h>

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

#include "streaming/hook/streamingdto.h"

namespace mixxx {
namespace kinetic {

TEST(ManifestGeneratorTest, GeneratesValidJson) {
    TrackMetadata track;
    track.remoteId = "12345";
    track.title = "Test Track";
    track.artist = "Test Artist";

    QString url = "http://example.com/stream";
    QString extension = "mp3";
    int bitrate = 320;

    QString path = ManifestGenerator::generate(track, url, extension, bitrate);
    ASSERT_FALSE(path.isEmpty());
    ASSERT_TRUE(QFile::exists(path));

    QFile f(path);
    ASSERT_TRUE(f.open(QIODevice::ReadOnly));
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    ASSERT_FALSE(doc.isNull());

    QJsonObject root = doc.object();
    EXPECT_EQ(root["url"].toString(), url);
    EXPECT_EQ(root["id"].toString(), track.remoteId);
    EXPECT_EQ(root["extension"].toString(), extension);
    EXPECT_EQ(root["bitrate"].toInt(), bitrate);
    EXPECT_EQ(root["title"].toString(), track.title);

    f.close();
    QFile::remove(path);
}

TEST(ManifestGeneratorTest, HandlesEmptyUrl) {
    TrackMetadata track;
    track.remoteId = "12345";

    QString path = ManifestGenerator::generate(track, "", "mp3", 320);
    EXPECT_TRUE(path.isEmpty());
}

} // namespace kinetic
} // namespace mixxx
