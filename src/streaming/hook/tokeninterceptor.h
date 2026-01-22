#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "oauthmanager.h"

// Forward declaration
class RetryingNetworkReply;

class TokenInterceptor : public QNetworkAccessManager {
    Q_OBJECT
    friend class RetryingNetworkReply;

  public:
    explicit TokenInterceptor(OAuthManager* pOAuthManager, QObject* parent = nullptr);
    ~TokenInterceptor() override;

    // Helper for RetryingNetworkReply to call base implementation
    virtual QNetworkReply* createBaseRequest(Operation op, const QNetworkRequest& request, QIODevice* outgoingData);

    // Facade to OAuthManager
    QFuture<OAuthManager::TokenPair> refreshTokens(const QString& serviceId);
    QString getAccessToken(const QString& serviceId);

  signals:
    void tokenRefreshed(const QString& serviceId);
    void authError(const QString& serviceId, const QString& errorMsg);

  protected:
    QNetworkReply* createRequest(Operation op, const QNetworkRequest& request, QIODevice* outgoingData = nullptr) override;

  private:
    OAuthManager* m_pOAuthManager;
};
