#pragma once

#include <QFuture>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVector>

#include "streamingdto.h"
#include "streamingservice.h"

// Forward declarations
class OAuthManager;

class BeatportService : public StreamingService {
    Q_OBJECT
  public:
    explicit BeatportService(QNetworkAccessManager* pNam, OAuthManager* pOAuthManager, QObject* parent = nullptr);
    ~BeatportService() override;

    // Identity
    QString serviceName() const override { return "Beatport"; }
    QString serviceId() const override { return "beatport"; }

    // Authentication
    void initiateLogin() override;
    void logout() override;
    bool isAuthenticated() const override;
    SubscriptionTier getSubscriptionTier() const override;

    // Catalog
    QFuture<TrackMetadata> fetchTrackMetadata(const QString& trackId) override;
    QFuture<QVector<TrackMetadata>> search(const SearchQuery& query) override;
    QFuture<QVector<TrackMetadata>> getPlaylist(const QString& playlistId) override;

    // Streaming
    QFuture<StreamInfo> getStreamInfo(const QString& trackId) override;

  protected:
    QUrl getAuthorizationUrl() const override;

  private slots:
    void onAuthStateChanged(const QString& serviceId);
    void onAuthError(const QString& serviceId, const QString& errorMsg);

  private:
    QNetworkAccessManager* m_pNam;
    OAuthManager* m_pOAuthManager;
    AuthState m_authState;
    SubscriptionTier m_subscriptionTier;

    // API Configuration
    static constexpr const char* kBaseUrl = "https://api.beatport.com/v4";
    static constexpr const char* kAuthUrl = "https://api.beatport.com/v4/auth/o/authorize/";
    static constexpr const char* kTokenUrl = "https://api.beatport.com/v4/auth/o/token/";
    static constexpr const char* kClientId = "default"; // TODO: Replace with actual client ID
    static constexpr const char* kClientSecret = ""; // Optional for some flows
    static constexpr const char* kScope = "streaming catalog";

    // Helper methods
    void setupOAuth();
    void fetchSubscriptionInfo();
    TrackMetadata parseTrackMetadata(const QJsonObject& json);
    QString normalizeKey(const QString& beatportKey);
    QString normalizeArtists(const QJsonArray& artistsArray);
};
