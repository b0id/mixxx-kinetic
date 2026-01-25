#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcessEnvironment>
#include <QTimer>

#include "streaming/hook/beatportservice.h"
#include "streaming/hook/oauthmanager.h"

// Simple validation test for Beatport OAuth and API access
// Run with: BEATPORT_USER=xxx BEATPORT_PASS=yyy ./mixxx-beatport-verify

class BeatportVerifier : public QObject {
    Q_OBJECT
public:
    BeatportVerifier(const QString& username, const QString& password, QObject* parent = nullptr)
            : QObject(parent),
              m_nam(new QNetworkAccessManager(this)),
              m_oauth(new OAuthManager(m_nam, this)),
              m_service(new BeatportService(m_nam, m_oauth, this)),
              m_username(username),
              m_testsRun(0),
              m_testsPassed(0) {
        connect(m_service, &BeatportService::loginError, this, &BeatportVerifier::onLoginError);
        connect(m_service, &BeatportService::authStateChanged, this, &BeatportVerifier::onAuthStateChanged);
        connect(m_service, &BeatportService::streamInfoReceived, this, &BeatportVerifier::onStreamInfoReceived);
        connect(m_service, &BeatportService::searchResultsReceived, this, &BeatportVerifier::onSearchResults);
    }

    void start() {
        qDebug() << "\n========================================";
        qDebug() << "Beatport Integration Verification";
        qDebug() << "========================================\n";
        qDebug() << "[1/5] Authenticating as:" << m_username;
        m_testsRun++;  // Count auth as a test
        m_service->authenticate(m_username, m_password);
    }

    void setPassword(const QString& password) {
        m_password = password;
    }

private slots:
    void onLoginError(const QString& msg) {
        qCritical() << "  ✗ FAILED:" << msg;
        printSummary();
        QCoreApplication::exit(1);
    }

    void onAuthStateChanged(AuthState state) {
        if (state == AuthState::LoggedIn) {
            m_testsPassed++;  // Auth test passed (testsRun already incremented in start())
            qDebug() << "  ✓ Authentication successful";

            // Test 2: Verify token exists
            testTokenExists();
        }
    }

    void testTokenExists() {
        qDebug() << "\n[2/5] Verifying access token...";
        m_testsRun++;

        QString token = m_oauth->getAccessToken("beatport");
        if (token.isEmpty()) {
            qCritical() << "  ✗ FAILED: No access token stored";
        } else {
            m_testsPassed++;
            qDebug() << "  ✓ Access token acquired (length:" << token.length() << ")";
        }

        // Test 3: Call subscription endpoint to verify token works
        testSubscriptionEndpoint();
    }

    void testSubscriptionEndpoint() {
        qDebug() << "\n[3/5] Testing API access (/my/subscriptions)...";
        m_testsRun++;

        QString token = m_oauth->getAccessToken("beatport");
        QUrl url("https://api.beatport.com/v4/my/subscriptions");
        QNetworkRequest request(url);
        request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());
        request.setRawHeader("User-Agent", "Mixxx-Kinetic/1.0");

        QNetworkReply* reply = m_nam->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            reply->deleteLater();

            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QByteArray body = reply->readAll();

            if (reply->error() != QNetworkReply::NoError) {
                qCritical() << "  ✗ FAILED: HTTP" << statusCode << "-" << reply->errorString();
                qCritical() << "    Response:" << body.left(200);
            } else {
                m_testsPassed++;
                QJsonDocument doc = QJsonDocument::fromJson(body);
                qDebug() << "  ✓ API access confirmed (HTTP 200)";
                if (!doc.isNull()) {
                    qDebug() << "    Subscription data:" << doc.toJson(QJsonDocument::Compact).left(100) << "...";
                }
            }

