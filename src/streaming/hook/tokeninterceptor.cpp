#include "tokeninterceptor.h"

#include <QDebug>
#include <QTimer>

// ============================================================================
// RetryingNetworkReply
// ============================================================================

class RetryingNetworkReply : public QNetworkReply {
    Q_OBJECT
  public:
    RetryingNetworkReply(TokenInterceptor* pInterceptor, QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* outgoingData);
    ~RetryingNetworkReply() override;

    void abort() override;
    void close() override;
    bool isSequential() const override;
    qint64 bytesAvailable() const override;
    qint64 readData(char* data, qint64 maxlen) override;

  private slots:
    void onBaseReplyFinished();
    void onBaseReplyError(QNetworkReply::NetworkError code);
    void onBaseReplyReadyRead();
    void onBaseReplyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void onTokenRefreshed(const QString& serviceId);
    void onAuthError(const QString& serviceId, const QString& errorMsg);

  private:
    void startRequest();
    void cleanupBaseReply();
    QString getServiceIdFromRequest();
    void retryWithNewToken();

    TokenInterceptor* m_pInterceptor;
    QNetworkAccessManager::Operation m_op;
    QNetworkRequest m_request;
    QIODevice* m_outgoingData;

    QNetworkReply* m_pBaseReply;
    bool m_retrying;
    bool m_waitingForRefresh;
};

RetryingNetworkReply::RetryingNetworkReply(TokenInterceptor* pInterceptor, QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice* outgoingData)
        : m_pInterceptor(pInterceptor),
          m_op(op),
          m_request(request),
          m_outgoingData(outgoingData),
          m_pBaseReply(nullptr),
          m_retrying(false),
          m_waitingForRefresh(false) {
    setRequest(request);
    setUrl(request.url());
    setOperation(op);

    startRequest();
}

RetryingNetworkReply::~RetryingNetworkReply() {
    cleanupBaseReply();
    if (m_waitingForRefresh) {
        disconnect(m_pInterceptor, nullptr, this, nullptr);
    }
}

void RetryingNetworkReply::startRequest() {
    cleanupBaseReply();

    m_pBaseReply = m_pInterceptor->createBaseRequest(m_op, m_request, m_outgoingData);

    if (!m_pBaseReply) {
        setError(QNetworkReply::UnknownNetworkError, "Failed to create request");
        emit errorOccurred(error());
        emit finished();
        return;
    }

    connect(m_pBaseReply, &QNetworkReply::readyRead, this, &RetryingNetworkReply::onBaseReplyReadyRead);
    connect(m_pBaseReply, &QNetworkReply::downloadProgress, this, &RetryingNetworkReply::onBaseReplyDownloadProgress);
    connect(m_pBaseReply, &QNetworkReply::finished, this, &RetryingNetworkReply::onBaseReplyFinished);
    connect(m_pBaseReply, &QNetworkReply::errorOccurred, this, &RetryingNetworkReply::onBaseReplyError);
}

void RetryingNetworkReply::cleanupBaseReply() {
    if (m_pBaseReply) {
        disconnect(m_pBaseReply, nullptr, this, nullptr);
        m_pBaseReply->deleteLater();
        m_pBaseReply = nullptr;
    }
}

void RetryingNetworkReply::abort() {
    if (m_pBaseReply)
        m_pBaseReply->abort();
}

void RetryingNetworkReply::close() {
    if (m_pBaseReply)
        m_pBaseReply->close();
    QNetworkReply::close();
}

bool RetryingNetworkReply::isSequential() const {
    return true;
}

qint64 RetryingNetworkReply::bytesAvailable() const {
    return m_pBaseReply ? m_pBaseReply->bytesAvailable() : 0;
}

qint64 RetryingNetworkReply::readData(char* data, qint64 maxlen) {
    return m_pBaseReply ? m_pBaseReply->read(data, maxlen) : -1;
}

void RetryingNetworkReply::onBaseReplyReadyRead() {
    emit readyRead();
}

void RetryingNetworkReply::onBaseReplyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    emit downloadProgress(bytesReceived, bytesTotal);
}

void RetryingNetworkReply::onBaseReplyError(QNetworkReply::NetworkError code) {
    Q_UNUSED(code);
    // Suppress error emission until we determine if handled
}

