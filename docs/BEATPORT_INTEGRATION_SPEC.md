# Beatport Integration Specification
## Mixxx-Kinetic Streaming Services Implementation

**Generated:** 2026-01-24
**Status:** Authentication VERIFIED, Browsing Model PENDING
**Priority:** API enumeration for catalog rendering

---

## 1. EXECUTIVE SUMMARY

### What's Working
| Component | Status | Evidence |
|-----------|--------|----------|
| Session Cookie OAuth | ✅ Verified | Token acquired, 30-char access token |
| `/stream/` Endpoint | ✅ Verified | Returns HLS URL with **full track duration** |
| HLS Parsing | ✅ Implemented | M3U8 manifest + AES-128-CBC decryption understood |
| Search Endpoint | ✅ Works | `/catalog/search/?q=...` returns results |
| Token Refresh | ✅ Implemented | 5-min buffer before expiry |

### What's Missing for Catalog Browsing
| Component | Status | Required For |
|-----------|--------|--------------|
| Genre Browser | ❌ Not wired | Primary navigation entry point |
| Charts Display | ❌ Not wired | Ranked/curated lists |
| Artist/Label Pivot | ❌ Not wired | Entity-based browsing |
| Playlist Support | ❌ Not wired | DJ Charts & curated collections |
| TreeItemModel Population | ❌ BLOCKER | Sidebar catalog display |

### Critical Finding
**Full track streaming confirmed.** The `sample_end_ms` values (e.g., 256,463 ms = 4:16) represent complete tracks, NOT 30-second previews. Subscription tier (Link Pro) grants full access.

---

## 2. AUTHENTICATION REFERENCE

### Verified 3-Step Flow (beatportdl pattern)
```
Step 1: Login
  POST /auth/login/
  Body: {"username": "email", "password": "pass"}
  Returns: sessionid cookie

Step 2: Authorize
  GET /auth/o/authorize/?client_id={CLIENT_ID}&response_type=code
  Headers: Cookie: sessionid={from step 1}
  Returns: 302 redirect with ?code={auth_code}

Step 3: Token Exchange
  POST /auth/o/token/
  Body: client_id={CLIENT_ID}&grant_type=authorization_code&code={auth_code}
  Returns: {access_token, refresh_token, expires_in: 36000}
```

### Key Values
```
CLIENT_ID:     ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ
BASE_URL:      https://api.beatport.com/v4
TOKEN_TTL:     36,000 seconds (10 hours)
REFRESH_AT:    current_time + 300 >= issued_at + expires_in
```

### Current Implementation Location
- `src/streaming/hook/beatportservice.cpp` - Main service
- `src/streaming/hook/oauthmanager.cpp` - Token lifecycle

---

## 3. BROWSING MODEL API REFERENCE

This is **the missing piece** for rendering the Beatport catalog experience.

### 3.1 Genre-First Navigation (Primary Entry Point)

**List All Genres**
```
GET /v4/catalog/genres/
Authorization: Bearer {token}

Response:
{
  "results": [
    {"id": 1, "name": "House", "slug": "house"},
    {"id": 2, "name": "Techno", "slug": "techno"},
    {"id": 3, "name": "Drum & Bass", "slug": "drum-and-bass"},
    ...
  ]
}
```

**Get Genre Details (with Subgenres)**
```
GET /v4/catalog/genres/{id}/
Authorization: Bearer {token}

Response:
{
  "id": 1,
  "name": "House",
  "slug": "house",
  "subgenres": [
    {"id": 11, "name": "Deep House", "slug": "deep-house"},
    {"id": 12, "name": "Tech House", "slug": "tech-house"},
    ...
  ]
}
```

**Genre Top 100 Tracks**
```
GET /v4/catalog/genres/{id}/top/100/
Authorization: Bearer {token}

Response: Paginated list of Track objects
```

### 3.2 Charts & Ranked Lists

**List Charts**
```
GET /v4/catalog/charts/
Authorization: Bearer {token}

Response:
{
  "results": [
    {
      "id": 123,
      "name": "Beatport Top 100",
      "slug": "beatport-top-100",
      "track_count": 100,
      "genres": [{"id": 1, "name": "House"}],
      "image": {"url": "https://..."},
      "publish_date": "2026-01-24"
    }
  ]
}
```

