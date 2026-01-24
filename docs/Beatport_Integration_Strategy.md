# Beatport Streaming Integration Strategy

**Generated:** 2026-01-23
**Confidence Level:** HIGH (based on working beatportdl implementation)

---

## Executive Summary

The current mixxx-kinetic Beatport implementation has several critical misconfigurations preventing successful authentication. The working beatportdl reference implementation reveals the correct API patterns.

### Core Issues Identified

| Issue | Current (Broken) | Required (Working) |
|-------|------------------|-------------------|
| **Client ID** | `0GIvkCltVIuPkkwSJHp6NDb3s0potTjLBQr388Dd` | `ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ` |
| **Auth Flow** | PKCE Browser Flow | Password Grant or Session Cookie Flow |
| **Redirect URI** | `post-message` (uninterceptable) | Not needed for password grant |
| **Scope** | Empty | `app:locker user:dj` (implicit) |
| **Token URL** | `/v4/auth/o/token/` | `/auth/o/token/` (no v4 prefix in path) |

---

## Part 1: Beatport API Specification (High Confidence)

### 1.1 Authentication Endpoints

| Endpoint | Method | Purpose |
|----------|--------|---------|
| `https://api.beatport.com/v4/auth/o/token/` | POST | Token exchange/refresh |
| `https://api.beatport.com/v4/auth/o/authorize/` | GET | Authorization (session cookie flow) |
| `https://api.beatport.com/v4/auth/login/` | POST | Session login |

### 1.2 Recommended Auth Flow: Password Grant

This is the simplest approach, used successfully by beatportdl:

```
POST https://api.beatport.com/v4/auth/o/token/
Content-Type: application/x-www-form-urlencoded

client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ
grant_type=password
username={user_email}
password={user_password}
```

**Response:**
```json
{
  "access_token": "...",
  "refresh_token": "...",
  "expires_in": 36000,
  "token_type": "Bearer",
  "scope": "app:locker user:dj"
}
```

**Key Details:**
- Access token lifetime: 36000 seconds (~10 hours)
- Refresh token lifetime: 31 days
- Refresh tokens are single-use (rotation required)

### 1.3 Token Refresh

```
POST https://api.beatport.com/v4/auth/o/token/
Content-Type: application/x-www-form-urlencoded

client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ
grant_type=refresh_token
refresh_token={refresh_token}
```

### 1.4 Catalog API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/catalog/tracks/{id}/` | GET | Track metadata (BPM, key, artists, etc.) |
| `/catalog/tracks/{id}/stream/` | GET | **Streaming URL** |
| `/catalog/tracks/{id}/download/?quality={quality}` | GET | Download URL |
| `/catalog/search/?q={query}` | GET | Search catalog |
| `/catalog/releases/{id}/` | GET | Release info |

### 1.5 Stream Response Format

```json
{
  "stream_url": "https://cdn.beatport.com/...",
  "sample_start_ms": 0,
  "sample_end_ms": 0
}
```

The `stream_url` typically points to an AAC stream:
- Basic: 128kbps AAC
- Premium: 256kbps AAC
- Quality may depend on subscription tier

### 1.6 Required Headers

```
Authorization: Bearer {access_token}
Accept: application/json
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36...
```

---

## Part 2: Implementation Fixes Required

### 2.1 streamingconfig.cpp Changes

```cpp
StreamingConfig::ServiceConfig StreamingConfig::getDefaultBeatportConfig() {
    ServiceConfig config;
    config.baseUrl = "https://api.beatport.com/v4";
    config.authUrl = "https://api.beatport.com/v4/auth/o/authorize/";
    config.tokenUrl = "https://api.beatport.com/v4/auth/o/token/";

    // CRITICAL FIX: Use the working client ID from beatportdl
    config.clientId = "ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ";
    config.clientSecret = ""; // Not required for password grant

    // No explicit scope needed - server assigns based on client_id
    config.scope = "";

    // Not needed for password grant, but kept for potential PKCE fallback
    config.redirectUri = "http://localhost:8889/callback";
    return config;
}
```

