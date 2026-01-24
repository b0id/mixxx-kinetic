#pragma once

#include <QDateTime>
#include <QFuture>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QString>
#include <QTimer>

// Forward declarations
class QNetworkAccessManager;
class StreamingService;

class OAuthManager : public QObject {
    Q_OBJECT
  public:
    enum class GrantType {
        AuthorizationCode,
        PKCE,
        DeviceFlow
    };

    struct TokenPair {
        QString accessToken;
        QString refreshToken;
        QDateTime expiresAt;
        QString scope;
    };

    explicit OAuthManager(QNetworkAccessManager* pNam, QObject* parent = nullptr);
    virtual ~OAuthManager();

    // Configuration
    void registerService(const QString& serviceId, const QString& clientId, const QString& clientSecret, const QString& authUrl, const QString& tokenUrl, const QString& scope);

    // Flow Management
    QFuture<TokenPair> initiateDeviceFlow(const QString& serviceId);
    QFuture<TokenPair> refreshTokens(const QString& serviceId);
    void setAccessTokens(const QString& serviceId, const TokenPair& tokens);

    // Storage (Keyring wrapper interaction)
    bool hasValidToken(const QString& serviceId) const;
    QString getAccessToken(const QString& serviceId);

    // Signals
    // For now, assuming synchronous lookup from memory/keyring is desired for speed
    // but keyring on Linux (DBus) can be async.
    // mixxx typically loads secrets at startup?
    // Let's implement getting from memory cache which is populated on startup/refresh.

    // Signals
  signals:
    void tokenRefreshed(const QString& serviceId);
    void authError(const QString& serviceId, const QString& errorMsg);
    void deviceCodeReceived(const QString& serviceId, const QString& userCode, const QString& verificationUrl);
    void browserUrlReady(const QString& serviceId, const QUrl& url);

  public:
    QFuture<TokenPair> initiateBrowserFlow(const QString& serviceId);
    void cancelBrowserFlow();

  private:
    QNetworkAccessManager* m_pNam;

    struct ServiceConfig {
        QString clientId;
        QString clientSecret;
        QString authUrl;
        QString tokenUrl;
        QString scope;
    };

    QMap<QString, ServiceConfig> m_services;

    // In-memory token cache (backed by libsecret)
    QMap<QString, TokenPair> m_tokenCache;

    // Polling state for device flow
    struct PollState {
        QString deviceCode;
        QString serviceId;
        int interval; // seconds
        int attempts;
        QTimer* timer;
    };
    QMap<QString, PollState*> m_activePollStates;

    void saveToKeyring(const QString& serviceId, const TokenPair& tokens);
    void loadFromKeyring(const QString& serviceId);
    void pollForToken(const QString& serviceId, const QString& deviceCode, int interval);
    void stopPolling(const QString& serviceId);

    // PKCE / Browser Flow
    class QTcpServer* m_tcpServer;
    QString m_currentPkceVerifier;
    QString m_currentServiceId;

    void startLocalServer();
    void stopLocalServer();
    void handleNewConnection();
    QString generateRandomString(int length);
    QString base64UrlEncode(const QByteArray& data);
};
