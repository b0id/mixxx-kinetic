# Mixxx-Kinetic Implementation Guide for Agentic Coders

**Date**: 2026-01-24
**Purpose**: Provide TOTAL CLARITY for implementing streaming integration
**Target**: AI agents performing iterative implementation

---

## Executive Summary

### Current State: Pre-Alpha, ~35% Complete

The codebase has:
- OAuth framework (works)
- BeatportService (broken async, wrong endpoint)
- FUSE/Bridge architecture (code exists, not integrated)
- Library feature (empty shell)

### What's Missing for MVP:
1. Fix BeatportService async pattern (QFuture actually returns values)
2. Implement HLS streaming (use patterns from beatportdl)
3. Wire sidebar to display track tables
4. Mount FUSE filesystem

---

## Part 1: Critical Files Reference

### Beatport API (VERIFIED WORKING)
Read these documents BEFORE implementing:
- `docs/BeatPort/VERIFIED_STREAMING_API.md` - Complete tested flow
- `docs/BeatPort/CONSOLIDATED_API_REFERENCE.md` - All endpoints

### Key Implementation Patterns (from beatportdl)
Source: `/home/b0id/AI-sandbox/mixxx-kinetic/mixxx-kinetic/docs/BeatPort/beatportdl/`

| File | What It Teaches |
|------|-----------------|
| `internal/beatport/auth.go` | Session cookie OAuth flow |
| `cmd/beatportdl/needledrop.go` | HLS parsing + AES decryption |
| `internal/beatport/tracks.go` | Track/stream endpoint calls |

---

## Part 2: Authentication Implementation

### The Correct Flow (from beatportdl auth.go)

```
┌─────────────────────────────────────────────────────────────┐
│ STEP 1: Login                                               │
│ POST https://api.beatport.com/v4/auth/login/                │
│ Body: {"username": "...", "password": "..."}                │
│ Content-Type: application/json                              │
│ Response: Sets 'sessionid' cookie                           │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│ STEP 2: Authorize                                           │
│ GET https://api.beatport.com/v4/auth/o/authorize/           │
│     ?client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ     │
│     &response_type=code                                     │
│ Cookie: sessionid={from_step_1}                             │
│ Response: 302 redirect with Location containing ?code=XXX   │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│ STEP 3: Token Exchange                                      │
│ POST https://api.beatport.com/v4/auth/o/token/              │
│ Body: client_id=...&grant_type=authorization_code&code=XXX  │
│ Content-Type: application/x-www-form-urlencoded             │
│ Response: {"access_token": "...", "refresh_token": "...",   │
│            "expires_in": 36000}                             │
└─────────────────────────────────────────────────────────────┘
```

### C++ Implementation Pattern

```cpp
// In BeatportService::authenticate()
void BeatportService::authenticate(const QString& username, const QString& password) {
    // Step 1: Login
    QUrl loginUrl(m_baseUrl + "/auth/login/");
    QNetworkRequest loginReq(loginUrl);
    loginReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject loginBody;
    loginBody["username"] = username;
    loginBody["password"] = password;

    QNetworkReply* loginReply = m_nam->post(loginReq, QJsonDocument(loginBody).toJson());
    connect(loginReply, &QNetworkReply::finished, [this, loginReply]() {
        // Extract sessionid cookie
        QList<QNetworkCookie> cookies = m_nam->cookieJar()->cookiesForUrl(loginReply->url());
        QString sessionId;
        for (const auto& cookie : cookies) {
            if (cookie.name() == "sessionid") {
                sessionId = cookie.value();
                break;
            }
        }

        if (sessionId.isEmpty()) {
            emit authError("beatport", "Login failed - no session cookie");
            return;
        }

        // Step 2: Authorize (get code from redirect)
        authorizeWithSession(sessionId);
    });
}
```

### Token Refresh (CRITICAL - tokens expire in 10 hours)