            // Test 4: Search
            testSearch();
        });
    }

    void testSearch() {
        qDebug() << "\n[4/5] Testing search endpoint...";
        m_testsRun++;

        QString token = m_oauth->getAccessToken("beatport");
        QUrl url("https://api.beatport.com/v4/catalog/search/?q=deadmau5&per_page=3");
        QNetworkRequest request(url);
        request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());
        request.setRawHeader("User-Agent", "Mixxx-Kinetic/1.0");

        QNetworkReply* reply = m_nam->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            reply->deleteLater();

            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QByteArray body = reply->readAll();

            if (reply->error() != QNetworkReply::NoError) {
                qCritical() << "  ✗ FAILED: HTTP" << statusCode << "-" << reply->errorString();
                qCritical() << "    Response:" << body.left(200);
            } else {
                m_testsPassed++;
                QJsonDocument doc = QJsonDocument::fromJson(body);
                QJsonObject obj = doc.object();
                int count = obj["count"].toInt();
                qDebug() << "  ✓ Search works - found" << count << "results";

                // Get first track ID for stream test
                QJsonArray tracks = obj["tracks"].toArray();
                if (!tracks.isEmpty()) {
                    m_testTrackId = QString::number(tracks[0].toObject()["id"].toInt());
                    qDebug() << "    Using track ID:" << m_testTrackId << "for stream test";
                }
            }

            // Test 5: Stream endpoint
            testStreamEndpoint();
        });
    }

    void testStreamEndpoint() {
        qDebug() << "\n[5/5] Testing stream endpoint...";
        m_testsRun++;

        if (m_testTrackId.isEmpty()) {
            m_testTrackId = "23050990"; // Fallback known track
        }

        QString token = m_oauth->getAccessToken("beatport");
        QUrl url(QString("https://api.beatport.com/v4/catalog/tracks/%1/stream/").arg(m_testTrackId));
        QNetworkRequest request(url);
        request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());
        request.setRawHeader("User-Agent", "Mixxx-Kinetic/1.0");

        QNetworkReply* reply = m_nam->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            reply->deleteLater();

            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QByteArray body = reply->readAll();

            if (reply->error() != QNetworkReply::NoError) {
                qCritical() << "  ✗ FAILED: HTTP" << statusCode << "-" << reply->errorString();
                qCritical() << "    Response:" << body.left(300);
            } else {
                QJsonDocument doc = QJsonDocument::fromJson(body);
                QJsonObject obj = doc.object();
                QString streamUrl = obj["stream_url"].toString();
                int durationMs = obj["sample_end_ms"].toInt();

                if (!streamUrl.isEmpty()) {
                    m_testsPassed++;
                    qDebug() << "  ✓ Stream URL acquired";
                    qDebug() << "    Duration:" << (durationMs / 1000) << "seconds";
                    qDebug() << "    URL:" << streamUrl.left(80) << "...";
                } else {
                    qCritical() << "  ✗ FAILED: No stream_url in response";
                    qCritical() << "    Response:" << body.left(300);
                }
            }

            printSummary();
            QCoreApplication::exit(m_testsPassed == m_testsRun ? 0 : 1);
        });
    }

    void onStreamInfoReceived(const StreamInfo& info) {
        Q_UNUSED(info);
        // Not used in direct API tests
    }

    void onSearchResults(const QVector<StreamTrackMetadata>& results) {
        Q_UNUSED(results);
        // Not used in direct API tests
    }

private:
    void printSummary() {
        qDebug() << "\n========================================";
        qDebug() << "SUMMARY:" << m_testsPassed << "/" << m_testsRun << "tests passed";
        qDebug() << "========================================";
        if (m_testsPassed == m_testsRun) {
            qDebug() << "✓ ALL TESTS PASSED - Beatport integration is working!";
        } else {
            qCritical() << "✗ SOME TESTS FAILED - Check output above";
        }
        qDebug() << "";
    }

    QNetworkAccessManager* m_nam;
    OAuthManager* m_oauth;
    BeatportService* m_service;
    QString m_username;
    QString m_password;
    QString m_testTrackId;
    int m_testsRun;
    int m_testsPassed;
};

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    // Get credentials from environment
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString username = env.value("BEATPORT_USER");
    QString password = env.value("BEATPORT_PASS");

    if (username.isEmpty() || password.isEmpty()) {
        qCritical() << "Usage: BEATPORT_USER='username' BEATPORT_PASS='password' ./mixxx-beatport-verify";
        qCritical() << "";
        qCritical() << "This tool verifies that Beatport OAuth and API access work correctly.";
        qCritical() << "Set your Beatport credentials in environment variables and run.";
        qCritical() << "";
        qCritical() << "IMPORTANT: Use single quotes around your password to prevent shell expansion";
        qCritical() << "           of special characters like $ ! ` etc.";
        return 1;
    }

    BeatportVerifier verifier(username, password);
    verifier.setPassword(password);
    verifier.start();

    // Timeout
    QTimer::singleShot(30000, []() {
        qCritical() << "\n✗ TIMEOUT waiting for responses (30s)";
        QCoreApplication::exit(1);
    });

    return app.exec();
}

#include "beatport_integration_test.moc"
