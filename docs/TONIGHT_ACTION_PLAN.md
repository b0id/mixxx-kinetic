# Tonight's Action Plan
## Beatport Catalog Browsing Implementation

**Date:** 2026-01-24 Evening Session
**Priority:** Wire catalog data to UI for browsing experience
**Prerequisite:** Authentication is VERIFIED WORKING

---

## THE ONE BLOCKER

**BLOCKER-003:** Users see "Beatport" in sidebar but cannot browse tracks.

**Root Cause:** `BeatportFeature` doesn't call API to populate `TreeItemModel`.

**Fix Location:** `src/streaming/hook/beatportfeature.cpp`

---

## HOUR 1: Genre Browsing

### Step 1.1: Add Genre Fetch to BeatportService

**File:** `src/streaming/hook/beatportservice.h`
```cpp
// Add declarations
QFuture<QList<Genre>> getGenres();
QFuture<QList<TrackMetadata>> getGenreTracks(int genreId, int page = 1);
```

**File:** `src/streaming/hook/beatportservice.cpp`
```cpp
QFuture<QList<Genre>> BeatportService::getGenres() {
    return QtConcurrent::run([this]() {
        QList<Genre> genres;
        QUrl url(m_baseUrl + "/catalog/genres/");
        QNetworkRequest request(url);
        request.setRawHeader("Authorization", "Bearer " + m_accessToken.toUtf8());

        QNetworkReply* reply = m_networkManager->get(request);
        // ... parse JSON response into Genre list
        return genres;
    });
}

QFuture<QList<TrackMetadata>> BeatportService::getGenreTracks(int genreId, int page) {
    return QtConcurrent::run([this, genreId, page]() {
        QList<TrackMetadata> tracks;
        QString endpoint = QString("/catalog/genres/%1/top/100/?page=%2").arg(genreId).arg(page);
        // ... fetch and parse
        return tracks;
    });
}
```

### Step 1.2: Wire to BeatportFeature

**File:** `src/streaming/hook/beatportfeature.cpp`
```cpp
void BeatportFeature::activate() {
    // On activation, fetch genres and populate tree
    auto future = m_service->getGenres();
    auto watcher = new QFutureWatcher<QList<Genre>>(this);
    connect(watcher, &QFutureWatcher<QList<Genre>>::finished, [this, watcher]() {
        auto genres = watcher->result();
        populateGenreTree(genres);
        watcher->deleteLater();
    });
    watcher->setFuture(future);
}

void BeatportFeature::populateGenreTree(const QList<Genre>& genres) {
    // Clear existing
    m_childModel.clear();

    // Create "Genres" parent node
    auto* genresNode = new TreeItem(this, "Genres");
    for (const auto& genre : genres) {
        auto* genreItem = new TreeItem(genresNode, genre.name);
        genreItem->setData(genre.id);  // Store ID for later fetch
    }
    m_childModel.appendRow(genresNode);
}
```

### Step 1.3: Handle Genre Selection

```cpp
void BeatportFeature::onGenreSelected(const QModelIndex& index) {
    int genreId = index.data(Qt::UserRole).toInt();
    auto future = m_service->getGenreTracks(genreId);
    // ... display tracks in right panel
}
```

---

## HOUR 2: Charts Browsing

### Step 2.1: Add Chart Fetch

**File:** `src/streaming/hook/beatportservice.cpp`
```cpp
QFuture<QList<Chart>> BeatportService::getCharts() {
    // GET /catalog/charts/
}

QFuture<QList<TrackMetadata>> BeatportService::getChartTracks(int chartId, int page) {
    // GET /catalog/charts/{id}/tracks/?page={page}
}
```

### Step 2.2: Add Charts Node to Tree

```cpp
void BeatportFeature::populateChartsTree(const QList<Chart>& charts) {
    auto* chartsNode = new TreeItem(this, "Charts");
    for (const auto& chart : charts) {
        auto* chartItem = new TreeItem(chartsNode, chart.name);
        chartItem->setData(chart.id);
    }
    m_childModel.appendRow(chartsNode);
}
```

---

## HOUR 3: Track Display & Streaming

### Step 3.1: Display Track List

When user clicks a genre or chart, populate the track table:

```cpp
void BeatportFeature::displayTracks(const QList<TrackMetadata>& tracks) {
    // Emit signal to track table model
    emit tracksLoaded(tracks);
}
```

### Step 3.2: Handle Track Selection for Streaming

```cpp
void BeatportFeature::onTrackDoubleClicked(const QModelIndex& index) {
    QString trackId = index.data(Qt::UserRole).toString();

    // Get stream info
    auto future = m_service->getStreamInfo(trackId);
    auto watcher = new QFutureWatcher<StreamInfo>(this);
    connect(watcher, &QFutureWatcher<StreamInfo>::finished, [this, watcher, trackId]() {
        StreamInfo info = watcher->result();

        // Create manifest for FUSE
        ManifestGenerator gen;
        QString manifestPath = gen.generate(trackId, info);

        // Load to deck
        emit loadTrackToDeck(manifestPath);

        watcher->deleteLater();
    });
    watcher->setFuture(future);
}
```

---

## API QUICK REFERENCE

