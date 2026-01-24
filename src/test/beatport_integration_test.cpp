#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QProcessEnvironment>
#include <QTimer>

#include "streaming/hook/beatportservice.h"
#include "streaming/hook/oauthmanager.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    // Get credentials from environment
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString username = env.value("BEATPORT_USER");
    QString password = env.value("BEATPORT_PASS");
    // Optional: Allow passing track ID
    QString trackId = env.value("BEATPORT_TRACK_ID", "23050990"); // Default track

    if (username.isEmpty() || password.isEmpty()) {
        qCritical() << "Error: BEATPORT_USER and BEATPORT_PASS environment variables must be set.";
        return 1;
    }

    qDebug() << "Starting Beatport verification for user:" << username;

    QNetworkAccessManager nam;
    OAuthManager oauth(&nam);
    BeatportService service(&nam, &oauth);

    QObject::connect(&service, &BeatportService::loginError, [](const QString& msg) {
        qCritical() << "Login Error:" << msg;
        QCoreApplication::exit(1);
    });

    QObject::connect(&service, &BeatportService::authStateChanged, [&](StreamingService::AuthState state) {
        if (state == StreamingService::AuthState::LoggedIn) {
            qDebug() << "Authentication Successful!";
            qDebug() << "Fetching stream info for track:" << trackId;
            service.getStreamInfo(trackId);
        }
    });

    QObject::connect(&service, &BeatportService::streamInfoReceived, [&](const StreamInfo& info) {
        qDebug() << "Stream Info Received!";
        qDebug() << "Stream URL:" << info.streamUrl;
        qDebug() << "Bitrate:" << info.bitrate;
        qDebug() << "Encrypted:" << info.encrypted;
        qDebug() << "Expires:" << info.expiresAt;

        if (info.streamUrl.isValid()) {
            qDebug() << "VERIFICATION SUCCESS";
            QCoreApplication::exit(0);
        } else {
            qCritical() << "VERIFICATION FAILED: Invalid stream URL";
            QCoreApplication::exit(1);
        }
    });

    // Start Login
    qDebug() << "Authenticating...";
    service.authenticate(username, password);

    // Timeout
    QTimer::singleShot(30000, []() {
        qCritical() << "Timeout waiting for response.";
        QCoreApplication::exit(1);
    });

    return app.exec();
}
