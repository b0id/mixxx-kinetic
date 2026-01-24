#include "beatportservice.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

#include "oauthmanager.h"
#include "streaming/hls/hlsparser.h"

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
    connect(m_pOAuthManager, &OAuthManager::tokenRefreshed, this, &BeatportService::onAuthStateChanged);
    connect(m_pOAuthManager, &OAuthManager::authError, this, &BeatportService::onAuthError);
    connect(m_pOAuthManager, &OAuthManager::deviceCodeReceived, this, [this](const QString& serviceId, const QString& userCode, const QString& url) {
        if (serviceId == this->serviceId()) {
            emit deviceAuthReady(userCode, QUrl(url));
        }
    });

    // Connect browser flow
    connect(m_pOAuthManager, &OAuthManager::browserUrlReady, this, [this](const QString& serviceId, const QUrl& url) {
        if (serviceId == this->serviceId()) {
            // Emitting empty userCode implies Browser Flow
            emit deviceAuthReady(QString(), url);
        }
    });
}

void BeatportService::initiateLogin() {
    m_authState = AuthState::Authorizing;
    emit authStateChanged(m_authState);

    // Start the device flow
    // Start the browser flow (PKCE)
    m_pOAuthManager->initiateBrowserFlow(serviceId());
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

        emit searchResultsReceived(tracks);
    });

    return QFuture<QVector<TrackMetadata>>();
}

QFuture<QVector<TrackMetadata>> BeatportService::getPlaylist(const QString& playlistId) {
    // TODO: Implement playlist fetching
    Q_UNUSED(playlistId);
    return QFuture<QVector<TrackMetadata>>();
}