### 2.2 New OAuthManager Method: Password Grant

Add to `oauthmanager.h`:
```cpp
QFuture<TokenPair> initiatePasswordGrant(
    const QString& serviceId,
    const QString& username,
    const QString& password);
```

Add to `oauthmanager.cpp`:
```cpp
QFuture<OAuthManager::TokenPair> OAuthManager::initiatePasswordGrant(
        const QString& serviceId,
        const QString& username,
        const QString& password) {
    if (!m_services.contains(serviceId)) {
        emit authError(serviceId, "Service not registered");
        return QFuture<TokenPair>();
    }

    const auto& config = m_services[serviceId];

    QNetworkRequest request(config.tokenUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("grant_type", "password");
    params.addQueryItem("client_id", config.clientId);
    params.addQueryItem("username", username);
    params.addQueryItem("password", password);

    QNetworkReply* reply = m_pNam->post(request,
        params.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, [this, reply, serviceId]() {
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
        int expiresIn = obj["expires_in"].toInt(36000);
        tokens.expiresAt = QDateTime::currentDateTime().addSecs(expiresIn);
        tokens.scope = obj["scope"].toString();

        if (tokens.accessToken.isEmpty()) {
            emit authError(serviceId, "Invalid response: Missing access_token");
            return;
        }

        m_tokenCache.insert(serviceId, tokens);
        saveToKeyring(serviceId, tokens);
        emit tokenRefreshed(serviceId);
    });

    return QFuture<TokenPair>();
}
```

### 2.3 BeatportService Changes

Update `initiateLogin()` to use password grant:

```cpp
void BeatportService::initiateLogin() {
    m_authState = AuthState::Authorizing;
    emit authStateChanged(m_authState);

    // Emit signal requesting credentials from UI
    emit credentialsRequired();
}

void BeatportService::login(const QString& username, const QString& password) {
    m_pOAuthManager->initiatePasswordGrant(serviceId(), username, password);
}
```

### 2.4 Login Dialog Update

Add username/password page for Beatport:

```cpp
// In setupUi(), add a credentials page:
m_pageCredentials = new QWidget(this);
auto* layoutCreds = new QVBoxLayout(m_pageCredentials);
layoutCreds->addWidget(new QLabel("Beatport Login", m_pageCredentials));

m_txtUsername = new QLineEdit(m_pageCredentials);
m_txtUsername->setPlaceholderText("Email");
layoutCreds->addWidget(m_txtUsername);

m_txtPassword = new QLineEdit(m_pageCredentials);
m_txtPassword->setPlaceholderText("Password");
m_txtPassword->setEchoMode(QLineEdit::Password);
layoutCreds->addWidget(m_txtPassword);

auto* btnLogin = new QPushButton("Login", m_pageCredentials);
connect(btnLogin, &QPushButton::clicked, this, [this]() {
    m_service->login(m_txtUsername->text(), m_txtPassword->text());
    m_stack->setCurrentWidget(m_pageCode);  // Show "waiting..."
});
layoutCreds->addWidget(btnLogin);
m_stack->addWidget(m_pageCredentials);
```

---

## Part 3: Tidal Cleanup Notes

The Tidal implementation added last night has structural issues:

### 3.1 API Version Mismatch

```cpp
// CURRENT (potentially wrong):
config.baseUrl = "https://openapi.tidal.com/v2";

// The playbackinfo endpoint is v1:
// https://api.tidal.com/v1/tracks/{id}/playbackinfopostpaywall
```

### 3.2 Manifest Parsing Issue

Tidal returns base64-encoded manifest data, not a JSON array:

```cpp
// CURRENT (wrong):
QJsonArray manifestsArray = obj["manifest"].toArray();

// CORRECT: manifest is a base64-encoded string
QString manifestBase64 = obj["manifest"].toString();
QByteArray manifestData = QByteArray::fromBase64(manifestBase64.toLatin1());
```

