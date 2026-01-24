# Beatport API Consolidated Reference

**Last Updated**: 2026-01-24
**Status**: VERIFIED AND READY FOR IMPLEMENTATION
**Confidence**: HIGH - All endpoints tested with live API

---

## AGENT INSTRUCTIONS

When implementing Beatport integration, use this document as your primary API reference.
All endpoints below have been **verified working** with real API calls on 2026-01-24.

**Key files to modify:**
- `src/streaming/hook/beatportservice.cpp` - Main service implementation
- `src/streaming/hook/oauthmanager.cpp` - Token management

---

## Quick Reference

| Component | Value | Verified |
|-----------|-------|----------|
| Base URL | `https://api.beatport.com/v4` | YES |
| Client ID | `ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ` | YES |
| Token Lifetime | 36,000 seconds (10 hours) | YES |
| Refresh Token TTL | ~31 days | YES |
| Stream Format | HLS (M3U8 + AES-128 encrypted segments) | YES |
| Stream Quality | 128 kbps AAC | YES |

---

## 1. Authentication (VERIFIED WORKING)

### 1.1 Session Cookie Flow - USE THIS

```
Step 1: POST /auth/login/
        Content-Type: application/json
        Body: {"username": "...", "password": "..."}
        Response: Sets 'sessionid' cookie

Step 2: GET /auth/o/authorize/?client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ&response_type=code
        Cookie: sessionid={from_step_1}
        Response: 302 redirect, Location header contains ?code={authCode}
        NOTE: Do NOT follow redirect automatically - extract code from Location

Step 3: POST /auth/o/token/
        Content-Type: application/x-www-form-urlencoded
        Body: client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ&grant_type=authorization_code&code={authCode}
        Response: {"access_token": "...", "refresh_token": "...", "expires_in": 36000}
```

### 1.2 Token Refresh (VERIFIED)

```bash
curl -X POST https://api.beatport.com/v4/auth/o/token/ \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ" \
  -d "grant_type=refresh_token" \
  -d "refresh_token={REFRESH_TOKEN}"
```

Response:
```json
{
  "access_token": "new_token",
  "expires_in": 36000,
  "token_type": "Bearer",
  "scope": "app:locker user:dj",
  "refresh_token": "new_refresh_token"
}
```

### 1.3 Token Usage

All API calls require:
```
Authorization: Bearer {access_token}
```

### 1.4 Token Refresh Timing

Refresh when: `current_time + 300 >= (issued_at + expires_in)`
(5-minute buffer before expiry)

---

## 2. API Endpoints (ALL VERIFIED)

### 2.1 Streaming - PRIMARY ENDPOINT

| Endpoint | Method | Auth | Response |
|----------|--------|------|----------|
| `/catalog/tracks/{id}/stream/` | GET | YES | HLS manifest URL |

**Request:**
```bash
curl -H "Authorization: Bearer {TOKEN}" \
  "https://api.beatport.com/v4/catalog/tracks/23050990/stream/"
```

**Response (VERIFIED):**
```json
{
  "stream_url": "https://needledrop.beatport.com/{timestamp}_{hash}/{path}/{uuid}.128k.aac.m3u8?start=0&end=256463",
  "sample_start_ms": 0,
  "sample_end_ms": 256463
}
```

**IMPORTANT**: `sample_end_ms` is the FULL TRACK duration, not a 30-second preview.

### 2.2 Download - Alternative (beatportdl uses this)

| Endpoint | Method | Auth | Response |
|----------|--------|------|----------|
| `/catalog/tracks/{id}/download/?quality={q}` | GET | YES | Direct file URL |

Quality values: `medium` (128k), `high` (256k), `lossless` (FLAC)

**Response:**
```json
{
  "location": "https://assets.beatportprod.com/...",
  "stream_quality": ".128k.aac.mp4"
}
```

### 2.3 Catalog Endpoints

| Endpoint | Method | Notes |
|----------|--------|-------|
| `/catalog/tracks/{id}/` | GET | Track metadata |
| `/catalog/releases/{id}/` | GET | Release metadata |
| `/catalog/releases/{id}/tracks/?page={n}&per_page=100` | GET | Paginated tracks |
| `/catalog/playlists/{id}/` | GET | Playlist metadata |
| `/catalog/playlists/{id}/tracks/?page={n}` | GET | Paginated tracks |
| `/catalog/charts/{id}/` | GET | Chart metadata |
| `/catalog/charts/{id}/tracks/?page={n}` | GET | Paginated tracks |
| `/catalog/artists/{id}/` | GET | Artist metadata |
| `/catalog/artists/{id}/tracks/?page={n}` | GET | Paginated tracks |
| `/catalog/labels/{id}/` | GET | Label metadata |
| `/catalog/labels/{id}/releases/?page={n}` | GET | Paginated releases |
| `/catalog/genres/` | GET | All genres list |
| `/catalog/genres/{id}/top/100/?per_page=100` | GET | Top 100 for genre |
| `/catalog/search/?q={query}` | GET | Search results |

### 2.4 User Endpoints