void RetryingNetworkReply::onBaseReplyFinished() {
    if (!m_pBaseReply)
        return;

    // Check status
    int statusCode = m_pBaseReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QNetworkReply::NetworkError err = m_pBaseReply->error();

    // If 401 and we haven't retried yet
    if (statusCode == 401 && !m_retrying) {
        if (m_outgoingData && m_outgoingData->isSequential()) {
            qWarning() << "TokenInterceptor: Cannot retry 401 with sequential outgoing data";
        } else {
            QString serviceId = getServiceIdFromRequest();
            if (!serviceId.isEmpty()) {
                qDebug() << "TokenInterceptor: 401 detected for" << serviceId << "- waiting for refresh";
                m_retrying = true;

                // Cleanup invalid reply
                cleanupBaseReply();

                // Wait for signal
                m_waitingForRefresh = true;
                connect(m_pInterceptor, &TokenInterceptor::tokenRefreshed, this, &RetryingNetworkReply::onTokenRefreshed);
                connect(m_pInterceptor, &TokenInterceptor::authError, this, &RetryingNetworkReply::onAuthError);

                // Initiate refresh (signals will fire when done)
                m_pInterceptor->refreshTokens(serviceId);
                return;
            }
        }
    }

    // If logic reaches here: not 401, or already retried and failed again, or cannot retry

    // Propagate errors
    if (err != QNetworkReply::NoError) {
        setError(err, m_pBaseReply->errorString());
        emit errorOccurred(err);
    }

    // Propagate attributes
    setAttribute(QNetworkRequest::HttpStatusCodeAttribute, statusCode);
    // TODO: Copy other attributes/headers if critical

    emit finished();
}

void RetryingNetworkReply::onTokenRefreshed(const QString& serviceId) {
    if (!m_waitingForRefresh)
        return;

    QString myServiceId = getServiceIdFromRequest();
    if (serviceId != myServiceId)
        return; // Not for us

    qDebug() << "TokenInterceptor: Token refreshed for" << serviceId << "- retrying request";

    // Stop listening
    disconnect(m_pInterceptor, &TokenInterceptor::tokenRefreshed, this, &RetryingNetworkReply::onTokenRefreshed);
    disconnect(m_pInterceptor, &TokenInterceptor::authError, this, &RetryingNetworkReply::onAuthError);
    m_waitingForRefresh = false;

    retryWithNewToken();
}

void RetryingNetworkReply::onAuthError(const QString& serviceId, const QString& errorMsg) {
    if (!m_waitingForRefresh)
        return;

    QString myServiceId = getServiceIdFromRequest();
    if (serviceId != myServiceId)
        return;

    qWarning() << "TokenInterceptor: Auth error for" << serviceId << ":" << errorMsg << "- aborting retry";

    disconnect(m_pInterceptor, &TokenInterceptor::tokenRefreshed, this, &RetryingNetworkReply::onTokenRefreshed);
    disconnect(m_pInterceptor, &TokenInterceptor::authError, this, &RetryingNetworkReply::onAuthError);
    m_waitingForRefresh = false;

    // Fail
    setError(QNetworkReply::AuthenticationRequiredError, errorMsg);
    emit errorOccurred(error());
    emit finished();
}

void RetryingNetworkReply::retryWithNewToken() {
    QString serviceId = getServiceIdFromRequest();
    QString newToken = m_pInterceptor->getAccessToken(serviceId);

    if (newToken.isEmpty()) {
        qWarning() << "TokenInterceptor: Refresh succeeded but token empty?";
        setError(QNetworkReply::AuthenticationRequiredError, "Token empty after refresh");
        emit errorOccurred(error());
        emit finished();
        return;
    }

    // Update header
    m_request.setRawHeader("Authorization", QString("Bearer %1").arg(newToken).toUtf8());

    startRequest();
}

QString RetryingNetworkReply::getServiceIdFromRequest() {
    QString host = m_request.url().host();
    if (host.contains("beatport"))
        return "beatport";
    if (host.contains("tidal"))
        return "tidal";
    if (host.contains("soundcloud"))
        return "soundcloud";
    return "";
}

// ============================================================================
// TokenInterceptor
// ============================================================================

TokenInterceptor::TokenInterceptor(OAuthManager* pOAuthManager, QObject* parent)
        : QNetworkAccessManager(parent),
          m_pOAuthManager(pOAuthManager) {
    // Forward signals from OAuthManager
    connect(m_pOAuthManager, &OAuthManager::tokenRefreshed, this, &TokenInterceptor::tokenRefreshed);
    connect(m_pOAuthManager, &OAuthManager::authError, this, &TokenInterceptor::authError);
}

TokenInterceptor::~TokenInterceptor() {
}

QNetworkReply* TokenInterceptor::createBaseRequest(Operation op, const QNetworkRequest& request, QIODevice* outgoingData) {
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}

QFuture<OAuthManager::TokenPair> TokenInterceptor::refreshTokens(const QString& serviceId) {
    return m_pOAuthManager->refreshTokens(serviceId);
}

QString TokenInterceptor::getAccessToken(const QString& serviceId) {
    return m_pOAuthManager->getAccessToken(serviceId);
}

QNetworkReply* TokenInterceptor::createRequest(Operation op, const QNetworkRequest& request, QIODevice* outgoingData) {
    QString host = request.url().host();
    if (host.contains("beatport") || host.contains("tidal") || host.contains("soundcloud")) {
        return new RetryingNetworkReply(this, op, request, outgoingData);
    }
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}

#include "moc_tokeninterceptor.cpp"
#include "tokeninterceptor.moc"
