# Next Steps After HAR Analysis

**Date**: 2026-01-23
**Context**: Session cookie auth flow implemented, HAR analyzed, ready for testing

---

## TL;DR

✅ Your implementation is correct (matches beatportdl)
✅ Web player uses different approach (PKCE) - that's fine
❌ HAR missing streaming endpoint (user didn't play track)
⏭️ **Next: Test auth end-to-end**

---

## Immediate Action Items

### 1. Test Authentication (Priority 1)

**Goal**: Verify session cookie flow works

**Steps**:
```bash
# 1. Run Mixxx
/home/b0id/repo/build/mixxx

# 2. Open Preferences → Streaming → Beatport
# 3. Click Login
# 4. Enter credentials from beatportDL-credentials.json:
#    Username: <your_beatport_username>
#    Password: <your_beatport_password>
# 5. Watch console for auth flow messages
```

**Expected Console Output**:
```
BeatportService: Initiating session cookie flow for <username>
OAuthManager: Step 1 - Logging in to get session cookie...
OAuthManager: Step 2 - Authorizing with session cookie...
OAuthManager: Step 3 - Exchanging code for token...
OAuthManager: Session flow complete, token expires: <timestamp>
```

**Verify Token Saved**:
```bash
# Check if token was stored in keyring
secret-tool search service beatport

# Should show:
# attribute.service = beatport
# secret = <access_token>
```

**If auth fails**:
- Check error message in console
- Compare to `SessionCookieAuthFlow.md`
- Check network logs for 401/403 responses

---

### 2. Test Catalog API (Priority 2)

**Goal**: Verify token works for API calls

**Steps**:
```bash
# Extract token from keyring
TOKEN=$(secret-tool lookup service beatport)

# Test user profile endpoint
curl -H "Authorization: Bearer $TOKEN" \
  https://api.beatport.com/v4/my/beatport/ | jq

# Expected: User profile JSON with username, subscription
```

**Test search**:
```bash
curl -H "Authorization: Bearer $TOKEN" \
  "https://api.beatport.com/v4/catalog/search/?q=daft+punk&type=tracks" | jq
```

**Test track metadata**:
```bash
# Use a known track ID (from HAR: 23278801)
curl -H "Authorization: Bearer $TOKEN" \
  https://api.beatport.com/v4/catalog/tracks/23278801/ | jq
```

---

### 3. Find Streaming Endpoint (Priority 3)

**Option A: Test /download endpoint (from beatportService.cpp)**

Your code currently uses:
```cpp
QUrl url(QString("%1/catalog/tracks/%2/download").arg(m_config.baseUrl, trackId));
urlQuery.addQueryItem("quality", "256-aac");
```

Test it:
```bash
TOKEN=$(secret-tool lookup service beatport)
TRACK_ID=23278801

curl -v -H "Authorization: Bearer $TOKEN" \
  "https://api.beatport.com/v4/catalog/tracks/$TRACK_ID/download?quality=256-aac"
```

**Expected outcomes**:
- 200 + JSON with stream URL → Success! Use this URL
- 302 Redirect → Follow redirect to CDN URL
- 403 Forbidden → Need different endpoint or subscription issue
- 404 Not Found → Wrong endpoint, need /stream/ or /stream-sdk

**Option B: Get new HAR with actual playback**

1. Go to https://dj.beatport.com
2. Open DevTools → Network tab
3. **Play a track** (critical!)
4. Filter for "media" or "audio" in Network tab
5. Export HAR
6. Look for CDN URLs or streaming endpoint calls

**Option C: Use beatportdl to capture stream URL**

```bash
# Run beatportdl in verbose mode
cd BeatPort/beatportdl
./beatportdl -v download <track-url>

# Watch console for download URL
# Grep source for exact endpoint used
grep -r "download" internal/beatport/*.go
```

---

### 4. Test Streaming (Priority 4)

**Once you have a stream URL**:

```bash
# Test if URL returns audio
curl -H "Authorization: Bearer $TOKEN" \
  "<stream_url_from_step_3>" | file -

# Expected: "Audio file with ID3 version 2.4.0" or similar
```

**Test in Mixxx**:
1. Create a `.knt` manifest with the track:
   ```json
   {
     "trackId": "beatport:23278801",
     "title": "Test Track",
     "artist": "Test Artist",
     "duration": 360,
     "sampleRate": 44100,
     "bitrate": 256,
     "codec": "aac",
     "streamUrl": "https://api.beatport.com/v4/catalog/tracks/23278801/download?quality=256-aac"
   }
   ```
2. Place in `~/.local/share/mixxx/test_beatport.knt`
3. Load in Mixxx
4. Check console for FUSE mount and streaming activity

---

## What We Learned from HAR

### Web Player Auth (PKCE)
```
POST /api/custom/bp/v2/auth-v2.php
  → Initialize session

[Browser OAuth flow]

POST /api/custom/bp/v2/auth-code-v2.php
  code=<auth_code>
  code_verifier=<pkce_verifier>
  → Get access token (via proxy)
```

### Your Implementation (Session Cookie)
```
POST /auth/login/
  → Get session cookie

GET /auth/o/authorize/ (with cookie)
  → Get auth code from redirect

POST /auth/o/token/
  code=<auth_code>
  → Get access token (direct)
```

**Both are valid!** Web uses PKCE + proxy (browser security).
You use session cookie (mimicking beatportdl - proven working).

---

## Troubleshooting Guide

### Auth fails with "unauthorized_client"
- **Cause**: Wrong grant type for client_id
- **Fix**: Already fixed (using session flow, not password grant)

### Auth fails with "invalid_credentials"
- **Cause**: Wrong username/password
- **Fix**: Verify credentials work on beatport.com

### API returns 401 after successful auth
- **Cause**: Token not being sent or malformed
- **Fix**: Check `Authorization: Bearer <token>` header format

### API returns 403 Forbidden
- **Cause**: Subscription doesn't allow this endpoint
- **Fix**: Check subscription tier via `/my/account/`

### Streaming endpoint returns 404
- **Cause**: Wrong endpoint path
- **Fix**: Try `/stream/` or `/stream-sdk` instead of `/download`

### Stream URL returns 403 after working
- **Cause**: URL expired (time-limited signed URLs)
- **Fix**: Refresh flow already implemented in FuseDriver

---

## Success Criteria

### Phase 1: Auth Working ✅
- [ ] User can login via Mixxx UI
- [ ] Token saved to keyring
- [ ] Token works for `/my/beatport/` API call

### Phase 2: Catalog Working ✅
- [ ] Search returns results
- [ ] Track metadata retrieval works
- [ ] Genres/playlists load

### Phase 3: Streaming Working 🎯
- [ ] Stream URL can be fetched
- [ ] URL returns audio data
- [ ] Mixxx can load and play .knt manifest
- [ ] Audio plays through speakers

---

## Files to Monitor

### During Auth Testing
- `src/streaming/hook/oauthmanager.cpp` - Watch for qInfo/qWarning logs
- `src/streaming/hook/beatportservice.cpp` - Login flow
- `~/.config/mixxx/mixxx.log` - Full application logs

### During Streaming Testing
- `src/streaming/bridge/fusedriver.cpp` - FUSE operations
- `src/streaming/bridge/rangefetcher.cpp` - HTTP streaming
- `src/sources/soundsourcekineticproxy.cpp` - Manifest loading

---

## Additional Resources

- `SessionCookieAuthFlow.md` - Implementation reference
- `HAR_Analysis_Report.md` - Detailed HAR findings
- `Beatport_Integration_Strategy.md` - Complete API spec
- `beatportdl/internal/beatport/auth.go` - Reference implementation

---

## Contact Points

If stuck:
1. Check console logs for specific error messages
2. Test API endpoints manually with curl
3. Compare to beatportdl behavior (proven working)
4. Capture new HAR with streaming if needed