```cpp
void BeatportService::refreshToken() {
    QUrl tokenUrl(m_baseUrl + "/auth/o/token/");
    QNetworkRequest req(tokenUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray body;
    body.append("client_id=ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ");
    body.append("&grant_type=refresh_token");
    body.append("&refresh_token=" + m_refreshToken.toUtf8());

    QNetworkReply* reply = m_nam->post(req, body);
    // ... handle response, update tokens
}
```

---

## Part 3: Streaming Implementation

### The Problem in Current Code

Current `beatportservice.cpp:326`:
```cpp
// WRONG - uses download endpoint
QUrl url(QString("%1/catalog/tracks/%2/download").arg(m_config.baseUrl, trackId));
```

### The Fix

```cpp
// CORRECT - use stream endpoint
QUrl url(QString("%1/catalog/tracks/%2/stream/").arg(m_baseUrl, trackId));
// No quality parameter needed - streaming is always 128k HLS
```

### HLS Parsing (from needledrop.go)

The stream endpoint returns:
```json
{
  "stream_url": "https://needledrop.beatport.com/.../file.m3u8?start=0&end=256463",
  "sample_start_ms": 0,
  "sample_end_ms": 256463
}
```

You must:
1. Fetch the M3U8 manifest
2. Parse `#EXT-X-KEY` for encryption key URI and IV
3. Fetch the AES-128 key (16 bytes)
4. For each segment: fetch, decrypt with AES-128-CBC, concatenate

### C++ HLS Implementation

```cpp
struct HlsSegment {
    QString url;
    double duration;
};

struct HlsKey {
    QByteArray key;    // 16 bytes
    QByteArray iv;     // 16 bytes
};

class HlsParser {
public:
    bool parse(const QByteArray& m3u8Content, const QUrl& baseUrl) {
        QTextStream stream(m3u8Content);
        QString line;

        while (stream.readLineInto(&line)) {
            if (line.startsWith("#EXT-X-KEY:")) {
                // Parse: METHOD=AES-128,URI="file.key",IV=0x...
                parseKeyLine(line, baseUrl);
            } else if (line.startsWith("#EXTINF:")) {
                // Parse duration
                m_currentDuration = line.mid(8).split(',').first().toDouble();
            } else if (!line.startsWith("#") && !line.isEmpty()) {
                // Segment URL
                HlsSegment seg;
                seg.url = baseUrl.resolved(QUrl(line)).toString();
                seg.duration = m_currentDuration;
                m_segments.append(seg);
            }
        }
        return !m_segments.isEmpty();
    }

    QVector<HlsSegment> segments() const { return m_segments; }
    HlsKey key() const { return m_key; }

private:
    void parseKeyLine(const QString& line, const QUrl& baseUrl) {
        // Extract URI and IV from #EXT-X-KEY line
        QRegularExpression uriRe(R"(URI="([^"]+)")");
        QRegularExpression ivRe(R"(IV=0x([0-9a-fA-F]+))");

        auto uriMatch = uriRe.match(line);
        auto ivMatch = ivRe.match(line);

        if (uriMatch.hasMatch()) {
            m_keyUrl = baseUrl.resolved(QUrl(uriMatch.captured(1)));
        }
        if (ivMatch.hasMatch()) {
            m_key.iv = QByteArray::fromHex(ivMatch.captured(1).toUtf8());
        }
    }

    QVector<HlsSegment> m_segments;
    HlsKey m_key;
    QUrl m_keyUrl;
    double m_currentDuration = 0;
};
```

### AES-128-CBC Decryption

