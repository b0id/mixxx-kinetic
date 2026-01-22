#include "oauthmanager.h"

#include <QCryptographicHash>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QRandomGenerator>
#include <QTcpServer>
#include <QTcpSocket>

#ifdef __QTKEYCHAIN__
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qt6keychain/keychain.h>
#else
#include <qt5keychain/keychain.h>
#endif
using namespace QKeychain;
#endif // __QTKEYCHAIN__

OAuthManager::OAuthManager(QNetworkAccessManager* pNam, QObject* parent)
        : QObject(parent),
          m_pNam(pNam),
          m_tcpServer(nullptr) {
}

OAuthManager::~OAuthManager() {
}

void OAuthManager::registerService(const QString& serviceId, const QString& clientId, const QString& clientSecret, const QString& authUrl, const QString& tokenUrl, const QString& scope) {
    ServiceConfig config;
    config.clientId = clientId;
    config.clientSecret = clientSecret;
    config.authUrl = authUrl;
    config.tokenUrl = tokenUrl;
    config.scope = scope;
    m_services.insert(serviceId, config);
}

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

// ... (other methods)

QFuture<OAuthManager::TokenPair> OAuthManager::initiateDeviceFlow(const QString& serviceId) {
    if (!m_services.contains(serviceId)) {
        emit authError(serviceId, "Service not registered");
        return QFuture<TokenPair>();
    }

    const auto& config = m_services[serviceId];

    QNetworkRequest request(config.authUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("client_id", config.clientId);
    if (!config.scope.isEmpty()) {
        params.addQueryItem("scope", config.scope);
    }

    QNetworkReply* reply = m_pNam->post(request, params.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, [this, reply, serviceId]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit authError(serviceId, reply->errorString());
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();

        QString deviceCode = obj["device_code"].toString();
        QString userCode = obj["user_code"].toString();
        QString verificationUri = obj["verification_uri"].toString();
        // verification_uri_complete might also be present

        if (userCode.isEmpty() || verificationUri.isEmpty()) {
            emit authError(serviceId, "Invalid response: Missing user_code or verification_uri");
            return;
        }

        emit deviceCodeReceived(serviceId, userCode, verificationUri);

        // Start polling with deviceCode
        int interval = obj["interval"].toInt(5); // Default to 5 seconds if not specified
        pollForToken(serviceId, deviceCode, interval);
    });

    return QFuture<TokenPair>();
}

QFuture<OAuthManager::TokenPair> OAuthManager::refreshTokens(const QString& serviceId) {
    if (!m_services.contains(serviceId)) {
        emit authError(serviceId, "Service not registered");
        return QFuture<TokenPair>();
    }

    if (!m_tokenCache.contains(serviceId) || m_tokenCache[serviceId].refreshToken.isEmpty()) {
        emit authError(serviceId, "No refresh token available");
        return QFuture<TokenPair>();
    }

    const auto& config = m_services[serviceId];
    const QString& refreshToken = m_tokenCache[serviceId].refreshToken;

    QNetworkRequest request(config.tokenUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("grant_type", "refresh_token");
    params.addQueryItem("refresh_token", refreshToken);
    params.addQueryItem("client_id", config.clientId);
    if (!config.clientSecret.isEmpty()) {
        params.addQueryItem("client_secret", config.clientSecret);
    }

    QNetworkReply* reply = m_pNam->post(request, params.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, [this, reply, serviceId]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit authError(serviceId, QString("Token refresh failed: %1").arg(reply->errorString()));
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();

        TokenPair tokens;
        tokens.accessToken = obj["access_token"].toString();
        tokens.refreshToken = obj["refresh_token"].toString();
        tokens.scope = obj["scope"].toString();

        // Calculate expiration time
        int expiresIn = obj["expires_in"].toInt(3600);
        tokens.expiresAt = QDateTime::currentDateTime().addSecs(expiresIn);

        // If server didn't provide a new refresh token, keep the old one
        if (tokens.refreshToken.isEmpty() && m_tokenCache.contains(serviceId)) {
            tokens.refreshToken = m_tokenCache[serviceId].refreshToken;
        }

        if (tokens.accessToken.isEmpty()) {
            emit authError(serviceId, "Invalid response: Missing access_token");
            return;
        }

        // Store tokens
        m_tokenCache.insert(serviceId, tokens);
        saveToKeyring(serviceId, tokens);

        emit tokenRefreshed(serviceId);
    });

    return QFuture<TokenPair>();
}