**Get Chart Tracks (Paginated)**
```
GET /v4/catalog/charts/{id}/tracks/?page=1&per_page=50
Authorization: Bearer {token}

Response:
{
  "count": 100,
  "next": "/v4/catalog/charts/123/tracks/?page=2",
  "results": [
    {
      "id": 456,
      "name": "Track Name",
      "mix_name": "Original Mix",
      "artists": [{"id": 789, "name": "Artist Name"}],
      "bpm": 128,
      "key": {"name": "Am", "camelot": "8A"},
      "length_ms": 345000,
      "release": {"id": 111, "name": "Release Name"},
      "label": {"id": 222, "name": "Label Name"},
      "genre": {"id": 1, "name": "House"},
      "publish_date": "2026-01-20"
    }
  ]
}
```

### 3.3 Entity-Based Browsing

**Artist Details**
```
GET /v4/catalog/artists/{id}/
Authorization: Bearer {token}

Response:
{
  "id": 789,
  "name": "Artist Name",
  "slug": "artist-name",
  "image": {"url": "https://..."}
}
```

**Artist Tracks (Paginated)**
```
GET /v4/catalog/artists/{id}/tracks/?page=1&per_page=50
Authorization: Bearer {token}
```

**Label Details**
```
GET /v4/catalog/labels/{id}/
Authorization: Bearer {token}
```

**Label Releases (Paginated)**
```
GET /v4/catalog/labels/{id}/releases/?page=1&per_page=50
Authorization: Bearer {token}
```

### 3.4 Releases

**Release Details**
```
GET /v4/catalog/releases/{id}/
Authorization: Bearer {token}

Response:
{
  "id": 111,
  "name": "Release Name",
  "artists": [{"id": 789, "name": "Artist"}],
  "label": {"id": 222, "name": "Label"},
  "catalog_number": "CAT001",
  "upc": "123456789012",
  "date": "2026-01-20",
  "track_count": 4,
  "bpm_range": {"min": 124, "max": 130},
  "image": {"url": "https://..."}
}
```

**Release Tracks**
```
GET /v4/catalog/releases/{id}/tracks/
Authorization: Bearer {token}
```

### 3.5 Playlists & DJ Charts

**Get Playlist**
```
GET /v4/catalog/playlists/{id}/
Authorization: Bearer {token}

Response:
{
  "id": 333,
  "name": "My Playlist",
  "track_count": 25,
  "genres": [{"id": 1, "name": "House"}],
  "bpm_range": {"min": 120, "max": 130},
  "length_ms": 5400000,
  "created_date": "2026-01-15",
  "updated_date": "2026-01-24"
}
```

**Playlist Tracks**
```
GET /v4/catalog/playlists/{id}/tracks/?page=1
Authorization: Bearer {token}

Response includes position field for ordering:
{
  "results": [
    {"position": 1, "track": {track_object}},
    {"position": 2, "track": {track_object}}
  ]
}
```

### 3.6 Search

**Search Tracks**
```
GET /v4/catalog/search/?q={query}&type=tracks&page=1&per_page=50
Authorization: Bearer {token}

Additional filters:
  &genre_id={id}
  &bpm_min={min}&bpm_max={max}
  &key={camelot_notation}
  &is_available_for_streaming=true
  &order_by=-publish_date
```

---

## 4. TRACK METADATA STRUCTURE

### Complete Track Object (from API)
```json
{
  "id": 456,
  "name": "Track Title",
  "mix_name": "Original Mix",
  "slug": "track-title-original-mix",
  "number": 1,
  "length": "5:45",
  "length_ms": 345000,
  "bpm": 128,
  "key": {
    "name": "A minor",
    "camelot": "8A",
    "openkey": "1m"
  },
  "genre": {"id": 1, "name": "House", "slug": "house"},
  "subgenre": {"id": 11, "name": "Deep House", "slug": "deep-house"},
  "isrc": "USRC12345678",
  "artists": [
    {"id": 789, "name": "Artist Name", "slug": "artist-name"}
  ],
  "remixers": [
    {"id": 790, "name": "Remixer Name", "slug": "remixer-name"}
  ],
  "release": {
    "id": 111,
    "name": "Release Name",
    "slug": "release-name"
  },
  "label": {
    "id": 222,
    "name": "Label Name",
    "slug": "label-name"
  },
  "publish_date": "2026-01-20",
  "images": {
    "small": {"url": "https://..."},
    "medium": {"url": "https://..."},
    "large": {"url": "https://..."}
  },
  "url": "https://www.beatport.com/track/track-title/456"
}
```

