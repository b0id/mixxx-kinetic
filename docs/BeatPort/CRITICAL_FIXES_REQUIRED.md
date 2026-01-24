# CRITICAL FIXES REQUIRED - Beatport Streaming

**Date**: 2026-01-23
**Status**: 🚨 BLOCKING ISSUES IDENTIFIED

---

## HAR Analysis Results

### ✅ Streaming Endpoint FOUND

**Correct Endpoint**: `GET /v4/catalog/tracks/{trackId}/stream/`

**Response Format**:
```json
{
  "stream_url": "https://needledrop.beatport.com/1769220310_8628b059acf75de5b4b1835c2371ec162b3f27e1/9/c/0/9c0c3f13-602f-4d4f-8b7b-00318a8205f6.128k.aac.m3u8?start=0&end=256463",
  "sample_start_ms": 0,
  "sample_end_ms": 256463
}
```

### 🚨 PROBLEMS IN CURRENT IMPLEMENTATION

#### Problem 1: WRONG ENDPOINT
**Current**: `/catalog/tracks/{id}/download`
**Correct**: `/catalog/tracks/{id}/stream/`

**File**: `src/streaming/hook/beatportservice.cpp:326`

#### Problem 2: WRONG EXPECTED RESPONSE
**Current**: Code expects direct audio URL
**Correct**: Endpoint returns JSON with HLS manifest URL (`.m3u8`)

#### Problem 3: HLS NOT SUPPORTED YET
**Current**: FUSE driver expects direct MP3/AAC file
**Correct**: Need to parse M3U8 playlist and fetch segments

#### Problem 4: AUTH MAY NOT BE NEEDED
**Observation**: Web player's `/stream/` requests show NO Authorization header in HAR
**Implication**: Preview streaming might not require auth, OR cookies are being used

---

## IMMEDIATE FIXES NEEDED

### Fix 1: Change Endpoint (5 minutes)

**File**: `src/streaming/hook/beatportservice.cpp`
**Line**: 326

**Change**:
```cpp
// WRONG:
QUrl url(QString("%1/catalog/tracks/%2/download").arg(m_config.baseUrl, trackId));

// CORRECT:
QUrl url(QString("%1/catalog/tracks/%2/stream/").arg(m_config.baseUrl, trackId));
```

**Remove quality parameter** (not used by /stream/):
```cpp
// DELETE lines 327-337 (QUrlQuery for quality)
```

### Fix 2: Parse JSON Response (10 minutes)

**File**: `src/streaming/hook/beatportservice.cpp`
**Lines**: 347-390

**Current code expects**:
```cpp
// Tries to parse JSON with direct URL
```

**Should parse**:
```cpp
QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
QJsonObject obj = doc.object();

QString hlsUrl = obj["stream_url"].toString();
int sampleStartMs = obj["sample_start_ms"].toInt();
int sampleEndMs = obj["sample_end_ms"].toInt();

// NOTE: This is an HLS manifest, not direct audio!
```

### Fix 3A: QUICK WORKAROUND - Use Direct Segment URL

**Goal**: Get audio playing ASAP without implementing full HLS support

**Change the M3U8 URL to direct segment**:
```cpp
// HLS URL example:
// https://needledrop.beatport.com/.../file.128k.aac.m3u8

// Direct segment URL (remove .m3u8, add -1.aac):
// https://needledrop.beatport.com/.../file.128k.aac-1.aac
QString directUrl = hlsUrl;
directUrl.replace(".m3u8", "-1.aac"); // Get first segment

StreamInfo info;
info.streamUrl = directUrl;
info.format = "aac";
info.bitrate = 128; // kbps
```

**Limitation**: Only plays first ~3 seconds (one segment)

### Fix 3B: PROPER FIX - Implement HLS Parser (1-2 hours)

**Need to**:
1. Fetch `.m3u8` manifest
2. Parse segment list (`.aac-1.aac`, `.aac-2.aac`, etc.)
3. Return segment URLs to FUSE driver
4. FUSE driver concatenates segments on-the-fly

**Implementation Options**:

**Option A**: Parse M3U8 in `BeatportService::getStreamInfo()`
- Fetch M3U8
- Extract all segment URLs
- Return first segment URL initially
- Store segment list for later use

**Option B**: Make FUSE driver HLS-aware
- Pass M3U8 URL to FUSE
- FUSE fetches and parses playlist
- FUSE handles segment streaming

**Recommendation**: Option A (simpler, less FUSE changes)

---

## AUTH MYSTERY

### Observation
Web player makes `/stream/` requests with **NO** auth:
- No `Authorization: Bearer` header
- No cookies (in HAR export)

### Possible Explanations

1. **HAR stripped auth** (likely) - Export removed sensitive headers
2. **Preview doesn't need auth** - Free streaming for preview/needledrop
3. **Cookie-based auth** - Using HttpOnly cookies (not in HAR)

### Test Required

Try calling `/stream/` endpoint **without** auth:
```bash
curl https://api.beatport.com/v4/catalog/tracks/23050990/stream/
```

**If it works without auth**: Preview streaming is public!
**If 401**: Auth required but HAR didn't capture it

---

## BEATPORTDL CONFUSION

### Why beatportdl != Web Player

beatportdl uses `/download` endpoint for **full track downloads** (not streaming):
- Requires subscription
- Returns complete file
- Different use case

Web player uses `/stream/` for **preview/streaming**:
- May not require auth (needs testing)
- Returns HLS manifest
- Segmented delivery

**You mixed the two approaches** - used beatportdl auth but web player endpoint expectations.

---

## ACTION PLAN (TANGIBLE STEPS)

### Step 1: Test /stream/ Without Auth (2 minutes)

```bash
curl https://api.beatport.com/v4/catalog/tracks/23050990/stream/ | jq
```

**Expected**:
- If 200: Preview streaming is PUBLIC
- If 401: Auth required

### Step 2: Fix Endpoint in Code (5 minutes)

Edit `beatportservice.cpp:326`:
```cpp
QUrl url(QString("%1/catalog/tracks/%2/stream/").arg(m_config.baseUrl, trackId));
// Remove QUrlQuery section (lines 327-337)
```

### Step 3: Fix Response Parsing (10 minutes)

Edit `beatportservice.cpp:347-390` to parse JSON:
```cpp
QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
QString hlsUrl = doc.object()["stream_url"].toString();

// WORKAROUND: Convert M3U8 to first segment
QString directUrl = hlsUrl.replace(".m3u8", "-1.aac");

StreamInfo info;
info.streamUrl = directUrl;
info.format = "aac";
info.bitrate = 128;
info.sampleRate = 44100;
info.channels = 2;
```

### Step 4: Build & Test (5 minutes)

```bash
cd /home/b0id/repo/build
cmake --build . --target mixxx
./mixxx
```

### Step 5: If Step 3 Plays Audio - Implement Full HLS (1 hour)

Create HLS segment fetcher:
1. Parse M3U8 manifest
2. Extract segment URLs
3. Stream segments sequentially

---

## BOTTOM LINE

**Your implementation has 3 bugs**:
1. ❌ Using `/download` endpoint (should be `/stream/`)
2. ❌ Expecting direct audio URL (should parse JSON response)
3. ❌ Not handling HLS format (M3U8 → segments)

**Quick win**: Fix endpoints + parse JSON + use first segment = ~3 seconds of audio
**Full solution**: Add HLS parser = full streaming

---

## NEXT STEPS

1. Test `/stream/` endpoint manually (curl)
2. Fix endpoint in code
3. Fix response parsing
4. Test with first segment
5. Implement full HLS if step 4 works
