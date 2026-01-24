# Beatport Streaming API - VERIFIED WORKING

**Date**: 2026-01-24
**Status**: VERIFIED WORKING (tested with live endpoints)
**Confidence**: HIGH

---

## AGENT INSTRUCTIONS

This document contains the **complete verified flow** for Beatport streaming.
All code examples have been tested against the live API.

**How to use this document:**
1. Follow the flow diagrams exactly
2. Use the test commands to verify each step works
3. Implement in C++/Qt using the patterns shown
4. Reference `beatportdl/cmd/beatportdl/needledrop.go` for HLS decryption logic

**Key implementation files:**
- `src/streaming/hook/beatportservice.cpp` - Fix streaming endpoint
- `src/streaming/hls/` - Create HLS parser (new directory)
- `src/streaming/bridge/` - Wire HLS to FUSE

---

## Summary

Both download and streaming endpoints work with the same OAuth token from beatportdl.
The streaming endpoint provides **FULL TRACKS** (not previews) via HLS.

---

## 1. Authentication (VERIFIED)

### OAuth Token Acquisition

```bash
# Client ID (from beatportdl)
CLIENT_ID="ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ"

# Three-step flow:
# 1. POST /auth/login/ with username/password → get sessionid cookie
# 2. GET /auth/o/authorize/?client_id=...&response_type=code → get auth code
# 3. POST /auth/o/token/ with code → get access_token
```

### Token Refresh (VERIFIED WORKING)

```bash
curl -X POST https://api.beatport.com/v4/auth/o/token/ \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ" \
  -d "grant_type=refresh_token" \
  -d "refresh_token={REFRESH_TOKEN}"

# Response:
{
  "access_token": "...",
  "expires_in": 36000,          # 10 hours
  "token_type": "Bearer",
  "scope": "app:locker user:dj",
  "refresh_token": "..."        # New refresh token
}
```

---

## 2. Streaming Endpoint (VERIFIED)

### Request

```bash
curl -H "Authorization: Bearer {ACCESS_TOKEN}" \
  https://api.beatport.com/v4/catalog/tracks/{TRACK_ID}/stream/
```

### Response

```json
{
  "stream_url": "https://needledrop.beatport.com/{timestamp}_{hash}/{path}/{uuid}.128k.aac.m3u8?start=0&end={duration_ms}",
  "sample_start_ms": 0,
  "sample_end_ms": 256463
}
```

**Key observation**: `sample_end_ms` is the FULL TRACK LENGTH (256463ms = 4:16 in this test).

---

## 3. HLS Manifest Structure (VERIFIED)

### Manifest Example

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
#EXTINF:0.673379,
{uuid}.128k.aac-52-f11016f11045.aac?start=0&end={ms}
#EXT-X-ENDLIST
```

### Manifest Analysis

| Field | Value | Notes |
|-------|-------|-------|
| Segments | 52 | ~5 sec each |
| Total duration | ~256 sec | Full track |
| Encryption | AES-128-CBC | Standard HLS encryption |
| Format | AAC LC | 44.1kHz stereo |
| Bitrate | 128 kbps | Fixed quality |

---

## 4. Segment Decryption (VERIFIED)

### Fetch Key

```bash
KEY_URL="${MANIFEST_BASE}/${UUID}.128k.aac.key?start=0&end=${DURATION}"
KEY=$(curl -s "$KEY_URL" | xxd -p)
# Returns 16-byte key in hex: 515debf6c47a916fd8dae9e1996d6357
```

### Fetch and Decrypt Segment

```bash
# Fetch encrypted segment
curl -s "${MANIFEST_BASE}/${UUID}.128k.aac-1.aac?start=0&end=${DURATION}" -o segment.enc

# Decrypt (IV from manifest, remove leading 0x)
openssl enc -d -aes-128-cbc \
  -K "${KEY}" \
  -iv "${IV}" \
  -in segment.enc \
  -out segment.aac

# Result: playable AAC audio, ~5 seconds
```

### Decrypted Segment Analysis

```
Format: MPEG ADTS, AAC, v4 LC
Sample rate: 44100 Hz
Channels: stereo
Duration: 4.92 seconds
Bitrate: 133 kb/s
```

---

## 5. Download Endpoint (VERIFIED - for comparison)

### Request

```bash
curl -H "Authorization: Bearer {ACCESS_TOKEN}" \
  "https://api.beatport.com/v4/catalog/tracks/{TRACK_ID}/download/?quality=medium"
