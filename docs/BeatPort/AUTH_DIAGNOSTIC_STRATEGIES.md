# Authentication Diagnostic Strategies

**Problem**: Auth required for `/stream/`, but HAR files sanitize credentials
**Goal**: Capture real auth headers and token flow from working web player

---

## ✅ Confirmed: Auth IS Required

```bash
curl https://api.beatport.com/v4/catalog/tracks/23050990/stream/
→ {"detail": "Authentication credentials were not provided."}
```

HAR files from Chrome automatically strip:
- `Authorization` headers
- `Cookie` headers
- Other sensitive data

**We need alternative methods to inspect auth.**

---

## Strategy 1: Browser DevTools Live Capture (EASIEST)

### Method: Watch requests in real-time, don't export HAR

**Steps**:
1. Open `dj.beatport.com` in Chrome
2. Open DevTools (F12) → Network tab
3. Login to Beatport
4. **Clear** the Network log (trash icon)
5. Play a track
6. Find the `/stream/` request in Network tab
7. Click on it → **Headers** tab
8. Manually copy the `Authorization` header value

**What to capture**:
```
Request Headers:
  Authorization: Bearer <COPY_THIS_TOKEN>
  Cookie: <COPY_ALL_COOKIES>
  User-Agent: <COPY_THIS>
```

**Screenshot**: Take screenshot of Headers panel if working remotely

**Pros**:
- No special tools needed
- See exactly what browser sends
- Real-time, not sanitized

**Cons**:
- Manual process
- Need to be at the machine

---

## Strategy 2: Use EditThisCookie Extension (GOOD FOR TOKENS)

### Method: Extract auth cookies directly from browser

**Steps**:
1. Install "EditThisCookie" Chrome extension
2. Go to `dj.beatport.com` and login
3. Click EditThisCookie icon
4. Look for cookies named:
   - `sessionid`
   - `access_token`
   - `bp_token`
   - Or anything authentication-related
5. Export cookies as JSON

**Alternative**: Use browser console:
```javascript
// Paste in Chrome console at dj.beatport.com
document.cookie.split(';').forEach(c => console.log(c.trim()));

// Or get localStorage tokens
Object.keys(localStorage).forEach(key => {
  console.log(key + ':', localStorage.getItem(key));
});

// Or get sessionStorage tokens
Object.keys(sessionStorage).forEach(key => {
  console.log(key + ':', sessionStorage.getItem(key));
});
```

**Pros**:
- Easy to extract tokens
- Can export and test tokens

**Cons**:
- Only shows storage, not request flow

---

## Strategy 3: Network Proxy (mitmproxy) - BEST FOR FULL FLOW

### Method: HTTPS proxy that shows all traffic, including auth

**Setup**:
```bash
# Install mitmproxy
pip install mitmproxy

# Start proxy
mitmproxy -p 8888
```

**Browser Configuration**:
1. Set browser proxy to `localhost:8888`
2. Install mitmproxy CA certificate (for HTTPS inspection)
   - Go to `http://mitm.it/` in browser
   - Download cert for your OS
   - Trust the certificate

**Usage**:
1. Start mitmproxy
2. Configure browser to use proxy
3. Go to `dj.beatport.com`
4. Login and play track
5. mitmproxy shows ALL headers (not sanitized!)

**Export flow**:
```bash
# In mitmproxy, save flows
w flows.mitm

# Or export as HAR (includes auth!)
mitmdump -r flows.mitm --save-stream-file flows.har
```

**Pros**:
- Captures EVERYTHING (cookies, headers, tokens)
- Not sanitized
- Can replay requests

**Cons**:
- Requires setup
- HTTPS certificate trust required
- More technical

---

## Strategy 4: Chrome NetLog (NUCLEAR OPTION)

### Method: Chrome's internal network logging (captures EVERYTHING)

**Steps**:
```bash
# Start Chrome with net logging
google-chrome \
  --log-net-log=/tmp/chrome-netlog.json \
  --net-log-capture-mode=Everything

# Or on existing Chrome:
# Navigate to: chrome://net-export/
# Start logging
# Reproduce issue
# Stop and save
```

**Analysis**:
```bash
# View in Chrome NetLog viewer
# Go to: https://netlog-viewer.appspot.com/
# Upload chrome-netlog.json
```

**Pros**:
- Captures EVERYTHING including TLS negotiation
- Not sanitized
- Official Chrome tool

**Cons**:
- File is HUGE
- Complex to analyze
- May contain sensitive data

---

## Strategy 5: Test With beatportdl Token (FASTEST)

### Method: Use existing working token from beatportdl