```cpp
#include <openssl/evp.h>

QByteArray decryptSegment(const QByteArray& encrypted,
                          const QByteArray& key,
                          const QByteArray& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.constData()),
                       reinterpret_cast<const unsigned char*>(iv.constData()));

    QByteArray decrypted(encrypted.size() + EVP_CIPHER_block_size(EVP_aes_128_cbc()), 0);
    int outLen = 0;
    int totalLen = 0;

    EVP_DecryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(decrypted.data()),
                      &outLen,
                      reinterpret_cast<const unsigned char*>(encrypted.constData()),
                      encrypted.size());
    totalLen = outLen;

    EVP_DecryptFinal_ex(ctx,
                        reinterpret_cast<unsigned char*>(decrypted.data()) + outLen,
                        &outLen);
    totalLen += outLen;

    EVP_CIPHER_CTX_free(ctx);
    decrypted.resize(totalLen);
    return decrypted;
}
```

---

## Part 4: Sidebar/Library Integration

### Current Problem

`BeatportFeature` only creates an empty tree. No track display.

### Required Architecture

```
LibraryPane (QStackedWidget)
├── WTrackTableView (local library)
└── StreamingTrackTableView (NEW - for remote tracks)
    ├── StreamingTrackModel (QAbstractTableModel)
    │   └── QVector<TrackMetadata> m_tracks
    └── StreamingTableDelegate
        └── CoverArtDelegate (for thumbnails)
```

### Implementation Steps

1. **Create StreamingTrackModel** (`src/library/streaming/streamingtackmodel.h`):

```cpp
class StreamingTrackModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Column {
        COLUMN_COVER = 0,
        COLUMN_TITLE,
        COLUMN_ARTIST,
        COLUMN_BPM,
        COLUMN_KEY,
        COLUMN_GENRE,
        COLUMN_DURATION,
        COLUMN_LABEL,
        NUM_COLUMNS
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return m_tracks.size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return NUM_COLUMNS;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) return QVariant();

        const TrackMetadata& track = m_tracks[index.row()];

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case COLUMN_TITLE: return track.title;
                case COLUMN_ARTIST: return track.artist;
                case COLUMN_BPM: return QString::number(track.bpm);
                case COLUMN_KEY: return track.key;
                case COLUMN_GENRE: return track.genre;
                case COLUMN_DURATION: return formatDuration(track.durationMs);
                case COLUMN_LABEL: return track.label;
            }
        } else if (role == Qt::DecorationRole && index.column() == COLUMN_COVER) {
            return m_coverCache.value(track.remoteId);
        }

        return QVariant();
    }

    void setTracks(const QVector<TrackMetadata>& tracks) {
        beginResetModel();
        m_tracks = tracks;
        endResetModel();
        // Trigger cover art loading
        for (const auto& track : tracks) {
            loadCoverArt(track.coverArtUrl, track.remoteId);
        }
    }

private:
    QVector<TrackMetadata> m_tracks;
    QHash<QString, QPixmap> m_coverCache;
};
```

2. **Wire to BeatportFeature**:

```cpp
void BeatportFeature::activate() {
    // Create streaming view if needed
    if (!m_pStreamingView) {
        m_pStreamingView = new StreamingTrackTableView(m_pTrackCollection);
        m_pStreamingModel = new StreamingTrackModel(this);
        m_pStreamingView->setModel(m_pStreamingModel);
    }

    // Show in library pane
    switchToPane(m_pStreamingView);
}

void BeatportFeature::onSearchResults(const QVector<TrackMetadata>& tracks) {
    m_pStreamingModel->setTracks(tracks);
}
```

### Traktor-Style Genre/Chart Browsing

Traktor shows Top 100 per genre. Implement similarly:

```cpp
class BeatportFeature : public LibraryFeature {
    void buildTree() {
        TreeItem* root = new TreeItem(this, "Beatport");
        TreeItem* search = new TreeItem(root, "Search");
        TreeItem* charts = new TreeItem(root, "Charts");
        TreeItem* genres = new TreeItem(root, "Genres");

        // Fetch genres from API
        // GET /v4/catalog/genres/
        for (const auto& genre : m_genres) {
            TreeItem* genreItem = new TreeItem(genres, genre.name);
            genreItem->setData(QVariant::fromValue(genre.id));

            // Add Top 100 under each genre
            TreeItem* top100 = new TreeItem(genreItem, "Top 100");
            top100->setData(QVariant::fromValue(
                QString("/v4/catalog/genres/%1/top/100/").arg(genre.id)));
        }

        setRootItem(root);
    }

    void activateChild(const QModelIndex& index) {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        QString endpoint = item->data().toString();

        if (endpoint.contains("/top/100/")) {
            // Fetch top 100 for this genre
            m_pService->fetchChart(endpoint);
        }
    }
};
```