bool OAuthManager::hasValidToken(const QString& serviceId) const {
    return m_tokenCache.contains(serviceId);
}

QString OAuthManager::getAccessToken(const QString& serviceId) {
    if (m_tokenCache.contains(serviceId)) {
        return m_tokenCache[serviceId].accessToken;
    }
    return QString();
}

void OAuthManager::pollForToken(const QString& serviceId, const QString& deviceCode, int interval) {
    // Clean up any existing poll state for this service
    stopPolling(serviceId);

    // Create new poll state
    PollState* state = new PollState();
    state->deviceCode = deviceCode;
    state->serviceId = serviceId;
    state->interval = interval * 1000; // Convert to milliseconds
    state->attempts = 0;
    state->timer = new QTimer(this);

    m_activePollStates.insert(serviceId, state);

    // Connect timer to polling logic
    connect(state->timer, &QTimer::timeout, this, [this, serviceId]() {
        if (!m_activePollStates.contains(serviceId)) {
            return; // Poll was cancelled
        }

        PollState* pollState = m_activePollStates[serviceId];
        pollState->attempts++;

        // RFC 8628: Stop polling after reasonable attempts (e.g., 300 seconds / 5 minutes)
        if (pollState->attempts * pollState->interval / 1000 > 300) {
            stopPolling(serviceId);
            emit authError(serviceId, "Device authorization expired: user did not complete login in time");
            return;
        }

        const auto& config = m_services[serviceId];

        QNetworkRequest request(config.tokenUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QUrlQuery params;
        params.addQueryItem("grant_type", "urn:ietf:params:oauth:grant-type:device_code");
        params.addQueryItem("device_code", pollState->deviceCode);
        params.addQueryItem("client_id", config.clientId);

        QNetworkReply* reply = m_pNam->post(request, params.toString(QUrl::FullyEncoded).toUtf8());

        connect(reply, &QNetworkReply::finished, this, [this, reply, serviceId]() {
            reply->deleteLater();

            if (!m_activePollStates.contains(serviceId)) {
                return; // Poll was cancelled during request
            }

            PollState* pollState = m_activePollStates[serviceId];

            if (reply->error() != QNetworkReply::NoError) {
                QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject obj = doc.object();
                QString error = obj["error"].toString();

                if (error == "authorization_pending") {
                    // Continue polling - user hasn't approved yet
                    return;
                } else if (error == "slow_down") {
                    // Increase polling interval as requested by server
                    pollState->interval += 5000; // Add 5 seconds
                    pollState->timer->setInterval(pollState->interval);
                    return;
                } else if (error == "expired_token") {
                    stopPolling(serviceId);
                    emit authError(serviceId, "Device code expired");
                    return;
                } else if (error == "access_denied") {
                    stopPolling(serviceId);
                    emit authError(serviceId, "User denied authorization");
                    return;
                } else {
                    stopPolling(serviceId);
                    emit authError(serviceId, QString("Token request failed: %1").arg(error.isEmpty() ? reply->errorString() : error));
                    return;
                }
            }

            // Success! Parse tokens
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();

            TokenPair tokens;
            tokens.accessToken = obj["access_token"].toString();
            tokens.refreshToken = obj["refresh_token"].toString();
            tokens.scope = obj["scope"].toString();

            // Calculate expiration time
            int expiresIn = obj["expires_in"].toInt(3600); // Default to 1 hour
            tokens.expiresAt = QDateTime::currentDateTime().addSecs(expiresIn);

            if (tokens.accessToken.isEmpty()) {
                stopPolling(serviceId);
                emit authError(serviceId, "Invalid response: Missing access_token");
                return;
            }

            // Store tokens
            m_tokenCache.insert(serviceId, tokens);
            saveToKeyring(serviceId, tokens);

            // Stop polling and signal success
            stopPolling(serviceId);
            emit tokenRefreshed(serviceId);
        });
    });

    // Start the timer
    state->timer->start(state->interval);
}

void OAuthManager::stopPolling(const QString& serviceId) {
    if (m_activePollStates.contains(serviceId)) {
        PollState* state = m_activePollStates.take(serviceId);
        if (state->timer) {
            state->timer->stop();
            state->timer->deleteLater();
        }
        delete state;
    }
}

void OAuthManager::saveToKeyring(const QString& serviceId, const TokenPair& tokens) {
#ifdef __QTKEYCHAIN__
    QString serviceName = QString("Mixxx - Kinetic - OAuth - %1").arg(serviceId);

    // Serialize TokenPair to JSON
    QJsonObject jsonObj;
    jsonObj["access_token"] = tokens.accessToken;
    jsonObj["refresh_token"] = tokens.refreshToken;
    jsonObj["expires_at"] = tokens.expiresAt.toString(Qt::ISODate);
    jsonObj["scope"] = tokens.scope;

    QJsonDocument doc(jsonObj);
    QString jsonData = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));

    WritePasswordJob writeJob(serviceName);
    writeJob.setAutoDelete(false);
    writeJob.setKey("tokens");
    writeJob.setTextData(jsonData);

    QEventLoop loop;
    writeJob.connect(&writeJob, &WritePasswordJob::finished, &loop, &QEventLoop::quit);
    writeJob.start();
    loop.exec();

    if (writeJob.error() != Error::NoError) {
        qWarning() << "OAuthManager::saveToKeyring: Failed to save tokens for" << serviceId
                   << ":" << writeJob.errorString();
    }