```
# Genres
GET /v4/catalog/genres/                    → List all genres
GET /v4/catalog/genres/{id}/top/100/       → Genre's Top 100

# Charts
GET /v4/catalog/charts/                    → List all charts
GET /v4/catalog/charts/{id}/tracks/        → Chart tracks

# Streaming
GET /v4/catalog/tracks/{id}/stream/        → HLS URL

# Auth header for all requests:
Authorization: Bearer {access_token}
```

---

## DATA STRUCTURES TO ADD

**File:** `src/streaming/hook/streamingdto.h`

```cpp
struct Genre {
    int id;
    QString name;
    QString slug;
    QList<Genre> subgenres;
};

struct Chart {
    int id;
    QString name;
    QString slug;
    int trackCount;
    QString imageUrl;
};
```

---

## TESTING SEQUENCE

1. **Build & Run**
   ```bash
   cd /home/b0id/repo/build
   cmake .. && make -j4
   ./mixxx
   ```

2. **Login Test**
   - Preferences → Streaming → Login for Beatport
   - Enter credentials
   - Verify "Logged in" status

3. **Browse Test**
   - Click "Beatport" in sidebar
   - Verify genres appear
   - Click a genre
   - Verify tracks appear in right panel

4. **Stream Test**
   - Double-click a track
   - Verify audio plays

---

## IF STUCK

### Genre API Not Working?
```bash
curl -H "Authorization: Bearer YOUR_TOKEN" \
  https://api.beatport.com/v4/catalog/genres/
```

### Tracks Not Displaying?
Check TreeItemModel is properly connected to QTreeView.

### Stream URL Issues?
Verify using `/stream/` not `/download/` endpoint.

### HLS Not Playing?
Quick hack: Replace `.m3u8` with `-1.aac` for single segment test.

---

## FILES TO MODIFY (Priority Order)

1. `src/streaming/hook/streamingdto.h` - Add Genre, Chart structs
2. `src/streaming/hook/beatportservice.h` - Add method declarations
3. `src/streaming/hook/beatportservice.cpp` - Implement genre/chart fetches
4. `src/streaming/hook/beatportfeature.cpp` - Wire tree population
5. Test and iterate

---

## CURRENT STATE ANALYSIS

### BeatportFeature (lines 1-51)
**Current:** Skeleton with only "Search" node
```cpp
// Line 19 - Only child is Search
root->appendChild("Search", "beatport_search");

// Lines 37-40 - activate() is EMPTY
void BeatportFeature::activate() {
    // Switch main view to a search page or something
    // m_pLibrary->switchToView("BeatportView");
}

// Lines 42-44 - activateChild() is EMPTY
void BeatportFeature::activateChild(const QModelIndex& index) {
    // Handle click on children
}
```

**NEEDED:** Populate tree on activate(), handle child clicks to fetch tracks

### streamingdto.h (lines 1-71)
**Current:** Has StreamTrackMetadata, StreamInfo, SearchQuery
**MISSING:** Genre, Chart, Artist, Label, Playlist DTOs

### Specific Insertions Required

**In streamingdto.h after SearchQuery (line 71):**
```cpp
struct BeatportGenre {
    int id;
    QString name;
    QString slug;
    QList<BeatportGenre> subgenres;
};

struct BeatportChart {
    int id;
    QString name;
    QString slug;
    int trackCount;
    QString imageUrl;
    QDate publishDate;
};

struct BeatportArtist {
    int id;
    QString name;
    QString slug;
};

struct BeatportLabel {
    int id;
    QString name;
    QString slug;
};
```

**In beatportfeature.cpp activate() (replace lines 37-40):**
```cpp
void BeatportFeature::activate() {
    // Fetch genres on first activation
    if (!m_genresLoaded) {
        fetchGenres();
    }
    // Switch to Beatport view
    emit featureSelect(this, QModelIndex());
}

void BeatportFeature::fetchGenres() {
    auto* service = dynamic_cast<BeatportService*>(m_pService.get());
    if (!service) return;

    connect(service, &BeatportService::genresFetched,
            this, &BeatportFeature::onGenresFetched,
            Qt::UniqueConnection);
    service->fetchGenres();
}

void BeatportFeature::onGenresFetched(const QList<BeatportGenre>& genres) {
    TreeItem* root = m_pSidebarModel->getRootItem();

    // Add Genres parent
    TreeItem* genresNode = root->appendChild("Genres", "beatport_genres");
    for (const auto& genre : genres) {
        genresNode->appendChild(genre.name, QString("beatport_genre_%1").arg(genre.id));
    }

    // Add Charts parent (fetch separately)
    root->appendChild("Charts", "beatport_charts");

    m_genresLoaded = true;
}
```

---

## SUCCESS CRITERIA

By end of session:
- [ ] Genres list visible in sidebar under "Beatport"
- [ ] Clicking genre shows Top 100 tracks
- [ ] Charts list visible
- [ ] Clicking chart shows tracks
- [ ] Double-clicking track initiates stream

**Stretch goals:**
- [ ] Artist/Label pivot working
- [ ] Search with filters
- [ ] Playlist support

---

*Keep BEATPORT_INTEGRATION_SPEC.md open for API reference during implementation.*
