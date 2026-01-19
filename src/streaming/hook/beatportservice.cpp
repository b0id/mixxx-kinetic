#include "beatportservice.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

#include "oauthmanager.h"

BeatportService::BeatportService(QNetworkAccessManager* pNam, OAuthManager* pOAuthManager, QObject* parent)
        : StreamingService(parent),
          m_pNam(pNam),
          m_pOAuthManager(pOAuthManager),
          m_authState(AuthState::LoggedOut),
          m_subscriptionTier(SubscriptionTier::None) {
    setupOAuth();
}

BeatportService::~BeatportService() {
}

void BeatportService::setupOAuth() {
    // Register this service with the OAuth manager
    m_pOAuthManager->registerService(
            serviceId(),
            kClientId,
            kClientSecret,
            kAuthUrl,
            kTokenUrl,
            kScope);

    // Connect to OAuth manager signals
    connect(m_pOAuthManager, &OAuthManager::tokenRefreshed,
            this, &BeatportService::onAuthStateChanged);
    connect(m_pOAuthManager, &OAuthManager::authError,
            this, &BeatportService::onAuthError);
}

void BeatportService::initiateLogin() {
    m_authState = AuthState::Authorizing;
    emit authStateChanged(m_authState);

    // Start the device flow
    m_pOAuthManager->initiateDeviceFlow(serviceId());
}

void BeatportService::logout() {
    // Clear tokens from memory and keyring
    // TODO: Implement token clearing in OAuthManager

    m_authState = AuthState::LoggedOut;
    m_subscriptionTier = SubscriptionTier::None;
    emit authStateChanged(m_authState);
}

bool BeatportService::isAuthenticated() const {
    return m_pOAuthManager->hasValidToken(serviceId()) &&
           m_authState == AuthState::LoggedIn;
}

SubscriptionTier BeatportService::getSubscriptionTier() const {
    return m_subscriptionTier;
}

QUrl BeatportService::getAuthorizationUrl() const {
    return QUrl(kAuthUrl);
}

void BeatportService::onAuthStateChanged(const QString& serviceId) {
    if (serviceId != this->serviceId()) {
        return; // Not for us
    }

    m_authState = AuthState::LoggedIn;
    emit authStateChanged(m_authState);

    // Fetch subscription info to determine tier
    fetchSubscriptionInfo();
}

void BeatportService::onAuthError(const QString& serviceId, const QString& errorMsg) {
    if (serviceId != this->serviceId()) {
        return; // Not for us
    }

    m_authState = AuthState::Error;
    emit authStateChanged(m_authState);
    emit loginError(errorMsg);
}

void BeatportService::fetchSubscriptionInfo() {
    QString accessToken = m_pOAuthManager->getAccessToken(serviceId());
    if (accessToken.isEmpty()) {
        return;
    }

    QUrl url(QString("%1/my/subscriptions").arg(kBaseUrl));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "Mixxx-Kinetic/1.0");

    QNetworkReply* reply = m_pNam->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "BeatportService: Failed to fetch subscription info:" << reply->errorString();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();

        // Parse subscription tier
        // This is simplified - actual API response structure may vary
        QString plan = obj["plan"].toString().toLower();
        if (plan.contains("pro")) {
            m_subscriptionTier = SubscriptionTier::Professional;
        } else if (plan.contains("link")) {
            m_subscriptionTier = SubscriptionTier::Premium;
        } else {
            m_subscriptionTier = SubscriptionTier::Basic;
        }

        qDebug() << "BeatportService: Subscription tier:" << static_cast<int>(m_subscriptionTier);
    });
}

QFuture<TrackMetadata> BeatportService::fetchTrackMetadata(const QString& trackId) {
    QString accessToken = m_pOAuthManager->getAccessToken(serviceId());
    if (accessToken.isEmpty()) {
        // Return empty future - TODO: use QPromise for proper future handling
        return QFuture<TrackMetadata>();
    }

    QUrl url(QString("%1/catalog/tracks/%2").arg(kBaseUrl, trackId));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "Mixxx-Kinetic/1.0");

    QNetworkReply* reply = m_pNam->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "BeatportService: Failed to fetch track metadata:" << reply->errorString();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();

        TrackMetadata metadata = parseTrackMetadata(obj);
        // TODO: Signal or return through QPromise
    });

    return QFuture<TrackMetadata>();
}