| Endpoint | Method | Notes |
|----------|--------|-------|
| `/my/beatport/` | GET | User profile, favorites |
| `/my/subscriptions` | GET | Subscription tier |
| `/auth/o/introspect/` | GET | Token validation |

---

## 3. HLS Streaming Details (VERIFIED)

### 3.1 Manifest Structure

Fetching the `stream_url` returns an M3U8 playlist:

```m3u8
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-PLAYLIST-TYPE:VOD
#EXT-X-MEDIA-SEQUENCE:0
#EXT-X-TARGETDURATION:6
#EXT-X-KEY:METHOD=AES-128,URI="{uuid}.128k.aac.key?start=0&end={ms}",IV=0x{32_hex_chars}
#EXTINF:5.015510,
{uuid}.128k.aac-1.aac?start=0&end={ms}
#EXTINF:5.015510,
{uuid}.128k.aac-2.aac?start=0&end={ms}
...
#EXT-X-ENDLIST
```

### 3.2 Encryption Details (VERIFIED)

| Property | Value |
|----------|-------|
| Algorithm | AES-128-CBC |
| Key Size | 16 bytes |
| IV Source | From `#EXT-X-KEY` line (hex after `0x`) |
| Key Source | Fetch from URI in `#EXT-X-KEY` |
| Padding | PKCS#7 |

### 3.3 Segment Decryption (TESTED WORKING)

```bash
# 1. Fetch key (16 bytes raw binary)
KEY=$(curl -s "{manifest_base}/{uuid}.128k.aac.key?..." | xxd -p)

# 2. Get IV from manifest (remove 0x prefix)
IV="201d0ce6c77d92062639fe2687d2a97f"

# 3. Fetch and decrypt segment
curl -s "{manifest_base}/{uuid}.128k.aac-1.aac?..." -o segment.enc
openssl enc -d -aes-128-cbc -K "$KEY" -iv "$IV" -in segment.enc -out segment.aac

# Result: playable AAC audio
```

---

## 4. Track Metadata Schema

```json
{
  "id": 23050990,
  "name": "Track Name",
  "mix_name": "Extended Mix",
  "slug": "track-slug",
  "bpm": 132,
  "key": {"name": "G min", "camelot": "6A"},
  "genre": {"id": 1, "name": "Tech House"},
  "sub_genre": {"id": 2, "name": "Minimal"},
  "length_ms": 256463,
  "artists": [{"id": 123, "name": "Artist Name"}],
  "remixers": [],
  "release": {
    "id": 456,
    "name": "Release Name",
    "label": {"id": 789, "name": "Label Name"},
    "catalog_number": "CAT001"
  },
  "publish_date": "2026-01-15",
  "image": {"uri": "https://..."},
  "url": "https://www.beatport.com/track/..."
}
```

---

## 5. Pagination

All collection endpoints use cursor-based pagination:

```json
{
  "next": "https://api.beatport.com/v4/.../tracks/?page=2",
  "previous": null,
  "count": 150,
  "page": "1",
  "per_page": 50,
  "results": [...]
}
```

Continue fetching while `next` is not null.

---

## 6. Implementation Checklist for Agents

### Authentication
- [ ] Implement POST /auth/login/ with JSON body
- [ ] Extract sessionid cookie from response
- [ ] Implement GET /auth/o/authorize/ with cookie, extract code from redirect
- [ ] Implement POST /auth/o/token/ for code exchange
- [ ] Implement token refresh with 5-minute buffer
- [ ] Store tokens securely (QtKeychain)

### Streaming
- [ ] Call GET /catalog/tracks/{id}/stream/ with Bearer token
- [ ] Parse JSON response for stream_url
- [ ] Fetch M3U8 manifest from stream_url
- [ ] Parse #EXT-X-KEY for key URI and IV
- [ ] Fetch 16-byte AES key
- [ ] For each segment: fetch, decrypt AES-128-CBC, strip PKCS#7 padding
- [ ] Concatenate decrypted segments for playback

### Catalog
- [ ] Implement search via /catalog/search/
- [ ] Implement genre listing via /catalog/genres/
- [ ] Implement Top 100 per genre via /catalog/genres/{id}/top/100/
- [ ] Handle pagination for all collection endpoints

---

## 7. Common Errors

| Error | Cause | Fix |
|-------|-------|-----|
| `{"detail":"Authentication credentials were not provided."}` | Missing or expired token | Refresh token or re-authenticate |
| 302 with no code in redirect | Session cookie invalid | Re-do login step |
| HLS 403 | URL expired | Re-fetch stream URL |

---

## 8. Test Commands

```bash
# Refresh token
curl -X POST https://api.beatport.com/v4/auth/o/token/ \
  -d "client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ&grant_type=refresh_token&refresh_token={TOKEN}"

# Get stream URL
curl -H "Authorization: Bearer {TOKEN}" \
  https://api.beatport.com/v4/catalog/tracks/23050990/stream/

# Search
curl -H "Authorization: Bearer {TOKEN}" \
  "https://api.beatport.com/v4/catalog/search/?q=deadmau5"

# Get genres
curl -H "Authorization: Bearer {TOKEN}" \
  https://api.beatport.com/v4/catalog/genres/
```
