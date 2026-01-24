# Code Cleanup Checklist

**Date:** 2026-01-23
**Context:** Post-Tidal pivot cleanup and Beatport fix prioritization

---

## Priority 1: Critical Fixes (Beatport Auth)

### 1.1 Wrong Client ID in streamingconfig.cpp

**Location:** `src/streaming/streamingconfig.cpp:70`

**Current (BROKEN):**
```cpp
config.clientId = "0GIvkCltVIuPkkwSJHp6NDb3s0potTjLBQr388Dd";
```

**Fix:**
```cpp
config.clientId = "ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ";
```

**Rationale:** The Swagger docs client_id only works with browser post-message redirects. The beatportdl client_id supports password grant.

---

### 1.2 Add Password Grant to OAuthManager

**Location:** `src/streaming/hook/oauthmanager.h` and `oauthmanager.cpp`

**Add method:**
```cpp
QFuture<TokenPair> initiatePasswordGrant(
    const QString& serviceId,
    const QString& username,
    const QString& password);
```

See `Beatport_Integration_Strategy.md` for full implementation.

---

### 1.3 Update BeatportService Auth Flow

**Location:** `src/streaming/hook/beatportservice.cpp`

**Current:** Uses `initiateBrowserFlow()` which opens browser for PKCE
**Needed:** Option for `initiatePasswordGrant()` with username/password

---

## Priority 2: UI Updates

### 2.1 Login Dialog Credentials Page

**Location:** `src/streaming/ui/logindialog.cpp`

**Add:** Username/password entry page for Beatport (instead of device code)

---

## Priority 3: Tidal Deferral

### 3.1 Disable Tidal for Now

**Recommendation:** Comment out or disable TidalService registration until Beatport is working.

**Known Issues:**
1. API base URL mismatch (v2 vs v1 for playback endpoints)
2. Manifest parsing expects array, but Tidal returns base64 string
3. Missing client_id configuration (current one may be stale)

### 3.2 Files to Review/Disable

- `src/streaming/hook/tidalservice.cpp`
- `src/streaming/hook/tidalfeature.cpp`
- Registration in `coreservices.cpp` (if applicable)

---

## Priority 4: General Cleanup

### 4.1 Remove Debug/Troubleshooting Code

Search for and review:
- Hardcoded test values
- Excessive logging
- Temporary workarounds

### 4.2 TODO Comments to Address

From grep results:
```
beatportservice.cpp:68:    // TODO: Implement token clearing in OAuthManager
beatportservice.cpp:215:   // TODO: Signal or return through QPromise
beatportservice.cpp:292:   // TODO: Implement playlist fetching
beatportservice.cpp:446:   // TODO: Download and cache cover art
beatportservice.cpp:455:   // TODO: Implement proper key normalization
oauthmanager.cpp:417:      // TODO: Add state parameter
```

---

## Verification Checklist

After fixes, verify:

- [ ] Can obtain access token with correct client_id
- [ ] Token refresh works
- [ ] Search API returns results
- [ ] Stream URL endpoint returns valid URL
- [ ] Token interceptor retries 401s correctly
- [ ] Keyring storage persists tokens across restarts

---

## Testing Commands

```bash
# Test Beatport auth with curl
curl -X POST https://api.beatport.com/v4/auth/o/token/ \
  -d "client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ" \
  -d "grant_type=password" \
  -d "username=YOUR_EMAIL" \
  -d "password=YOUR_PASSWORD"

# Test search with token
curl "https://api.beatport.com/v4/catalog/search/?q=test" \
  -H "Authorization: Bearer YOUR_ACCESS_TOKEN"

# Test stream endpoint
curl "https://api.beatport.com/v4/catalog/tracks/12345678/stream/" \
  -H "Authorization: Bearer YOUR_ACCESS_TOKEN"
```

---

## Files Modified Summary

| File | Status | Notes |
|------|--------|-------|
| `streamingconfig.cpp` | NEEDS FIX | Wrong client_id |
| `oauthmanager.cpp` | NEEDS ADDITION | Password grant method |
| `oauthmanager.h` | NEEDS ADDITION | Method declaration |
| `beatportservice.cpp` | NEEDS UPDATE | Use password grant |
| `logindialog.cpp` | NEEDS UPDATE | Credentials page |
| `tidalservice.cpp` | DEFER | Disable for now |
| `tokeninterceptor.cpp` | OK | Looks correct |
| `beatportfeature.cpp` | OK | Basic structure good |
