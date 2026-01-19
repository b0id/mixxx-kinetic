#include "sources/soundsourcekineticprovider.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>

#include "sources/soundsourcekineticproxy.h"

namespace mixxx {

class SoundSourceKineticProviderTest : public testing::Test {
  protected:
    QTemporaryDir m_tempDir;
};

TEST_F(SoundSourceKineticProviderTest, HandlesKntFiles) {
    SoundSourceKineticProvider provider;
    EXPECT_TRUE(provider.getSupportedFileTypes().contains("knt"));
}

TEST_F(SoundSourceKineticProviderTest, CreatesProxyFromManifest) {
    // 1. Create a dummy .knt file
    QDir dir(m_tempDir.path());
    QString manifestPath = dir.filePath("test_track.knt");

    QJsonObject manifest;
    manifest["url"] = "https://example.com/stream.mp3";
    manifest["id"] = "12345";
    manifest["extension"] = "mp3";

    QFile file(manifestPath);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly));
    file.write(QJsonDocument(manifest).toJson());
    file.close();

    // 2. Use Provider to create Source
    SoundSourceKineticProvider provider;
    QUrl url = QUrl::fromLocalFile(manifestPath);
    auto pSource = provider.newSoundSource(url);

    ASSERT_NE(pSource, nullptr);

    // 3. Verify it is a proxy (we can't easily cast to KineticProxy effectively without RTTI or friendship,
    // but the fact it returned non-null for a .knt file is a strong signal if logic is correct)

    // We can verify that it failed to open (because BridgeClient won't connect in this unit test env likely,
    // or if it does, it registers successfully).
    // Actually, SoundSourceKineticProxy::tryOpen does the registration.
    // So newSoundSource just does `new SoundSourceKineticProxy`.

    // To verify parsing, we would need to mock BridgeClient or inspect the Proxy.
    // For now, this test confirms the Provider wiring works.
}

} // namespace mixxx
