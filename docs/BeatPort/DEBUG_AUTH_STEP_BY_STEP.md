# Debug Authentication - Step by Step

**Goal**: Identify exactly where auth is failing and fix it

---

## TEST 1: Can We Stream Without Auth? (CRITICAL TEST)

```bash
# Test if /stream/ endpoint is public (no auth needed)
curl -s https://api.beatport.com/v4/catalog/tracks/23050990/stream/ | jq

# Expected outcome A: 200 OK with JSON
# {
#   "stream_url": "https://needledrop.beatport.com/...",
#   "sample_start_ms": 0,
#   "sample_end_ms": 256463
# }
# ✅ MEANS: Preview streaming is PUBLIC - skip all auth!

# Expected outcome B: 401 Unauthorized
# ❌ MEANS: Auth required - must fix auth flow
```

**If TEST 1 returns 200**: Skip to STREAMING FIX section
**If TEST 1 returns 401**: Continue with auth debugging

---

## TEST 2: Session Cookie Flow (Manual Test)

### Step 2A: Login to Get Session Cookie

```bash
# Test the /login/ endpoint
curl -v -X POST https://api.beatport.com/v4/auth/login/ \
  -H "Content-Type: application/json" \
  -H "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36" \
  -d '{
    "username": "YOUR_USERNAME",
    "password": "YOUR_PASSWORD"
  }' \
  2>&1 | grep -i "set-cookie\|HTTP\|error"

# Look for:
# < HTTP/2 200
# < set-cookie: sessionid=<value>

# If 200 + sessionid: ✅ Login works
# If 401: ❌ Wrong credentials
# If 404: ❌ Wrong endpoint
# If 403: ❌ WAF/bot detection
```

### Step 2B: Use Session Cookie to Get Auth Code

```bash
# Extract sessionid from Step 2A
SESSION_ID="<from_step_2a>"

# Request authorization using session cookie
curl -v -L "https://api.beatport.com/v4/auth/o/authorize/?client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ&response_type=code&redirect_uri=http://localhost:8889/callback" \
  -H "Cookie: sessionid=$SESSION_ID" \
  -H "User-Agent: Mozilla/5.0" \
  2>&1 | grep -i "location\|HTTP\|code="

# Look for redirect with code:
# < Location: http://localhost:8889/callback?code=<AUTH_CODE>

# If redirect: ✅ Extract AUTH_CODE
# If 401/403: ❌ Session invalid or client_id wrong
```

### Step 2C: Exchange Code for Token

```bash
# Extract code from Step 2B
AUTH_CODE="<from_step_2b>"

curl -X POST https://api.beatport.com/v4/auth/o/token/ \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "grant_type=authorization_code" \
  -d "code=$AUTH_CODE" \
  -d "client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ" \
  -d "redirect_uri=http://localhost:8889/callback" | jq

# Expected:
# {
#   "access_token": "...",
#   "refresh_token": "...",
#   "expires_in": 36000,
#   "scope": "app:locker user:dj"
# }

# If success: ✅ Auth flow works!
# If error: Check error message
```

### Step 2D: Test Token

```bash
# Use token from Step 2C
TOKEN="<access_token>"

curl -H "Authorization: Bearer $TOKEN" \
  https://api.beatport.com/v4/my/beatport/ | jq

# Expected: User profile JSON
# If 401: Token invalid
```

---

## TEST 3: Alternative - PKCE Flow (If Session Fails)

The web player uses PKCE, not session flow. If session flow fails, try PKCE:

### Step 3A: Generate PKCE Verifier

```bash
# Generate code verifier (random 43-128 char string)
CODE_VERIFIER=$(openssl rand -base64 32 | tr -d "=+/" | cut -c1-43)
echo "Verifier: $CODE_VERIFIER"

# Generate code challenge
CODE_CHALLENGE=$(echo -n $CODE_VERIFIER | openssl dgst -sha256 -binary | base64 | tr -d "=+/" | tr '/+' '_-')
echo "Challenge: $CODE_CHALLENGE"
```

### Step 3B: Get Authorization (Browser Required)

```bash
# Open this URL in browser:
cat << EOF
https://api.beatport.com/v4/auth/o/authorize/?client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ&response_type=code&redirect_uri=http://localhost:8889/callback&code_challenge=$CODE_CHALLENGE&code_challenge_method=S256
EOF

# User logs in via browser
# Browser redirects to: http://localhost:8889/callback?code=<AUTH_CODE>
# Extract code from URL
```

### Step 3C: Exchange Code (with PKCE verifier)