QFuture<StreamInfo> BeatportService::getStreamInfo(const QString& trackId) {
    if (!m_pOAuthManager->hasValidToken(serviceId())) {
        return QFuture<StreamInfo>();
    }

    // Use the streaming endpoint (HLS)
    QUrl url(QString("%1/catalog/tracks/%2/stream/").arg(kBaseUrl, trackId));

    QNetworkRequest request(url);
    QString accessToken = m_pOAuthManager->getAccessToken(serviceId());
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

        QString streamUrl = obj["stream_url"].toString();
        if (streamUrl.isEmpty()) {
            qWarning() << "BeatportService: No stream_url in response";
            return;
        }

        // Fetch the M3U8 manifest to verify/prepare
        QNetworkRequest m3u8Req(streamUrl);
        // No auth needed for manifest usually, but let's see. Guide says public once generated.
        QNetworkReply* m3u8Reply = m_pNam->get(m3u8Req);
        connect(m3u8Reply, &QNetworkReply::finished, this, [this, m3u8Reply, trackId, streamUrl, obj]() {
            m3u8Reply->deleteLater();

            if (m3u8Reply->error() != QNetworkReply::NoError) {
                qWarning() << "BeatportService: Failed to fetch M3U8:" << m3u8Reply->errorString();
                return;
            }

            QByteArray m3u8Content = m3u8Reply->readAll();
            HlsParser parser;
            if (!parser.parse(m3u8Content, m3u8Reply->url())) {
                qWarning() << "BeatportService: Failed to parse M3U8";
                return;
            }

            // Successfully parsed HLS.
            // For now, we return the HLS URL.
            // In a full implementation, we might pre-fetch the key here or notify the FUSE layer.

            StreamInfo info;
            info.trackId = trackId;
            info.streamUrl = QUrl(streamUrl);
            info.mimeType = "application/vnd.apple.mpegurl";
            info.codec = "aac";
            info.encrypted = true; // HLS is encrypted
            info.bitrate = 128;    // Always 128k for streaming

            // Duration from JSON (sample_end_ms) or Parser?
            // JSON sample_end_ms should be accurate for full track if authorized correctly
            info.durationMs = obj["sample_end_ms"].toInt();

            // Set expiry
            // The URL expires, typically 1 hour?
            info.expiresAt = QDateTime::currentDateTime().addSecs(3600);

            // Emit result
            emit streamInfoReceived(info);

            // Optional: Fetch Key immediately to cache it?
            // HlsKey key = parser.key();
            // if (!key.uri.isEmpty()) { ... }
        });
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

void BeatportService::authenticate(const QString& username, const QString& password) {
    if (username.isEmpty() || password.isEmpty()) {
        emit authError(serviceId(), "Username and password required");
        return;
    }

    QUrl loginUrl(QString("%1/auth/login/").arg(kBaseUrl));
    QNetworkRequest loginReq(loginUrl);
    loginReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    loginReq.setRawHeader("User-Agent", "Mixxx-Kinetic/1.0");

    QJsonObject loginBody;
    loginBody["username"] = username;
    loginBody["password"] = password;

    QNetworkReply* loginReply = m_pNam->post(loginReq, QJsonDocument(loginBody).toJson());
    connect(loginReply, &QNetworkReply::finished, this, [this, loginReply]() {
        loginReply->deleteLater();

        if (loginReply->error() != QNetworkReply::NoError) {
            emit loginError(QString("Login failed: %1").arg(loginReply->errorString()));
            return;
        }

        // Extract sessionid cookie
        QList<QNetworkCookie> cookies = m_pNam->cookieJar()->cookiesForUrl(loginReply->url());
        QString sessionId;
        for (const auto& cookie : cookies) {
            if (cookie.name() == "sessionid") {
                sessionId = cookie.value();
                break;
            }
        }

        // Also check if set-cookie header has it manually if cookiejar fails
        if (sessionId.isEmpty()) {
            QVariant cookieVar = loginReply->header(QNetworkRequest::SetCookieHeader);
            if (cookieVar.isValid()) {
                QList<QNetworkCookie> headers = cookieVar.value<QList<QNetworkCookie>>();
                for (const auto& cookie : headers) {
                    if (cookie.name() == "sessionid") {
                        sessionId = cookie.value();
                        break;
                    }
                }
            }
        }

        if (sessionId.isEmpty()) {
            emit loginError("Login failed - no session cookie found");
            return;
        }

        // Step 2: Authorize
        authorizeWithSession(sessionId);
    });
}

void BeatportService::authorizeWithSession(const QString& sessionId) {
    QUrl authUrl(kAuthUrl);
    QUrlQuery query;
    query.addQueryItem("client_id", kClientId);
    query.addQueryItem("response_type", "code");
    authUrl.setQuery(query);

    QNetworkRequest req(authUrl);
    // Manually set cookie header
    req.setRawHeader("Cookie", QString("sessionid=%1").arg(sessionId).toUtf8());
    // Don't follow redirects automatically, we want the Location header
    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, false);

    QNetworkReply* reply = m_pNam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        // Expecting 302 Found
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if (statusCode.toInt() != 302) {
            // Sometimes it might return 200 if already authorized? Or 403?
            // But usually redirects to callback.
            // If we get 200, maybe we need to parse HTML?
            // For now assume redirect.
            emit loginError(QString("Authorize failed: Status %1").arg(statusCode.toInt()));
            return;
        }

        QString location = reply->header(QNetworkRequest::LocationHeader).toString();
        // Location: http://localhost:8889/callback?code=...
        QUrl locationUrl(location);
        QUrlQuery locQuery(locationUrl.query());
        QString code = locQuery.queryItemValue("code");

        if (code.isEmpty()) {
            emit loginError("Authorize failed: No code in redirect");
            return;
        }

        exchangeCodeForToken(code);
    });
}

void BeatportService::exchangeCodeForToken(const QString& code) {
    QNetworkRequest req(kTokenUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("grant_type", "authorization_code");
    params.addQueryItem("code", code);
    params.addQueryItem("client_id", kClientId);
    // Redirect URI must match what was implied or configured.
    // If we used the browser flow stub, it used localhost:8889.
    // The previous manual flow assumed specific redirect URI?
    // beatportdl uses "http://localhost:8889/callback"
    params.addQueryItem("redirect_uri", "http://localhost:8889/callback");

    QNetworkReply* reply = m_pNam->post(req, params.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit loginError(QString("Token exchange failed: %1").arg(reply->errorString()));
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();

        OAuthManager::TokenPair tokens;
        tokens.accessToken = obj["access_token"].toString();
        tokens.refreshToken = obj["refresh_token"].toString();
        tokens.expiresAt = QDateTime::currentDateTime().addSecs(obj["expires_in"].toInt(3600));
        tokens.scope = obj["scope"].toString();

        if (tokens.accessToken.isEmpty()) {
            emit loginError("Token exchange failed: Empty access token");
            return;
        }

        // Set tokens in OAuthManager
        m_pOAuthManager->setAccessTokens(serviceId(), tokens);
    });
}

#include "moc_beatportservice.cpp"