**Steps**:
```bash
# Extract token from beatportdl credentials
cd /home/b0id/repo/docs/BeatPort/beatportDL-credentials
cat *.json | jq -r '.access_token'

# Test it with our endpoint
TOKEN="<paste_from_above>"
curl -H "Authorization: Bearer $TOKEN" \
  https://api.beatport.com/v4/catalog/tracks/23050990/stream/ | jq

# If it works, you have a valid token format!
```

**Check token expiry**:
```bash
# Decode JWT token (if it's JWT format)
TOKEN="<your_token>"
echo $TOKEN | cut -d'.' -f2 | base64 -d | jq

# Look for "exp" field (expiration timestamp)
```

**Pros**:
- Already have working credentials
- Can test immediately
- Validates token format

**Cons**:
- Token might be expired
- Doesn't show HOW to get token

---

## Strategy 6: Instrument beatportdl (BEST FOR FLOW UNDERSTANDING)

### Method: Add logging to working beatportdl to see exact requests

**Steps**:
```bash
cd /home/b0id/repo/docs/BeatPort/beatportdl

# Edit internal/beatport/auth.go
# Add logging before each HTTP request
```

**Add this before HTTP requests**:
```go
log.Printf("REQUEST: %s %s", method, url)
log.Printf("HEADERS: %+v", headers)
log.Printf("BODY: %s", body)

// After response:
log.Printf("RESPONSE: %d", resp.StatusCode)
log.Printf("RESPONSE HEADERS: %+v", resp.Header)
log.Printf("RESPONSE BODY: %s", responseBody)
```

**Run with logging**:
```bash
./beatportdl -v download <track-url>
```

**Pros**:
- Shows exact working flow
- Can copy headers/format exactly
- Source of truth

**Cons**:
- Requires modifying Go code
- Rebuilding beatportdl

---

## Strategy 7: Compare Session Cookie Flow (DIAGNOSTIC)

### Method: Test session flow step-by-step with verbose output

**Test Step 1: Login**
```bash
curl -v -X POST https://api.beatport.com/v4/auth/login/ \
  -H "Content-Type: application/json" \
  -H "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36" \
  -d '{
    "username": "YOUR_USERNAME",
    "password": "YOUR_PASSWORD"
  }' \
  2>&1 | tee login_response.txt

# Look for Set-Cookie: sessionid=...
grep -i "set-cookie" login_response.txt
```

**Test Step 2: Authorize**
```bash
# Extract sessionid from Step 1
SESSION_ID="<from_step_1>"

curl -v -L "https://api.beatport.com/v4/auth/o/authorize/?client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ&response_type=code&redirect_uri=http://localhost:8889/callback" \
  -H "Cookie: sessionid=$SESSION_ID" \
  -H "User-Agent: Mozilla/5.0" \
  2>&1 | tee authorize_response.txt

# Look for redirect with code parameter
grep -i "location" authorize_response.txt
```

**Test Step 3: Token Exchange**
```bash
# Extract code from Step 2 redirect
AUTH_CODE="<from_step_2>"

curl -v -X POST https://api.beatport.com/v4/auth/o/token/ \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "grant_type=authorization_code" \
  -d "code=$AUTH_CODE" \
  -d "client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ" \
  -d "redirect_uri=http://localhost:8889/callback" \
  2>&1 | tee token_response.txt | jq
```

**Test Step 4: Use Token**
```bash
TOKEN="<from_step_3>"

curl -H "Authorization: Bearer $TOKEN" \
  https://api.beatport.com/v4/catalog/tracks/23050990/stream/ | jq
```

**Save all outputs** - each step shows where it fails

---

## Strategy 8: Add Verbose Logging to Your Implementation

### Method: Instrument OAuthManager to see what's happening

**File**: `src/streaming/hook/oauthmanager.cpp`

**Add logging**:
```cpp
// In initiateSessionFlow, add qInfo statements:

qInfo() << "=== STEP 1: LOGIN ===";
qInfo() << "URL:" << realLoginUrl.toString();
qInfo() << "Payload:" << loginData;

// After response:
qInfo() << "Response status:" << loginReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
qInfo() << "Response body:" << loginReply->readAll();
qInfo() << "Cookies:" << loginReply->header(QNetworkRequest::SetCookieHeader);
```

**Build and run**:
```bash
cd /home/b0id/repo/build
cmake --build . --target mixxx
./mixxx --verbose 2>&1 | tee auth_debug.log
```

**Analyze log**:
- Where does auth fail?
- What error messages?
- Are cookies being received?
- Are they being sent in next request?

---

## Strategy 9: Web Player Source Code Inspection

### Method: Look at web player's JavaScript to see how it authenticates

**Steps**:
1. Open `dj.beatport.com` in Chrome
2. DevTools → Sources tab
3. Search for `stream` or `auth` in JS files
4. Look for authentication code

**Or download JS**:
```bash
# Download main JS bundle
curl https://dj.beatport.com/main.js > beatport_web.js

# Search for auth logic
grep -i "authorization\|bearer\|access_token" beatport_web.js

# Search for API calls
grep -i "/stream/\|/auth/" beatport_web.js
```