```bash
AUTH_CODE="<from_browser_redirect>"

curl -X POST https://api.beatport.com/v4/auth/o/token/ \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "grant_type=authorization_code" \
  -d "code=$AUTH_CODE" \
  -d "client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ" \
  -d "redirect_uri=http://localhost:8889/callback" \
  -d "code_verifier=$CODE_VERIFIER" | jq
```

---

## TEST 4: Use beatportdl Token

If you have working beatportdl credentials:

```bash
# Extract token from beatportdl config
cd /home/b0id/repo/docs/BeatPort/beatportDL-credentials
cat *.json | jq -r '.access_token'

# Test it
TOKEN="<from_above>"
curl -H "Authorization: Bearer $TOKEN" \
  https://api.beatport.com/v4/my/beatport/ | jq

# If works: Token is valid, use it to test streaming
```

---

## STREAMING FIX (Once You Have Token OR if TEST 1 Showed Public Access)

### Fix 1: Update Endpoint

**File**: `src/streaming/hook/beatportservice.cpp`
**Line**: 326

Change:
```cpp
// OLD:
QUrl url(QString("%1/catalog/tracks/%2/download").arg(m_config.baseUrl, trackId));
QUrlQuery urlQuery;
urlQuery.addQueryItem("quality", "256-aac");
url.setQuery(urlQuery);

// NEW:
QUrl url(QString("%1/catalog/tracks/%2/stream/").arg(m_config.baseUrl, trackId));
// No query parameters needed
```

### Fix 2: Parse JSON Response

**File**: `src/streaming/hook/beatportservice.cpp`
**Lines**: 347-390

Change parsing logic:
```cpp
connect(reply, &QNetworkReply::finished, this, [this, reply, trackId]() {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        // [existing error handling]
        return;
    }

    // Parse JSON response
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        emit streamInfoFailed(trackId,
            mixxx::kinetic::StreamingError::parseError("Invalid JSON response"));
        return;
    }

    QJsonObject obj = doc.object();
    QString hlsUrl = obj["stream_url"].toString();

    if (hlsUrl.isEmpty()) {
        emit streamInfoFailed(trackId,
            mixxx::kinetic::StreamingError::parseError("No stream_url in response"));
        return;
    }

    // WORKAROUND: Convert M3U8 to first segment for testing
    // HLS URL: https://needledrop.beatport.com/.../file.128k.aac.m3u8
    // Segment:  https://needledrop.beatport.com/.../file.128k.aac-1.aac
    QString segmentUrl = hlsUrl;
    segmentUrl.replace(".m3u8", "-1.aac");

    qInfo() << "Stream URL (segment 1):" << segmentUrl;

    StreamInfo info;
    info.streamUrl = segmentUrl;  // Use first segment for testing
    info.format = "aac";
    info.bitrate = 128;
    info.sampleRate = 44100;
    info.channels = 2;

    emit streamInfoReady(trackId, info);
});
```

### Fix 3: Test Streaming

```bash
# Rebuild
cd /home/b0id/repo/build
cmake --build . --target mixxx

# Run
./mixxx

# Try loading a Beatport track
```

---

## EXPECTED OUTCOMES

### If /stream/ is public (TEST 1 = 200):
1. No auth needed
2. Just fix endpoint + parsing
3. Should stream immediately

### If /stream/ needs auth (TEST 1 = 401):
1. Fix auth flow (TEST 2 or TEST 3)
2. Then fix endpoint + parsing
3. Should stream with token

---

## CURRENT BUGS SUMMARY

1. ❌ **Wrong endpoint**: Using `/download` instead of `/stream/`
2. ❌ **Wrong parsing**: Not extracting `stream_url` from JSON
3. ❌ **HLS not handled**: Need to use segments, not M3U8 manifest
4. ⚠️ **Auth unknown**: May not be needed for preview streaming

---

## QUICK WIN PATH

1. Run TEST 1 (check if auth needed)
2. If not needed: Skip auth, go straight to streaming fixes
3. Apply streaming fixes (10 minutes)
4. Test with first segment
5. Should get ~3 seconds of audio playing

---

## FULL SOLUTION PATH

After quick win works:
1. Implement M3U8 parser
2. Fetch all segments
3. Concatenate in FUSE driver
4. Full track streaming

---

## WHAT TO DO RIGHT NOW

```bash
# 1. Test if streaming needs auth
curl https://api.beatport.com/v4/catalog/tracks/23050990/stream/ | jq

# 2. Based on result, either:
#    - Fix streaming (if public)
#    - Debug auth (if 401)

# Report back what you see!
```