### 3.3 Recommendation

Defer Tidal implementation until Beatport is working. Focus on one service.

---

## Part 4: Testing Checklist

### 4.1 Authentication Test

1. Update `streamingconfig.cpp` with correct client_id
2. Add password grant to OAuthManager
3. Test login with known-good credentials:
   - Username: `b0id`
   - Expected scope in response: `app:locker user:dj`

### 4.2 API Test Sequence

```bash
# 1. Get token (test with curl first)
curl -X POST https://api.beatport.com/v4/auth/o/token/ \
  -d "client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ" \
  -d "grant_type=password" \
  -d "username=YOUR_EMAIL" \
  -d "password=YOUR_PASSWORD"

# 2. Search (verify auth works)
curl https://api.beatport.com/v4/catalog/search/?q=test \
  -H "Authorization: Bearer ACCESS_TOKEN"

# 3. Get stream URL
curl https://api.beatport.com/v4/catalog/tracks/12345678/stream/ \
  -H "Authorization: Bearer ACCESS_TOKEN"
```

### 4.3 Integration Test

1. Launch Mixxx-Kinetic
2. Open streaming browser
3. Click Login for Beatport
4. Enter credentials
5. Search for a track
6. Attempt to load track → verify stream URL is obtained

---

## Part 5: Security Considerations

### 5.1 Credential Storage

- **Never** store plain-text passwords
- Use QtKeychain for token storage (already implemented)
- Consider prompting for password each session vs storing

### 5.2 Client ID Considerations

The `ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ` client_id is:
- Embedded in the beatportdl open-source project
- Allows password grant flow
- May be rate-limited or revoked if abused

Consider:
- Making client_id configurable via streaming.json
- Documenting that users can provide their own client_id if they have partner access

---

## Part 6: File Changes Summary

| File | Change Type | Priority |
|------|-------------|----------|
| `streamingconfig.cpp` | Fix client_id | **CRITICAL** |
| `oauthmanager.h` | Add password grant method | **CRITICAL** |
| `oauthmanager.cpp` | Implement password grant | **CRITICAL** |
| `beatportservice.cpp` | Update auth flow | **HIGH** |
| `logindialog.cpp` | Add credentials UI | **HIGH** |
| `tidalservice.cpp` | Defer/disable | MEDIUM |

---

## Appendix A: Reference Implementations

### beatportdl (Go) - Known Working
- Repository: `docs/BeatPort/beatportdl/`
- Key files:
  - `internal/beatport/auth.go` - Authentication logic
  - `internal/beatport/tracks.go` - Track/stream endpoints
  - `internal/beatport/beatport.go` - HTTP client setup

### beatport-mcp (Python) - Reference
- Repository: `docs/BeatPort/beatport-mcp/`
- Key file: `auth.py` - Simple password grant implementation

### Working Credentials (for testing)
- Location: `docs/BeatPort/BeatportDL-credentials/`
- Contains valid tokens with scope `app:locker user:dj`
- Token issued at timestamp indicates recent successful auth

---

## Appendix B: Error Codes

| HTTP Code | Meaning | Action |
|-----------|---------|--------|
| 401 | Token expired/invalid | Refresh token or re-authenticate |
| 403 | Subscription insufficient | Show subscription upgrade prompt |
| 404 | Track not found | Handle gracefully in UI |
| 429 | Rate limited | Implement backoff |

---

## Conclusion

The path forward is clear:

1. **Immediate**: Change client_id in streamingconfig.cpp
2. **Short-term**: Implement password grant in OAuthManager
3. **Medium-term**: Update LoginDialog for credential entry
4. **Deferred**: Tidal integration (requires separate investigation)

The beatportdl implementation proves the API works. The fixes are configuration and auth flow changes, not architectural.