---

## Part 5: Multi-Service Strategy

### Priority Order: Beatport → Tidal → SoundCloud

| Service | Auth Type | API Status | Priority |
|---------|-----------|------------|----------|
| Beatport | Session Cookie OAuth | VERIFIED WORKING | 1 (NOW) |
| Tidal | Device Flow OAuth | Developer access available | 2 (NEXT) |
| SoundCloud | OAuth 2.0 | Public API deprecated, reverse engineer | 3 (LATER) |

### Beatport Implementation (NOW)

You have everything needed:
- Auth flow documented and tested
- Stream endpoint verified (full tracks, not previews)
- HLS decryption pattern from beatportdl

**Implementation order:**
1. Fix `BeatportService::authenticate()` to use session cookie flow
2. Fix `BeatportService::getStreamInfo()` to use `/stream/` endpoint
3. Implement HLS parser and decryptor
4. Wire to sidebar with track table

### Tidal Implementation (NEXT)

You mention developer access. Tidal uses Device Flow:

```
1. POST /v1/oauth2/device_authorization
   Response: {"deviceCode": "...", "userCode": "ABC123", "verificationUri": "link.tidal.com/..."}

2. User visits link, enters code

3. Poll POST /v1/oauth2/token with device_code until success

4. GET /v1/tracks/{id}/playbackinfopostpaywall
   Response: Contains stream URLs (may require Widevine for some content)
```

**Key difference**: Tidal may use Widevine DRM for some content, requiring a separate CDM process.

### SoundCloud Implementation (LATER)

SoundCloud's public API was deprecated. Options:
1. Use their embed player API (limited)
2. Reverse engineer web player (risky, may break)
3. Wait for potential API reinstatement

**Recommend deferring** until Beatport + Tidal are solid.

---

## Part 6: Testing Methodology

### Test-Driven Enumeration Pattern

When implementing, follow this pattern:

```bash
# 1. Test endpoint manually first
TOKEN="your_token"
curl -H "Authorization: Bearer $TOKEN" \
  "https://api.beatport.com/v4/catalog/tracks/23050990/stream/" | jq

# 2. Verify response matches expectation
# 3. Implement in code
# 4. Write unit test with mock response
# 5. Run integration test against real API
```

### Mock Responses for Unit Tests

Create `test/fixtures/beatport/`:
```
stream_response.json
search_response.json
track_metadata.json
hls_manifest.m3u8
```

### Integration Test Pattern

```cpp
class BeatportServiceTest : public QObject {
    Q_OBJECT
private slots:
    void testAuthentication() {
        // Requires real credentials - skip in CI
        if (qEnvironmentVariableIsEmpty("BEATPORT_TEST_USER")) {
            QSKIP("No test credentials");
        }

        BeatportService service;
        QSignalSpy authSpy(&service, &BeatportService::authenticated);

        service.authenticate(
            qEnvironmentVariable("BEATPORT_TEST_USER"),
            qEnvironmentVariable("BEATPORT_TEST_PASS"));

        QVERIFY(authSpy.wait(10000));
        QVERIFY(service.hasValidToken());
    }

    void testGetStreamInfo() {
        // Assumes authenticated
        QSignalSpy streamSpy(&service, &BeatportService::streamInfoReceived);

        service.getStreamInfo("23050990");

        QVERIFY(streamSpy.wait(10000));
        auto args = streamSpy.first();
        StreamInfo info = args.at(0).value<StreamInfo>();

        QVERIFY(info.streamUrl.contains(".m3u8"));
        QCOMPARE(info.format, "aac");
    }
};
```