### Mapping to Mixxx Library Fields
| Beatport Field | Mixxx Field | Notes |
|---------------|-------------|-------|
| `id` | `remote_id` | Service-specific identifier |
| `name` + `mix_name` | `title` | Concatenate: "Track (Original Mix)" |
| `artists[].name` | `artist` | Join with ", " |
| `bpm` | `bpm` | Direct mapping |
| `key.camelot` | `key` | Or convert from key.name |
| `length_ms` | `duration` | Convert to seconds |
| `genre.name` | `genre` | Direct mapping |
| `label.name` | `album` | Convention for DJ software |
| `images.medium.url` | `coverart_url` | For cover display |
| `publish_date` | `year` | Extract year portion |
| `isrc` | `comment` | Optional storage |

---

## 5. STREAMING ENDPOINT DETAILS

### Get Stream URL
```
GET /v4/catalog/tracks/{id}/stream/
Authorization: Bearer {token}

Response:
{
  "stream_url": "https://needledrop.beatport.com/{timestamp}_{hash}/{path}/{uuid}.128k.aac.m3u8?start=0&end={duration_ms}",
  "sample_start_ms": 0,
  "sample_end_ms": 345000
}
```

### HLS Manifest Structure
```
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-PLAYLIST-TYPE:VOD
#EXT-X-TARGETDURATION:6
#EXT-X-KEY:METHOD=AES-128,URI="{uuid}.128k.aac.key?start=0&end={ms}",IV=0x{hex}
#EXTINF:5.015510,
{uuid}.128k.aac-1.aac?start=0&end={ms}
#EXTINF:5.015510,
{uuid}.128k.aac-2.aac?start=0&end={ms}
...
#EXT-X-ENDLIST
```

### Decryption Details
- **Algorithm:** AES-128-CBC
- **Key:** Fetch from URI in manifest (16 bytes)
- **IV:** Hex value after `0x` in `#EXT-X-KEY`
- **Padding:** PKCS#7

### URL Expiration
- Stream URLs expire after ~1 hour (timestamp-based signing)
- On 403/410 response: Re-fetch `/stream/` endpoint for fresh URL
- Implementation: FuseDriver triggers refresh callback → BridgeClient → API → update URL

---

## 6. BEATPORTDL PATTERNS TO LEVERAGE

### Pattern 1: Generic Pagination Handler
```go
// From beatportdl - adapt to C++/Qt
func ForPaginated[T any](
    entityId int64,
    params string,
    fetchPage func(id int64, page int, params string) (*Paginated[T], error),
    processItem func(item T, i int) error,
) error
```

**C++ Equivalent:**
```cpp
template<typename T>
void fetchAllPages(
    int entityId,
    std::function<QNetworkReply*(int id, int page)> fetchPage,
    std::function<void(const T& item, int index)> processItem
);
```

### Pattern 2: Lazy Full Track Fetching
- Paginated responses return minimal track data
- Full track fetched via `GET /tracks/{id}` when needed (e.g., for streaming)
- Reduces API bandwidth for browsing

### Pattern 3: Token Refresh on 401
```cpp
// Intercept all API responses
if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
    refreshToken();
    retryRequest(originalRequest);
}
```

### Pattern 4: Store Abstraction
beatportdl supports both Beatport and Beatsource with same codebase:
```cpp
enum class Store { Beatport, Beatsource };
QString baseUrl(Store s) {
    return s == Store::Beatport
        ? "https://api.beatport.com/v4"
        : "https://api.beatsource.com/v4";
}
```

---

## 7. IMPLEMENTATION ROADMAP

### Phase A: Catalog Data Wiring (IMMEDIATE - 2-3 hours)

**Goal:** Populate TreeItemModel with browsable catalog

**Files to Modify:**
- `src/streaming/hook/beatportservice.cpp` - Add genre/chart fetch methods
- `src/streaming/hook/beatportfeature.cpp` - Wire tree model population