**Look for**:
- Token storage method (localStorage, sessionStorage, cookies)
- How token is attached to requests
- Token format

---

## Strategy 10: Use Working Token to Test Implementation

### Method: Bypass auth implementation, test streaming with known-good token

**Steps**:
1. Get working token (from beatportdl or browser)
2. Manually set it in keyring:
```bash
# Store token manually
secret-tool store --label='Beatport Token' service beatport << EOF
<PASTE_TOKEN_HERE>
EOF
```

3. Modify code to skip auth:
```cpp
// In BeatportService::isAuthenticated()
bool BeatportService::isAuthenticated() const {
    return true; // TEMPORARY: Force authenticated
}

// In BeatportService::getStreamInfo()
QString accessToken = m_pOAuthManager->getAccessToken(serviceId());
// Override with manual token
accessToken = "PASTE_WORKING_TOKEN_HERE";
```

4. Test streaming with known-good auth

**Pros**:
- Isolates auth from streaming issues
- Can test streaming independently
- Proves streaming code works

**Cons**:
- Temporary workaround
- Still need to fix auth later

---

## Recommended Strategy for Remote Work

### Phase 1: Get Working Token (10 min)

**Option A**: Extract from beatportdl
```bash
cd /home/b0id/repo/docs/BeatPort/beatportDL-credentials
cat *.json | jq -r '.access_token'
```

**Option B**: Browser DevTools
1. Login to dj.beatport.com
2. F12 → Application → Local Storage
3. Look for token keys
4. Copy token value

### Phase 2: Test Token (5 min)

```bash
TOKEN="<from_phase_1>"

# Test if token works
curl -H "Authorization: Bearer $TOKEN" \
  https://api.beatport.com/v4/catalog/tracks/23050990/stream/ | jq

# If works: Token is valid
# If 401: Token expired, need new one
```

### Phase 3: Hardcode Token for Testing (5 min)

```cpp
// In beatportservice.cpp, line ~340
QString accessToken = "PASTE_WORKING_TOKEN_HERE"; // TEMP: Hardcoded token
request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());
```

### Phase 4: Apply Streaming Fixes (10 min)

Follow `EXACT_CODE_FIXES.md`:
- Fix endpoint
- Fix JSON parsing
- Fix HLS segment extraction

### Phase 5: Build and Test (5 min)

```bash
cmake --build . --target mixxx
./mixxx
# Should stream with hardcoded token
```

### Phase 6: Fix Auth Flow (Later)

Once streaming works with hardcoded token, debug auth:
- Use mitmproxy OR
- Instrument beatportdl OR
- Manual curl testing

---

## Decision Matrix

| Strategy | Setup Time | Accuracy | Remote Friendly |
|----------|-----------|----------|-----------------|
| DevTools Live | 0 min | ⭐⭐⭐⭐⭐ | ❌ Need local access |
| EditThisCookie | 2 min | ⭐⭐⭐⭐ | ✅ Can screenshot |
| mitmproxy | 15 min | ⭐⭐⭐⭐⭐ | ❌ Complex setup |
| Chrome NetLog | 5 min | ⭐⭐⭐⭐⭐ | ✅ Can upload file |
| beatportdl Token | 1 min | ⭐⭐⭐⭐ | ✅ Files exist |
| Instrument beatportdl | 20 min | ⭐⭐⭐⭐⭐ | ✅ Can log to file |
| Manual curl Test | 10 min | ⭐⭐⭐⭐ | ✅ Easy to script |
| Code Logging | 5 min | ⭐⭐⭐ | ✅ Log files |
| Web Source | 10 min | ⭐⭐⭐ | ✅ Can download |
| Hardcode Token | 2 min | ⭐⭐⭐ | ✅ Quick test |

---

## Immediate Action Plan for Remote Work

```bash
# 1. Extract beatportdl token (30 seconds)
cd /home/b0id/repo/docs/BeatPort/beatportDL-credentials
cat *.json | jq

# 2. Test token (30 seconds)
TOKEN="<paste_access_token>"
curl -H "Authorization: Bearer $TOKEN" \
  https://api.beatport.com/v4/catalog/tracks/23050990/stream/ | jq

# 3. If token works: Use it to test streaming fixes
# 4. If token expired: Use manual curl auth testing (Strategy 7)
# 5. Document results and commit
```

---

## Bottom Line

**Immediate**: Use beatportdl token to test streaming (Strategy 5 + 10)
**Short-term**: Browser DevTools to extract fresh token (Strategy 1)
**Long-term**: mitmproxy or instrumented beatportdl for full flow (Strategy 3 or 6)

Start with extracting and testing beatportdl token - that's the fastest path.
