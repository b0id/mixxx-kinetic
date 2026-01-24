# Beatport Integration Research

**Last Updated**: 2026-01-24
**Status**: Ready for Implementation

---

## Document Index

### Core References (Start Here)

| Document | Purpose | Confidence |
|----------|---------|------------|
| [VERIFIED_STREAMING_API.md](./VERIFIED_STREAMING_API.md) | **DEFINITIVE** - Complete verified streaming flow | HIGH |
| [CONSOLIDATED_API_REFERENCE.md](./CONSOLIDATED_API_REFERENCE.md) | Quick reference for all endpoints | HIGH |

### Implementation Guides

| Document | Purpose |
|----------|---------|
| [CRITICAL_FIXES_REQUIRED.md](./CRITICAL_FIXES_REQUIRED.md) | Known bugs and fixes needed in current code |
| [IMPLEMENTATION_ROADMAP.md](./IMPLEMENTATION_ROADMAP.md) | Phased implementation plan |

### Research History

| Document | Purpose |
|----------|---------|
| [HAR_Analysis_Report.md](./HAR_Analysis_Report.md) | Web player traffic analysis |
| [AUTH_DIAGNOSTIC_STRATEGIES.md](./AUTH_DIAGNOSTIC_STRATEGIES.md) | Auth debugging approaches |
| [SESSION_SUMMARY.md](./SESSION_SUMMARY.md) | Previous session notes |

---

## Key Findings Summary

### What Works (VERIFIED)

1. **beatportdl's session cookie OAuth** works for streaming
2. **`/v4/catalog/tracks/{id}/stream/`** returns full track HLS
3. **Same token** works for both download and stream endpoints
4. **HLS decryption** is standard AES-128-CBC

### What You Need to Implement

```
Auth Layer:
├── Session cookie flow (POST /auth/login/)
├── OAuth code exchange (GET /auth/o/authorize/, POST /auth/o/token/)
└── Token refresh (every 10 hours)

Streaming Layer:
├── GET /stream/ → parse JSON for stream_url
├── Fetch M3U8 manifest
├── Parse #EXT-X-KEY for key URI and IV
├── Fetch AES-128 key (16 bytes)
├── For each segment:
│   ├── Fetch encrypted AAC
│   ├── Decrypt with AES-128-CBC
│   └── Pipe to audio decoder
└── Handle segment seeking
```

---

## Quick Test Commands

### Refresh Token
```bash
curl -X POST https://api.beatport.com/v4/auth/o/token/ \
  -d "client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ" \
  -d "grant_type=refresh_token" \
  -d "refresh_token={YOUR_REFRESH_TOKEN}"
```

### Get Stream URL
```bash
curl -H "Authorization: Bearer {TOKEN}" \
  https://api.beatport.com/v4/catalog/tracks/23050990/stream/
```

### Fetch Manifest
```bash
curl "https://needledrop.beatport.com/{...}/file.m3u8?start=0&end={ms}"
```

---

## Reference Files in beatportdl

| File | Location | Contains |
|------|----------|----------|
| auth.go | `internal/beatport/auth.go` | Complete OAuth implementation |
| needledrop.go | `cmd/beatportdl/needledrop.go` | HLS parsing + AES decryption |
| tracks.go | `internal/beatport/tracks.go` | Track API endpoints |
| beatport.go | `internal/beatport/beatport.go` | HTTP client setup |

---

## Risk Assessment

| Risk | Mitigation |
|------|------------|
| Token expiration (10hr) | Implement refresh before expiry (5min buffer) |
| URL expiration | URLs have timestamp - refetch if stale |
| Rate limiting | Unknown limits - implement backoff |
| API changes | No versioning guarantee - monitor |

---

## Next Steps

1. Read [VERIFIED_STREAMING_API.md](./VERIFIED_STREAMING_API.md) for complete flow
2. Review beatportdl source for implementation patterns
3. Implement in order: Auth → Stream URL → HLS → Decrypt → Playback