**Steps:**
1. Add `getGenres()` method to BeatportService
2. Add `getGenreTracks(genreId)` method
3. Add `getCharts()` method
4. Add `getChartTracks(chartId)` method
5. Modify BeatportFeature to call these on activation
6. Populate TreeItemModel with results

**API Calls Needed:**
```
GET /v4/catalog/genres/           → Populate genre list
GET /v4/catalog/genres/{id}/top/100/  → Genre's top tracks
GET /v4/catalog/charts/           → Populate chart list
GET /v4/catalog/charts/{id}/tracks/   → Chart tracks
```

### Phase B: Entity Pivot (2 hours)

**Goal:** Click artist/label in track list → see their catalog

**Steps:**
1. Add `getArtistTracks(artistId)` method
2. Add `getLabelReleases(labelId)` method
3. Wire context menu or link clicks in track list
4. Navigate TreeItemModel to artist/label view

### Phase C: Search Enhancement (1 hour)

**Goal:** Genre/BPM/Key filters on search

**Steps:**
1. Extend SearchQuery DTO with filter fields
2. Modify search API call to include filter params
3. Add filter UI controls to StreamBrowserWidget

### Phase D: Playlist Support (1 hour)

**Goal:** Browse and play user playlists

**Steps:**
1. Add `getMyPlaylists()` method
2. Add `getPlaylistTracks(playlistId)` method
3. Add "My Playlists" node to TreeItemModel

---

## 8. RECOMMENDED TREE STRUCTURE

```
Beatport
├── Genres
│   ├── House
│   │   ├── Top 100
│   │   ├── Deep House (subgenre)
│   │   ├── Tech House (subgenre)
│   │   └── ...
│   ├── Techno
│   │   ├── Top 100
│   │   ├── Melodic Techno
│   │   └── ...
│   └── ...
├── Charts
│   ├── Beatport Top 100
│   ├── Top 100 Releases
│   ├── Hype Charts
│   └── ...
├── My Playlists
│   ├── Playlist 1
│   └── Playlist 2
├── My Library (purchased/saved)
└── Search Results
```

---

## 9. DATA TRANSFER OBJECTS

### Existing (in streamingdto.h)
```cpp
struct TrackMetadata {
    QString remoteId;
    QString title;
    QString artist;
    int bpm;
    QString key;
    int durationMs;
    QString coverArtUrl;
};

struct StreamInfo {
    QString streamUrl;
    QString format;  // "hls"
    int bitrate;
    int durationMs;
};

struct SearchQuery {
    QString query;
    QString service;
    int limit;
};
```

### Needed Additions
```cpp
struct Genre {
    int id;
    QString name;
    QString slug;
    QList<Genre> subgenres;  // For nested display
};

struct Chart {
    int id;
    QString name;
    QString slug;
    int trackCount;
    QString imageUrl;
    QDate publishDate;
};

struct Artist {
    int id;
    QString name;
    QString slug;
    QString imageUrl;
};

struct Label {
    int id;
    QString name;
    QString slug;
};

struct Release {
    int id;
    QString name;
    QString catalogNumber;
    QDate releaseDate;
    QString imageUrl;
    int trackCount;
};

struct Playlist {
    int id;
    QString name;
    int trackCount;
    int lengthMs;
    QDate createdDate;
    QDate updatedDate;
};
```

---

## 10. CRITICAL FIXES REQUIRED

### Fix 1: Stream Endpoint (If Not Already Done)
**Location:** `beatportservice.cpp:~326`
```cpp
// WRONG:
QString url = baseUrl + "/catalog/tracks/" + trackId + "/download";

// CORRECT:
QString url = baseUrl + "/catalog/tracks/" + trackId + "/stream/";
```

### Fix 2: Response Parsing
```cpp
// WRONG: Expects direct URL
QString streamUrl = doc["url"].toString();

// CORRECT: Parse HLS manifest URL from response
QJsonObject json = doc.object();
QString streamUrl = json["stream_url"].toString();
int durationMs = json["sample_end_ms"].toInt();
```

### Fix 3: HLS Handling
```cpp
// Current code may expect direct audio file
// Need to either:
// A) Parse M3U8 and fetch segments (proper HLS)
// B) Quick hack: Replace .m3u8 with -1.aac for single segment (~5s preview)
```

---

