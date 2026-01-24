# Session Cookie Auth Flow Implementation

**Problem:** Password grant returns "unauthorized_client" because the client_id doesn't support it.

**Solution:** Implement beatportdl's 3-step session cookie flow.

---

## The Flow

```
User enters credentials
         ↓
Step 1: POST /auth/login/
         → Returns sessionid cookie
         ↓
Step 2: GET /auth/o/authorize/?client_id=...&response_type=code
         with Cookie: sessionid=...
         → Redirects to ?code=AUTH_CODE
         ↓
Step 3: POST /auth/o/token/
         with grant_type=authorization_code&code=AUTH_CODE
         → Returns access_token
```

---

## Code Changes Required

### 1. Add to `oauthmanager.h`

```cpp
QFuture<TokenPair> initiateSessionFlow(
    const QString& serviceId,
    const QString& username,
    const QString& password);

private:
    // Helper for session flow
    QString extractCodeFromRedirect(const QUrl& redirectUrl);
```

### 2. Add to `oauthmanager.cpp`

```cpp
QFuture<OAuthManager::TokenPair> OAuthManager::initiateSessionFlow(
        const QString& serviceId,
        const QString& username,
        const QString& password) {

    if (!m_services.contains(serviceId)) {
        emit authError(serviceId, "Service not registered");
        return QFuture<TokenPair>();
    }

    if (username.isEmpty() || password.isEmpty()) {
        emit authError(serviceId, "Username and password required");
        return QFuture<TokenPair>();
    }

    const auto& config = m_services[serviceId];

    // STEP 1: Login to get session cookie
    QUrl loginUrl(QString("%1/auth/login/").arg(config.baseUrl));
    QNetworkRequest loginRequest(loginUrl);
    loginRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject loginPayload;
    loginPayload["username"] = username;
    loginPayload["password"] = password;
    QByteArray loginData = QJsonDocument(loginPayload).toJson();

    QNetworkReply* loginReply = m_pNam->post(loginRequest, loginData);

    connect(loginReply, &QNetworkReply::finished, this,
            [this, loginReply, serviceId, config]() {
        loginReply->deleteLater();

        if (loginReply->error() != QNetworkReply::NoError) {
            emit authError(serviceId,
                QString("Login failed: %1").arg(loginReply->errorString()));
            return;
        }

        // Extract sessionid cookie
        QString sessionId;
        for (const QNetworkCookie& cookie : loginReply->header(
                QNetworkRequest::SetCookieHeader).value<QList<QNetworkCookie>>()) {
            if (cookie.name() == "sessionid") {
                sessionId = cookie.value();
                break;
            }
        }

        if (sessionId.isEmpty()) {
            emit authError(serviceId, "No session cookie received from login");
            return;
        }

        qInfo() << "OAuthManager: Session cookie obtained";

        // STEP 2: Get authorization code using session cookie
        QUrl authUrl(QString("%1/auth/o/authorize/").arg(config.baseUrl));
        QUrlQuery authQuery;
        authQuery.addQueryItem("client_id", config.clientId);
        authQuery.addQueryItem("response_type", "code");
        authUrl.setQuery(authQuery);

        QNetworkRequest authRequest(authUrl);
        authRequest.setRawHeader("Cookie",
            QString("sessionid=%1").arg(sessionId).toUtf8());
        // Don't follow redirects - we want to capture the Location header
        authRequest.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
            QNetworkRequest::ManualRedirectPolicy);

        QNetworkReply* authReply = m_pNam->get(authRequest);

        connect(authReply, &QNetworkReply::finished, this,
                [this, authReply, serviceId, config]() {
            authReply->deleteLater();

            // Get redirect location
            QUrl redirectUrl = authReply->attribute(
                QNetworkRequest::RedirectionTargetAttribute).toUrl();

            if (redirectUrl.isEmpty()) {
                emit authError(serviceId, "No redirect from authorize endpoint");
                return;
            }

            // Extract code from redirect URL
            QUrlQuery query(redirectUrl);
            QString code = query.queryItemValue("code");

            if (code.isEmpty()) {
                emit authError(serviceId, "No authorization code in redirect");
                return;
            }

            qInfo() << "OAuthManager: Authorization code obtained";

            // STEP 3: Exchange code for access token
            QNetworkRequest tokenRequest(config.tokenUrl);
            tokenRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                "application/x-www-form-urlencoded");

            QUrlQuery tokenParams;
            tokenParams.addQueryItem("grant_type", "authorization_code");
            tokenParams.addQueryItem("client_id", config.clientId);
            tokenParams.addQueryItem("code", code);

            QNetworkReply* tokenReply = m_pNam->post(tokenRequest,
                tokenParams.toString(QUrl::FullyEncoded).toUtf8());

            connect(tokenReply, &QNetworkReply::finished, this,
                    [this, tokenReply, serviceId]() {
                tokenReply->deleteLater();

                if (tokenReply->error() != QNetworkReply::NoError) {
                    emit authError(serviceId,
                        QString("Token exchange failed: %1")
                            .arg(tokenReply->errorString()));
                    return;
                }

                QJsonDocument doc = QJsonDocument::fromJson(tokenReply->readAll());
                QJsonObject obj = doc.object();

                TokenPair tokens;
                tokens.accessToken = obj["access_token"].toString();
                tokens.refreshToken = obj["refresh_token"].toString();
                tokens.scope = obj["scope"].toString();

                int expiresIn = obj["expires_in"].toInt(36000);
                tokens.expiresAt = QDateTime::currentDateTime().addSecs(expiresIn);

                if (tokens.accessToken.isEmpty()) {
                    emit authError(serviceId, "Invalid response: Missing access_token");
                    return;
                }

                qInfo() << "OAuthManager: Session flow complete - Token obtained"
                        << "- Scope:" << tokens.scope;

                m_tokenCache.insert(serviceId, tokens);
                saveToKeyring(serviceId, tokens);

                emit tokenRefreshed(serviceId);
            });
        });
    });

    return QFuture<TokenPair>();
}
```

### 3. Update `BeatportService::login()`

```cpp
void BeatportService::login(const QString& username, const QString& password) {
    if (username.isEmpty() || password.isEmpty()) {
        emit loginError("Username and password are required");
        return;
    }

    m_authState = AuthState::Authorizing;
    emit authStateChanged(m_authState);

    qInfo() << "BeatportService: Initiating session cookie flow for" << username;

    // Use session cookie flow instead of password grant
    m_pOAuthManager->initiateSessionFlow(serviceId(), username, password);
}
```

---

## Why This Works

1. **Step 1 (Login):**
   - Authenticates user with Beatport
   - Returns session cookie (like logging into website)

2. **Step 2 (Authorize):**
   - Uses session to prove user is logged in
   - Beatport says "OK, you're authenticated, here's an auth code"
   - This is like clicking "Allow" on an OAuth consent screen

3. **Step 3 (Token Exchange):**
   - Exchanges the temporary auth code for long-lived tokens
   - Standard OAuth flow

**This bypasses the need for password grant** because we're using Beatport's own login endpoint + session cookies.

---

## Testing

After implementing, test:

```bash
# Start mixxx
./build/mixxx

# Login with credentials
# Email: b0id
# Password: PY5A$O8Doi0XkhW8

# Check logs for:
# "Session cookie obtained"
# "Authorization code obtained"
# "Session flow complete"
```

---

## Notes

- This is exactly what beatportdl does (proven working)
- More complex than password grant but actually works
- Client_id supports this flow
- No browser window needed (all programmatic)