#else
    Q_UNUSED(serviceId);
    Q_UNUSED(tokens);
    qWarning() << "OAuthManager::saveToKeyring: QtKeychain not available, tokens not persisted";
#endif
}

void OAuthManager::loadFromKeyring(const QString& serviceId) {
#ifdef __QTKEYCHAIN__
    QString serviceName = QString("Mixxx - Kinetic - OAuth - %1").arg(serviceId);

    ReadPasswordJob readJob(serviceName);
    readJob.setAutoDelete(false);
    readJob.setKey("tokens");

    QEventLoop loop;
    readJob.connect(&readJob, &ReadPasswordJob::finished, &loop, &QEventLoop::quit);
    readJob.start();
    loop.exec();

    if (readJob.error() == Error::NoError) {
        QString jsonData = readJob.textData();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());

        if (!doc.isNull() && doc.isObject()) {
            QJsonObject jsonObj = doc.object();

            TokenPair tokens;
            tokens.accessToken = jsonObj["access_token"].toString();
            tokens.refreshToken = jsonObj["refresh_token"].toString();
            tokens.expiresAt = QDateTime::fromString(jsonObj["expires_at"].toString(), Qt::ISODate);
            tokens.scope = jsonObj["scope"].toString();

            // Only cache if we have valid data
            if (!tokens.accessToken.isEmpty()) {
                m_tokenCache.insert(serviceId, tokens);
            }
        }
    } else if (readJob.error() != Error::EntryNotFound) {
        qWarning() << "OAuthManager::loadFromKeyring: Failed to read tokens for" << serviceId
                   << ":" << readJob.errorString();
    }
    // EntryNotFound is expected for new services, no warning needed
#else
    Q_UNUSED(serviceId);
#endif
}

// PKCE / Browser Flow Implementation

QString OAuthManager::generateRandomString(int length) {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~");
    QString randomString;
    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->generate() % possibleCharacters.length();
        randomString.append(possibleCharacters.at(index));
    }
    return randomString;
}