---

## Part 7: What Enlightened the Analysis

### From beatportdl

1. **auth.go lines 17-21**: The hardcoded client ID that actually works
2. **auth.go lines 118-132**: The three-step flow (login → authorize → token)
3. **auth.go lines 195-216**: How to extract auth code from redirect
4. **needledrop.go lines 28-77**: Complete HLS parsing including key/IV extraction
5. **needledrop.go lines 79-89**: AES-128-CBC decryption with PKCS#7 padding

### From Live API Testing

1. **Token refresh works** with beatportdl's credentials
2. **`/stream/` endpoint returns FULL TRACKS** (sample_end_ms = track duration)
3. **HLS manifest is publicly accessible** once you have the URL
4. **Same token works for both `/download/` and `/stream/`**

### From Codebase Analysis

1. **QFuture pattern is broken** - methods return empty futures, use signals
2. **FUSE is implemented but not mounted** - `/tmp/mountpoint/` assumed to exist
3. **Sidebar feature is empty shell** - no track table wired up
4. **Only Beatport has code** - Tidal/SoundCloud are just placeholders

---

## Part 8: Immediate Action Items

### For Beatport MVP (Priority 1)

1. **Fix BeatportService auth** (`src/streaming/hook/beatportservice.cpp`)
   - Replace PKCE flow with session cookie flow
   - Use client ID: `ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ`

2. **Fix stream endpoint** (`beatportservice.cpp:326`)
   - Change `/download` to `/stream/`
   - Parse JSON response for `stream_url`

3. **Implement HLS handler** (new file: `src/streaming/hls/hlsparser.cpp`)
   - Parse M3U8 manifest
   - Fetch and cache AES key
   - Decrypt segments on demand

4. **Wire sidebar** (`src/library/beatport/beatportfeature.cpp`)
   - Create `StreamingTrackModel`
   - Display search results in table view
   - Add genre tree with Top 100

### For Tidal (Priority 2)

1. Create `TidalService` class mirroring `BeatportService`
2. Implement Device Flow OAuth
3. Handle Widevine if needed (may require external CDM)

### For SoundCloud (Priority 3)

Defer until Beatport + Tidal working.

---

## Appendix: Key Constants

```cpp
// Beatport
const QString BEATPORT_BASE_URL = "https://api.beatport.com/v4";
const QString BEATPORT_CLIENT_ID = "ryZ8LuyQVPqbK2mBX2Hwt4qSMtnWuTYSqBPO92yQ";
const int BEATPORT_TOKEN_LIFETIME_SEC = 36000;  // 10 hours
const int BEATPORT_TOKEN_REFRESH_BUFFER_SEC = 300;  // 5 minutes

// Needledrop (HLS streaming)
const QString NEEDLEDROP_HOST = "needledrop.beatport.com";
const int HLS_SEGMENT_DURATION_SEC = 5;
const int HLS_ENCRYPTION_KEY_SIZE = 16;  // AES-128
```

---

## Appendix: File Locations

| What | Where |
|------|-------|
| BeatportService | `src/streaming/hook/beatportservice.cpp` |
| OAuthManager | `src/streaming/hook/oauthmanager.cpp` |
| BeatportFeature | `src/library/beatport/beatportfeature.cpp` |
| FUSE Driver | `src/streaming/bridge/fusedriver.cpp` |
| Sparse Cache | `src/streaming/bridge/sparsecache.cpp` |
| Sound Source Proxy | `src/sources/soundsourcekineticproxy.cpp` |
| API Reference | `docs/BeatPort/CONSOLIDATED_API_REFERENCE.md` |
| Verified Flow | `docs/BeatPort/VERIFIED_STREAMING_API.md` |