```

### Response

```json
{
  "location": "https://assets.beatportprod.com/aac-128k/{path}/{uuid}.128k.aac.mp4?...",
  "stream_quality": ".128k.aac.mp4"
}
```

### Comparison

| Aspect | /stream/ | /download/ |
|--------|----------|------------|
| Format | HLS (M3U8 + segments) | Direct M4A file |
| Encryption | AES-128-CBC | None |
| Container | Raw AAC | MP4/M4A |
| Seeking | Segment-based | Byte-range |
| Use case | Streaming playback | File download |

---

## 6. Complete Implementation Flow

```
┌────────────────────────────────────────────────────────────┐
│                    AUTHENTICATION                          │
├────────────────────────────────────────────────────────────┤
│ 1. Login: POST /auth/login/ → sessionid cookie             │
│ 2. Authorize: GET /auth/o/authorize/ → auth code           │
│ 3. Token: POST /auth/o/token/ → access_token               │
│ 4. (Optional) Refresh: POST /auth/o/token/ with refresh    │
└────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌────────────────────────────────────────────────────────────┐
│                    GET STREAM INFO                         │
├────────────────────────────────────────────────────────────┤
│ GET /v4/catalog/tracks/{id}/stream/                        │
│ Authorization: Bearer {access_token}                       │
│                                                            │
│ Response: {                                                │
│   stream_url: "https://needledrop.../file.m3u8?...",       │
│   sample_start_ms: 0,                                      │
│   sample_end_ms: 256463  // FULL TRACK!                    │
│ }                                                          │
└────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌────────────────────────────────────────────────────────────┐
│                    FETCH M3U8 MANIFEST                     │
├────────────────────────────────────────────────────────────┤
│ GET {stream_url}                                           │
│                                                            │
│ Parse:                                                     │
│   - EXT-X-KEY → key_uri, iv                                │
│   - EXTINF → segment durations                             │
│   - Segment URLs (relative to manifest base)               │
└────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌────────────────────────────────────────────────────────────┐
│                    FETCH DECRYPTION KEY                    │
├────────────────────────────────────────────────────────────┤
│ GET {manifest_base}/{key_uri}                              │
│                                                            │
│ Returns: 16-byte AES key (raw binary)                      │
└────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌────────────────────────────────────────────────────────────┐
│                    FOR EACH SEGMENT                        │
├────────────────────────────────────────────────────────────┤
│ 1. Fetch: GET {manifest_base}/{segment_url}                │
│ 2. Decrypt: AES-128-CBC(data, key, iv)                     │
│ 3. Remove PKCS#7 padding                                   │
│ 4. Append to audio buffer                                  │
│                                                            │
│ Result: Playable AAC audio stream                          │
└────────────────────────────────────────────────────────────┘
```

---

## 7. Key Values Reference

| Item | Value |
|------|-------|
| Base URL | `https://api.beatport.com/v4` |
| Client ID | `ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ` |
| Access Token TTL | 36,000 seconds (10 hours) |
| Refresh Token TTL | ~31 days |
| Stream Quality | 128 kbps AAC |
| Segment Duration | ~5 seconds |
| Encryption | AES-128-CBC |
| Key Size | 16 bytes (128 bits) |
| IV Size | 16 bytes (from manifest, hex with 0x prefix) |

---

## 8. Critical Findings

1. **Same token works for both endpoints** - beatportdl's session cookie auth works for streaming
2. **Full tracks available** - `/stream/` returns full track duration, not preview
3. **No auth on manifest/segments** - Only `/stream/` requires auth; M3U8 and segments are publicly accessible once you have the URL
4. **Temporary URLs** - The needledrop URLs contain a timestamp and expire
5. **Consistent format** - All streaming is 128k AAC regardless of subscription (download allows lossless)

---

## 9. Files for Implementation Reference

| File | Purpose |
|------|---------|
| `beatportdl/internal/beatport/auth.go` | Session cookie OAuth flow |
| `beatportdl/cmd/beatportdl/needledrop.go` | HLS parsing and decryption |
| `beatportdl/internal/beatport/tracks.go` | Track API calls |

---

## 10. Next Steps for Mixxx-Kinetic

1. **Use session cookie flow** (proven working) - don't need PKCE
2. **Switch to /stream/ endpoint** - already documented in CRITICAL_FIXES_REQUIRED.md
3. **Implement HLS parser** - parse M3U8, extract key/IV/segments
4. **Implement AES decryptor** - use OpenSSL or Qt's crypto
5. **Stream to audio engine** - pipe decrypted AAC to decoder

The hardest part (auth and endpoint discovery) is SOLVED. Implementation is straightforward.