QString OAuthManager::base64UrlEncode(const QByteArray& data) {
    return QString::fromLatin1(data.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
}

QFuture<OAuthManager::TokenPair> OAuthManager::initiateBrowserFlow(const QString& serviceId) {
    if (!m_services.contains(serviceId)) {
        emit authError(serviceId, "Service not registered");
        return QFuture<TokenPair>();
    }

    // Stop any existing server
    stopLocalServer();

    m_currentServiceId = serviceId;
    m_currentPkceVerifier = generateRandomString(128);

    // Create S256 Challenge
    QByteArray hash = QCryptographicHash::hash(m_currentPkceVerifier.toLatin1(), QCryptographicHash::Sha256);
    QString challenge = base64UrlEncode(hash);

    // Start Local Server
    startLocalServer();

    const auto& config = m_services[serviceId];
    QString redirectUri = "http://localhost:8889/callback";

    QUrl authUrl(config.authUrl);
    QUrlQuery params;
    params.addQueryItem("response_type", "code");
    params.addQueryItem("client_id", config.clientId);
    params.addQueryItem("redirect_uri", redirectUri);
    // params.addQueryItem("state", "xyz"); // TODO: Add state
    if (!config.scope.isEmpty()) {
        params.addQueryItem("scope", config.scope);
    }
    params.addQueryItem("code_challenge", challenge);
    params.addQueryItem("code_challenge_method", "S256");

    authUrl.setQuery(params);

    emit browserUrlReady(serviceId, authUrl);

    return QFuture<TokenPair>();
}

void OAuthManager::cancelBrowserFlow() {
    stopLocalServer();
}

void OAuthManager::startLocalServer() {
    if (!m_tcpServer) {
        m_tcpServer = new QTcpServer(this);
        connect(m_tcpServer, &QTcpServer::newConnection, this, &OAuthManager::handleNewConnection);
    }
    if (!m_tcpServer->isListening()) {
        if (!m_tcpServer->listen(QHostAddress::LocalHost, 8889)) {
            emit authError(m_currentServiceId, "Failed to start local server on port 8889");
        }
    }
}

void OAuthManager::stopLocalServer() {
    if (m_tcpServer && m_tcpServer->isListening()) {
        m_tcpServer->close();
    }
}

void OAuthManager::handleNewConnection() {
    QTcpSocket* socket = m_tcpServer->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
        QByteArray requestData = socket->readAll();
        QString request = QString::fromUtf8(requestData);

        // Simple parsing of GET line
        // GET /callback?code=XXXX HTTP/1.1
        QStringList lines = request.split("\r\n");
        if (lines.isEmpty())
            return;

        QStringList parts = lines[0].split(" ");
        if (parts.length() < 2)
            return;

        QString path = parts[1];
        QUrl url("http://localhost:8889" + path);
        QUrlQuery query(url.query());

        QString code = query.queryItemValue("code");
        QString error = query.queryItemValue("error");

        if (!code.isEmpty()) {
            // Send Success Response
            QString response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Login Successful</h1><p>You can close this window and return to Mixxx.</p><script>window.close();</script>";
            socket->write(response.toUtf8());
            socket->flush();
            socket->disconnectFromHost(); // wait for bytes written?

            // Exchange code for token
            // TODO: Move to helper
            const auto& config = m_services[m_currentServiceId];

            QNetworkRequest tokenReq(config.tokenUrl);
            tokenReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

            QUrlQuery params;
            params.addQueryItem("grant_type", "authorization_code");
            params.addQueryItem("code", code);
            params.addQueryItem("redirect_uri", "http://localhost:8889/callback");
            params.addQueryItem("client_id", config.clientId);
            params.addQueryItem("code_verifier", m_currentPkceVerifier);
            // params.addQueryItem("client_secret", config.clientSecret); // Not needed for PKCE usually, but maybe?

            QNetworkReply* reply = m_pNam->post(tokenReq, params.toString(QUrl::FullyEncoded).toUtf8());
            connect(reply, &QNetworkReply::finished, this, [this, reply, serviceId = m_currentServiceId]() {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) {
                    emit authError(serviceId, reply->errorString());
                    return;
                }

                QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject obj = doc.object();

                TokenPair tokens;
                tokens.accessToken = obj["access_token"].toString();
                tokens.refreshToken = obj["refresh_token"].toString();
                int expiresIn = obj["expires_in"].toInt(3600);
                tokens.expiresAt = QDateTime::currentDateTime().addSecs(expiresIn);
                tokens.scope = obj["scope"].toString();

                if (tokens.accessToken.isEmpty()) {
                    emit authError(serviceId, "Invalid response: Missing access_token");
                    return;
                }

                m_tokenCache.insert(serviceId, tokens);
                saveToKeyring(serviceId, tokens);
                emit tokenRefreshed(serviceId);

                // Stop server
                stopLocalServer();
            });

        } else {
            QString response = "HTTP/1.1 400 Bad Request\r\n\r\nError";
            socket->write(response.toUtf8());
            socket->disconnectFromHost();
            if (!error.isEmpty()) {
                emit authError(m_currentServiceId, error);
            }
        }
    });
}

#include "moc_oauthmanager.cpp"
