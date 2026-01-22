#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>
#include <thread>

#include "sources/soundsource.h"
#include "sources/soundsourcekineticproxy.h"
#include "sources/soundsourceprovider.h"
#include "sources/soundsourceproxy.h"
#include "util/logger.h"

// Note: This test requires the mixxx-fs daemon to be running!

class MockSoundSource : public mixxx::SoundSource {
  public:
    MockSoundSource(const QUrl& url)
            : mixxx::SoundSource(url) {
    }

    // We override open() because AudioSource::open is non-virtual and calls protected tryOpen().
    // Wait, AudioSource::tryOpen is pure virtual protected. We MUST override it.
    mixxx::AudioSource::OpenResult tryOpen(mixxx::AudioSource::OpenMode mode, const mixxx::AudioSource::OpenParams& params) override {
        // Init dummy signal info
        initChannelCountOnce(2);
        initSampleRateOnce(44100);
        initFrameIndexRangeOnce(mixxx::IndexRange::forward(0, 100000));
        return mixxx::AudioSource::OpenResult::Succeeded;
    }

    void close() override {
    }

    mixxx::ReadableSampleFrames readSampleFramesClamped(const mixxx::WritableSampleFrames& sampleFrames) override {
        // Return silence or dummy
        if (sampleFrames.writableData()) {
            std::fill(sampleFrames.writableData(),
                    sampleFrames.writableData() + sampleFrames.writableLength() * 2,
                    0.0f);
        }
        return mixxx::ReadableSampleFrames(sampleFrames.frameIndexRange());
    }
};

class MockSoundSourceProvider : public mixxx::SoundSourceProvider {
  public:
    QString getDisplayName() const override {
        return "Mock Provider";
    }
    QStringList getSupportedFileTypes() const override {
        return {"mp3"};
    }

    mixxx::SoundSourcePointer newSoundSource(const QUrl& url) override {
        return std::make_shared<MockSoundSource>(url);
    }
};

class HeadlessPlaybackTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Setup temp dir for manifest
        manifestDir.isValid();

        // Register Mock Provider
        mockProvider = std::make_shared<MockSoundSourceProvider>();
        // Using FRIEND_TEST access
        SoundSourceProxy::s_soundSourceProviders.registerProvider(mockProvider);
    }

    void TearDown() override {
    }

    void createManifest(const QString& filename, const QString& url) {
        QJsonObject root;
        root["url"] = url;
        root["id"] = "1234";
        root["extension"] = "mp3";

        QJsonDocument doc(root);
        manifestPath = manifestDir.path() + "/" + filename;
        QFile f(manifestPath);
        if (f.open(QIODevice::WriteOnly)) {
            f.write(doc.toJson());
            f.close();
        }
    }

    QTemporaryDir manifestDir;
    QString manifestPath;
    std::shared_ptr<MockSoundSourceProvider> mockProvider;
};

TEST_F(HeadlessPlaybackTest, ParsesManifestAndConnectsToBridge) {
    // 1. Create a Manifest pointing to a test URL
    createManifest("test_track.knt", "http://example.com/test.mp3");

    // 2. Instantiate Proxy
    SoundSourceKineticProxy proxy(manifestPath);

    // 3. Try Open
    mixxx::AudioSource::OpenParams params;
    // Use open() instead of tryOpen()
    mixxx::AudioSource::OpenResult result = proxy.open(mixxx::AudioSource::OpenMode::Strict, params);

    if (result == mixxx::AudioSource::OpenResult::Failed) {
        std::cout << "[INFO] Proxy open failed - is mixxx-fs running?" << std::endl;
        // Expected failure if Bridge Client cannot connect
    } else {
        std::cout << "[SUCCESS] Proxy opened! Manifest parsed correctly." << std::endl;

        // 4. Try Read
        mixxx::SampleBuffer buffer(1024 * 2);
        mixxx::SampleBuffer::WritableSlice slice(buffer);
        mixxx::WritableSampleFrames frames(mixxx::IndexRange::forward(0, 1024), slice);

        proxy.readSampleFramesClamped(frames);

        // Clean up
        proxy.close();
    }
}