## 11. TESTING CHECKLIST

### Authentication
- [ ] Login with valid credentials → Token acquired
- [ ] Token stored in system keyring
- [ ] Token refresh triggers 5 min before expiry
- [ ] 401 response triggers automatic refresh

### Catalog Browsing
- [ ] Genres list populates on activation
- [ ] Genre selection shows Top 100 tracks
- [ ] Charts list populates
- [ ] Chart selection shows tracks
- [ ] Artist/Label links navigable

### Streaming
- [ ] Track selection calls `/stream/` endpoint
- [ ] HLS URL received and parsed
- [ ] FUSE mount created
- [ ] Audio plays through Mixxx engine
- [ ] URL refresh works on 403

### Error Handling
- [ ] Network errors display in notifier
- [ ] Auth errors prompt re-login
- [ ] Rate limiting shows retry countdown

---

## 12. FILE LOCATIONS REFERENCE

### Core Implementation
```
src/streaming/hook/
├── beatportservice.h/.cpp    # API client (ADD catalog methods here)
├── beatportfeature.h/.cpp    # Sidebar tree (WIRE catalog data here)
├── oauthmanager.h/.cpp       # Token lifecycle
├── tokeninterceptor.h/.cpp   # Auth header injection
├── streamingdto.h            # Data structures (ADD new DTOs here)
└── streamingservice.h/.cpp   # Abstract base
```

### Bridge/FUSE
```
src/streaming/bridge/
├── bridgeclient.h/.cpp       # IPC client
├── fusedriver.h/.cpp         # FUSE operations
├── rangefetcher.h/.cpp       # HTTP range requests
└── sparsecache.h/.cpp        # Segment caching
```

### UI
```
src/streaming/ui/
├── logindialog.h/.cpp        # Credentials entry
├── streambrowserwidget.h/.cpp # Search interface
└── streamingerrornotifier.h/.cpp # Error display
```

### Reference Implementation
```
docs/BeatPort/beatportdl/
├── internal/beatport/auth.go      # Auth pattern reference
├── internal/beatport/genres.go    # Genre endpoint reference
├── internal/beatport/charts.go    # Charts endpoint reference
├── internal/beatport/tracks.go    # Track/stream endpoints
├── internal/beatport/artists.go   # Artist endpoints
├── internal/beatport/labels.go    # Label endpoints
├── internal/beatport/playlists.go # Playlist endpoints
└── cmd/beatportdl/needledrop.go   # HLS decryption reference
```

---

## 13. QUICK REFERENCE CARD

### API Base
```
https://api.beatport.com/v4
```

### Auth Header
```
Authorization: Bearer {access_token}
```

### Key Endpoints for Browsing
```
GET /catalog/genres/              # All genres
GET /catalog/genres/{id}/top/100/ # Genre tracks
GET /catalog/charts/              # All charts
GET /catalog/charts/{id}/tracks/  # Chart tracks
GET /catalog/artists/{id}/tracks/ # Artist tracks
GET /catalog/labels/{id}/releases/# Label releases
GET /catalog/playlists/{id}/tracks/ # Playlist tracks
GET /catalog/search/?q=...        # Search
```

### Key Endpoint for Streaming
```
GET /catalog/tracks/{id}/stream/  # Returns HLS manifest URL
```

### Pagination Pattern
```
?page=1&per_page=50
Response: { "next": "...?page=2", "count": 100, "results": [...] }
```

---

## 14. CONTEXT PRESERVATION NOTES

### What Changed Recently (Session 8)
1. OAuth password grant flow implemented and verified working
2. Stream endpoint confirmed returning full-track HLS (not previews)
3. Error handling system complete with deduplication
4. URL refresh mechanism implemented

### What Needs Attention Tonight
1. **Catalog Data Wiring** - The sidebar shows "Beatport" but no content
2. **Genre/Chart Endpoints** - API works, need to call them
3. **TreeItemModel Population** - Need to display results

### Architecture Invariants (DO NOT VIOLATE)
1. EngineWorker thread performs ZERO network I/O
2. All API calls go through Hook layer (StreamingService)
3. OAuth tokens stored in system keyring only
4. Stream URLs never persisted to database

---

*Document generated from automated codebase analysis. See SESSION_SNAPSHOT_2026-01-24.md for implementation history.*