QFuture<QVector<TrackMetadata>> BeatportService::search(const SearchQuery& query) {
    QString accessToken = m_pOAuthManager->getAccessToken(serviceId());
    if (accessToken.isEmpty()) {
        return QFuture<QVector<TrackMetadata>>();
    }

    QUrl url(QString("%1/catalog/search").arg(kBaseUrl));
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("q", query.text);
    if (!query.artist.isEmpty()) {
        urlQuery.addQueryItem("artist", query.artist);
    }
    if (!query.genre.isEmpty()) {
        urlQuery.addQueryItem("genre", query.genre);
    }
    urlQuery.addQueryItem("limit", QString::number(query.limit));
    urlQuery.addQueryItem("offset", QString::number(query.offset));
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "Mixxx-Kinetic/1.0");

    QNetworkReply* reply = m_pNam->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "BeatportService: Search failed:" << reply->errorString();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        QJsonArray results = obj["results"].toArray();

        QVector<TrackMetadata> tracks;
        for (const QJsonValue& value : results) {
            tracks.append(parseTrackMetadata(value.toObject()));
        }

        // TODO: Signal or return through QPromise
    });

    return QFuture<QVector<TrackMetadata>>();
}

QFuture<QVector<TrackMetadata>> BeatportService::getPlaylist(const QString& playlistId) {
    // TODO: Implement playlist fetching
    Q_UNUSED(playlistId);
    return QFuture<QVector<TrackMetadata>>();
}

QFuture<StreamInfo> BeatportService::getStreamInfo(const QString& trackId) {
    QString accessToken = m_pOAuthManager->getAccessToken(serviceId());
    if (accessToken.isEmpty()) {
        return QFuture<StreamInfo>();
    }

    QUrl url(QString("%1/catalog/tracks/%2/stream").arg(kBaseUrl, trackId));
    QUrlQuery urlQuery;
    // Request high quality if we have Premium/Pro subscription
    if (m_subscriptionTier >= SubscriptionTier::Premium) {
        urlQuery.addQueryItem("quality", "high");
    }
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "Mixxx-Kinetic/1.0");

    QNetworkReply* reply = m_pNam->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, trackId]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "BeatportService: Failed to get stream info:" << reply->errorString();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();

        StreamInfo info;
        info.trackId = trackId;
        info.streamUrl = QUrl(obj["stream_url"].toString());
        info.mimeType = "application/vnd.apple.mpegurl"; // HLS
        info.codec = "aac";
        info.encrypted = false; // Beatport typically unencrypted

        // Determine bitrate from quality
        QString quality = obj["quality"].toString();
        info.bitrate = (quality == "high") ? 256 : 128;

        // URL TTL is typically 1 hour
        info.expiresAt = QDateTime::currentDateTime().addSecs(3600);

        // TODO: Signal or return through QPromise
    });

    return QFuture<StreamInfo>();
}

TrackMetadata BeatportService::parseTrackMetadata(const QJsonObject& json) {
    TrackMetadata metadata;

    metadata.remoteId = QString::number(json["id"].toInt());
    metadata.title = json["name"].toString();
    metadata.album = json["release"].toObject()["name"].toString();
    metadata.bpm = json["bpm"].toInt();
    metadata.durationMs = json["length_ms"].toInt();
    metadata.genre = json["genre"].toObject()["name"].toString();
    metadata.year = json["release"].toObject()["year"].toInt();
    metadata.label = json["label"].toObject()["name"].toString();

    // Parse and normalize artists
    QJsonArray artistsArray = json["artists"].toArray();
    metadata.artist = normalizeArtists(artistsArray);

    // Parse and normalize key
    QJsonObject keyObj = json["key"].toObject();
    QString beatportKey = keyObj["name"].toString();
    metadata.key = normalizeKey(beatportKey);

    // Cover art
    QJsonObject imagesObj = json["images"].toObject();
    metadata.coverArtUrl = imagesObj["medium"].toObject()["url"].toString();
    // TODO: Download and cache cover art, set coverArtLocalPath

    metadata.source = this;

    return metadata;
}

QString BeatportService::normalizeKey(const QString& beatportKey) {
    // Beatport returns keys like "G# min", "A maj"
    // TODO: Implement proper key normalization to Camelot/OpenKey
    // For now, return as-is
    return beatportKey;
}

QString BeatportService::normalizeArtists(const QJsonArray& artistsArray) {
    QStringList artists;
    for (const QJsonValue& value : artistsArray) {
        QJsonObject artistObj = value.toObject();
        artists.append(artistObj["name"].toString());
    }
    return artists.join(", ");
}

#include "beatportservice.moc"
