# Beatport Streaming - Implementation Roadmap

**Status**: 🚨 Critical bugs identified, clear path forward
**Estimated Time to First Audio**: 15-30 minutes

---

## 🎯 The Problem

Your implementation mixed two different Beatport APIs:
- **beatportdl**: Uses `/download` for full track downloads (requires auth + subscription)
- **Web player**: Uses `/stream/` for preview streaming (may not need auth, HLS format)

**Result**: Wrong endpoint + wrong response parsing = no audio

---

## 🔍 What the HAR Revealed

### Actual Streaming Flow (from `dj.beatport.com3.har`)

1. **Request**: `GET /v4/catalog/tracks/23050990/stream/`
   - No Authorization header visible (may not need auth for previews)

2. **Response**: JSON (not direct audio!)
   ```json
   {
     "stream_url": "https://needledrop.beatport.com/1769220310_8628b059acf75de5b4b1835c2371ec162b3f27e1/9/c/0/9c0c3f13-602f-4d4f-8b7b-00318a8205f6.128k.aac.m3u8",
     "sample_start_ms": 0,
     "sample_end_ms": 256463
   }
   ```

3. **stream_url**: HLS playlist (M3U8)
   - Lists 50+ segments: `.aac-1.aac`, `.aac-2.aac`, ... `.aac-52.aac`
   - Each segment ~80KB, ~3 seconds of audio
   - Time-limited signed URLs (expire after ~1 hour)

---

## ✅ The Solution (3 Phases)

### Phase 1: Quick Win (15-30 min) - Get Sound Playing

**Goal**: Play first 3 seconds of audio

**Changes Needed**:
1. Change endpoint: `/download` → `/stream/`
2. Parse JSON response to get `stream_url`
3. Workaround HLS: Use first segment (`-1.aac` instead of `.m3u8`)

**Files**: See `EXACT_CODE_FIXES.md` for copy/paste code

**Result**: ~3 seconds of audio, then silence

---

### Phase 2: Full HLS Support (1-2 hours) - Complete Track

**Goal**: Stream all segments for full preview

**Implementation**:
1. Fetch M3U8 manifest from `stream_url`
2. Parse segment list (standard M3U8 format)
3. Return concatenated segments to FUSE driver
4. Handle segment fetching in RangeFetcher

**Result**: Full 30-second preview playback

---

### Phase 3: Full Downloads (Future) - Beyond Preview

**Goal**: Full track downloads for subscribers

**Requirements**:
1. Implement auth properly (session flow OR PKCE)
2. Switch to `/download` endpoint (different from `/stream/`)
3. Handle subscription tier checks
4. Store downloaded files

**Result**: Full track access for paying users

---

## 📋 Immediate Action Plan

### Step 1: Test Auth Requirements (2 minutes)

```bash
# Test if preview streaming needs auth
curl https://api.beatport.com/v4/catalog/tracks/23050990/stream/

# If you get JSON: No auth needed for preview! Skip to Step 3
# If you get 401: Auth needed, proceed to Step 2
```

### Step 2: Debug Auth (If Needed - 30 min)

Follow `DEBUG_AUTH_STEP_BY_STEP.md`:
- Test session cookie flow manually with curl
- Identify where it's failing
- Fix auth implementation

### Step 3: Fix Streaming Endpoint (10 min)

Follow `EXACT_CODE_FIXES.md`:
- Change 1 line: endpoint URL
- Change 30 lines: JSON parsing
- Build and test

### Step 4: Test (5 min)

```bash
cd /home/b0id/repo/build
cmake --build . --target mixxx
./mixxx
# Load Beatport track
# Should hear ~3 seconds of audio
```

### Step 5: Implement Full HLS (If Step 4 Works)

Create M3U8 parser and segment concatenation

---

## 📚 Documentation Guide

**Start here** (in order):
1. `CRITICAL_FIXES_REQUIRED.md` - Understand what's wrong
2. `DEBUG_AUTH_STEP_BY_STEP.md` - Test auth with curl
3. `EXACT_CODE_FIXES.md` - Copy/paste code changes
4. `HAR_Analysis_Report.md` - Deep dive into web player behavior

**Reference**:
- `Beatport_Integration_Strategy.md` - API endpoints
- `SessionCookieAuthFlow.md` - Auth implementation
- `KINETIC_DEVLOG.md` - Updated with today's findings

---

## 🎯 Success Criteria

### Phase 1 Complete When:
- [ ] `/stream/` endpoint is called (not `/download`)
- [ ] JSON response is parsed correctly
- [ ] First segment URL is extracted
- [ ] ~3 seconds of audio plays in Mixxx

### Phase 2 Complete When:
- [ ] M3U8 manifest is fetched
- [ ] All 50+ segments are listed
- [ ] Segments play sequentially
- [ ] Full 30-second preview plays

### Phase 3 Complete When:
- [ ] Auth flow works end-to-end
- [ ] Full downloads available for subscribers
- [ ] Tracks stored in Mixxx library

---

## ⚠️ Common Pitfalls

1. **Don't assume auth is needed**
   - Test `/stream/` without auth first
   - Preview mode might be public

2. **Don't use beatportdl flow for streaming**
   - beatportdl = downloads
   - Web player = streaming
   - Different APIs!

3. **Don't expect direct audio URLs**
   - Response is JSON with HLS URL
   - HLS URL is M3U8 playlist
   - Actual audio is in segments

---

## 🔥 Bottom Line

**You're close!** The infrastructure is solid. Just need to:
1. Fix endpoint (1 line)
2. Parse JSON (10 lines)
3. Handle HLS segments (workaround: 1 line, proper: 50 lines)

**Time investment**:
- Quick win: 15-30 min
- Full solution: 1-2 hours
- With auth: +30 min

**Go to**: `EXACT_CODE_FIXES.md` and start implementing!
