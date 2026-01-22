# Mixxx-Kinetic Technical Specification

**Version**: 1.0.0
**Status**: AUTHORITATIVE SPECIFICATION
**Target Platform**: EndeavourOS / Arch Linux
**License Constraint**: GPLv2+ (core), MIT/BSD (isolated components)

---

## Table of Contents

1. [Specification Overview](#1-specification-overview)
2. [Technology Stack](#2-technology-stack)
3. [Architecture Layers](#3-architecture-layers)
4. [Agent Boundary Definitions](#4-agent-boundary-definitions)
5. [Component Specifications](#5-component-specifications)
6. [Interface Contracts](#6-interface-contracts)
7. [Data Flow Specifications](#7-data-flow-specifications)
8. [Integration Protocols](#8-integration-protocols)
9. [Drift Prevention Rules](#9-drift-prevention-rules)
10. [Delivery Roadmap](#10-delivery-roadmap)
11. [Future Integration Framework](#11-future-integration-framework)

---

## 1. Specification Overview

### 1.1 Purpose

This document serves three functions:

| Function | Description |
|----------|-------------|
| **SPEC** | Authoritative technical requirements for all components |
| **TOOL REFERENCE** | Canonical list of technologies, libraries, and APIs |
| **DRIFT PREVENTION** | Boundary rules that maintain architectural integrity |

### 1.2 Core Principle: Zero-Trust Streaming

```
┌─────────────────────────────────────────────────────────────────┐
│                    ZERO-TRUST STREAM PRINCIPLE                  │
├─────────────────────────────────────────────────────────────────┤
│  The network is HOSTILE. The audio engine NEVER touches it.    │
│  All remote streams are virtualized as local filesystem paths.  │
│  Buffer underruns produce SILENCE, never thread blocking.       │
└─────────────────────────────────────────────────────────────────┘
```

### 1.3 Architectural Invariants

These rules are **NON-NEGOTIABLE**:

1. **INV-001**: `EngineWorker` thread performs ZERO network I/O
2. **INV-002**: `SoundSource` implementations return within 1ms
3. **INV-003**: DRM components run in isolated processes (GPL compliance)
4. **INV-004**: OAuth tokens stored exclusively in system keyring
5. **INV-005**: Stream URLs are NEVER persisted beyond session lifetime
6. **INV-006**: All API interactions occur through the Hook layer

---

## 2. Technology Stack

### 2.1 Core Dependencies

| Component | Technology | Version | Repository | Purpose |
|-----------|------------|---------|------------|---------|
| Base Application | Mixxx | 2.4+ | github.com/mixxxdj/mixxx | Fork base |
| Build System | CMake | 3.25+ | cmake.org | Cross-module builds |
| Compiler | GCC/Clang | 13+/16+ | gnu.org/clang.llvm.org | C++20 support |
| UI Framework | Qt | 6.5+ | qt.io | Widgets, Network, WebEngine |
| Database | SQLite | 3.40+ | sqlite.org | TrackDAO persistence |
| Audio Codec | FFmpeg | 6.0+ | ffmpeg.org | HLS/DASH/AAC/FLAC decoding |
| Userspace FS | libfuse3 | 3.14+ | github.com/libfuse/libfuse | Stream virtualization |
| Audio Server | PipeWire | 0.3.60+ | pipewire.org | Low-latency audio routing |
| Secrets | libsecret | 0.21+ | gnome.org | OAuth token storage |

### 2.2 Network & Protocol Libraries

| Library | Version | Purpose | Install (Arch) |
|---------|---------|---------|----------------|
| libcurl | 8.0+ | HTTP/HTTPS client, Range requests | `curl` |
| nghttp2 | 1.55+ | HTTP/2 multiplexing | `nghttp2` |
| openssl | 3.0+ | TLS 1.3, certificate validation | `openssl` |
| protobuf | 24+ | API serialization (future) | `protobuf` |
| nlohmann-json | 3.11+ | JSON parsing | `nlohmann-json` |

### 2.3 Streaming Protocol Support

| Protocol | Library/Tool | Use Case |
|----------|--------------|----------|
| HLS (.m3u8) | FFmpeg libavformat | Beatport streams |
| MPEG-DASH (.mpd) | FFmpeg + libdash | Tidal HiFi/Master |
| Widevine DRM | libwidevinecdm.so | Tidal encrypted content |
| OAuth 2.0 | Qt Network + libsecret | Authentication flows |

### 2.4 Development & Testing Tools

| Tool | Purpose | Install |
|------|---------|---------|
| GTest | Unit testing | `gtest` |
| Catch2 | BDD-style testing | `catch2` |
| Valgrind | Memory leak detection | `valgrind` |
| perf | Performance profiling | `perf` |
| strace | Syscall tracing | `strace` |
| Wireshark | Network debugging | `wireshark-qt` |

---

## 3. Architecture Layers

### 3.1 Layer Diagram

```
┌─────────────────────────────────────────────────────────────────────────┐
│                              LAYER 0: UI                                │
│                    Qt Widgets, Waveform, Library Browser                │
├─────────────────────────────────────────────────────────────────────────┤
│                           LAYER 1: ENGINE                               │
│              EngineWorker, EngineBuffer, CachingReader                  │
│                      [REALTIME PRIORITY - NO BLOCKING]                  │
├─────────────────────────────────────────────────────────────────────────┤
│                          LAYER 2: SOURCE                                │
│         SoundSourceProxy, SoundSourceStream, AnalyzerQueue              │
│                      [LOCK-FREE RING BUFFER ACCESS]                     │
├─────────────────────────────────────────────────────────────────────────┤
│                          LAYER 3: BRIDGE                                │
│              mixxx-fs (FUSE), SparseCache, IntervalTree                 │
│                      [SEPARATE PROCESS/THREAD]                          │
├─────────────────────────────────────────────────────────────────────────┤
│                           LAYER 4: HOOK                                 │
│       StreamingService, OAuthManager, MetadataNormalizer, CDMHost       │
│                      [NETWORK I/O PERMITTED]                            │
├─────────────────────────────────────────────────────────────────────────┤
│                         LAYER 5: EXTERNAL                               │
│              Beatport API, Tidal API, Widevine CDM                      │
│                      [UNTRUSTED - VOLATILE]                             │
└─────────────────────────────────────────────────────────────────────────┘
```

### 3.2 Layer Communication Rules

| From Layer | To Layer | Allowed Mechanism |
|------------|----------|-------------------|
| 0 (UI) | 1 (Engine) | Qt Signals/Slots, ControlProxy |
| 1 (Engine) | 2 (Source) | Direct method calls (< 1ms) |
| 2 (Source) | 3 (Bridge) | POSIX file I/O (`open`, `read`, `seek`) |
| 3 (Bridge) | 4 (Hook) | IPC: Unix Domain Socket, Shared Memory |
| 4 (Hook) | 5 (External) | HTTPS, OAuth flows |

**FORBIDDEN COMMUNICATIONS**:
- Layer 1 → Layer 4 (Engine cannot touch network)
- Layer 1 → Layer 5 (Engine cannot touch external APIs)
- Layer 2 → Layer 5 (Source cannot touch external APIs)

---

## 4. Agent Boundary Definitions

This section defines the work domains for implementation agents. Each agent owns a specific boundary and must not violate adjacent boundaries.

### 4.1 Agent Roster

| Agent ID | Name | Boundary | Primary Files |
|----------|------|----------|---------------|
| **A-HOOK** | Hook Agent | Layer 4: API Integration | `src/streaming/hook/*` |
| **A-BRIDGE** | Bridge Agent | Layer 3: FUSE Virtualization | `src/streaming/bridge/*` |
| **A-SOURCE** | Source Agent | Layer 2: SoundSource Extensions | `src/sources/*` |
| **A-DAO** | Database Agent | TrackDAO Schema Extensions | `src/library/dao/*` |
| **A-UI** | Interface Agent | Layer 0: UI Components | `src/widget/streaming/*` |
| **A-BUILD** | Build Agent | CMake, AUR Packaging | `CMakeLists.txt`, `PKGBUILD` |
| **A-TEST** | Test Agent | Integration & Unit Tests | `src/test/streaming/*` |

### 4.2 Agent Interaction Matrix

```
           A-HOOK  A-BRIDGE  A-SOURCE  A-DAO  A-UI  A-BUILD  A-TEST
A-HOOK       -        W         -        W      -       -        R
A-BRIDGE     R        -         W        -      -       -        R
A-SOURCE     -        R         -        R      W       -        R
A-DAO        R        -         R        -      R       -        R
A-UI         R        -         R        R      -       -        R
A-BUILD      -        -         -        -      -       -        R
A-TEST       R        R         R        R      R       R        -

Legend: W = Writes to agent's output, R = Reads agent's output, - = No direct interaction
```

### 4.3 Agent A-HOOK Specification

**Boundary**: All external API communication

**Responsibilities**:
1. OAuth 2.0 authentication flows (Beatport, Tidal)
2. Token refresh interceptor
3. Metadata retrieval and normalization
4. Stream URL acquisition
5. Subscription tier validation

**Inputs**:
- User credentials (via UI)
- Track IDs for metadata/stream requests
- Search queries

**Outputs**:
- Normalized `TrackMetadata` structs → A-DAO
- Stream URLs with TTL → A-BRIDGE
- Authentication state → A-UI

**Technology Constraints**:
- Qt Network for HTTP
- libsecret/QtKeychain for token storage
- nlohmann-json for parsing

**Files Owned**:
```
src/streaming/hook/
├── oauthmanager.h/.cpp       # OAuth flow management
├── beatportservice.h/.cpp    # Beatport API client
├── tidalservice.h/.cpp       # Tidal API client
├── metadatanormalizer.h/.cpp # Key/artist/art normalization
├── streamingservice.h        # Abstract service interface
└── tokeninterceptor.h/.cpp   # Auto-refresh logic
```

### 4.4 Agent A-BRIDGE Specification

**Boundary**: Stream virtualization and caching

**Responsibilities**:
1. FUSE filesystem implementation (`mixxx-fs`)
2. Sparse file caching with Interval Tree
3. HTTP Range request management
4. DRM decryption coordination (CDM Host IPC)
5. Prefetch scheduling

**Inputs**:
- Stream URLs from A-HOOK
- File I/O requests from A-SOURCE (via FUSE)

**Outputs**:
- Virtual file paths (`/run/user/{uid}/mixxx-stream/`)
- Decrypted audio data
- Cache status signals → A-UI

**Technology Constraints**:
- libfuse3 (FUSE 3 API)
- libcurl for HTTP Range requests
- POSIX shared memory for CDM IPC

**Files Owned**:
```
src/streaming/bridge/
├── fusedriver.h/.cpp         # FUSE operations implementation
├── sparsecache.h/.cpp        # Interval tree + backing store
├── rangefetcher.h/.cpp       # HTTP Range request manager
├── cdmhost.h/.cpp            # Widevine IPC coordinator
├── prefetcher.h/.cpp         # Predictive download scheduler
└── mountmanager.h/.cpp       # FUSE mount lifecycle
```

**Separate Binary**:
```
tools/mixxx-fs/
├── main.cpp                  # Standalone FUSE daemon entry
├── CMakeLists.txt
└── mixxx-fs.service          # systemd user service unit
```

### 4.5 Agent A-SOURCE Specification

**Boundary**: SoundSource hierarchy extensions

**Responsibilities**:
1. `SoundSourceProxy` implementation
2. `SoundSourceStream` implementation
3. Lock-free ring buffer management
4. Silence injection on buffer underrun
5. Analyzer throttling for streamed tracks

**Inputs**:
- Virtual file paths from A-BRIDGE
- Engine read requests

**Outputs**:
- PCM audio samples → Engine
- Buffer status signals → A-UI
- Cache percentage → Waveform renderer

**Technology Constraints**:
- Must inherit `SoundSource` interface exactly
- All methods return within 1ms
- No blocking I/O in `read()` or `seek()`

**Files Owned**:
```
src/sources/
├── soundsourceproxy.h/.cpp   # Engine-facing interface
├── soundsourcestream.h/.cpp  # Stream-aware implementation
├── streamringbuffer.h/.cpp   # Lock-free circular buffer
└── analyzerthrottle.h/.cpp   # Passive analysis coordinator
```

### 4.6 Agent A-DAO Specification

**Boundary**: Database schema and persistence

**Responsibilities**:
1. Schema migration for streaming columns
2. `TrackDAO` extensions for remote sources
3. Cache eviction policy implementation
4. Offline status management

**Inputs**:
- Normalized metadata from A-HOOK
- Cache state from A-BRIDGE

**Outputs**:
- Persisted track records
- Query results → A-UI, A-SOURCE

**Schema Extensions** (Source of Truth):
```sql
-- Migration: 001_streaming_support.sql
ALTER TABLE library ADD COLUMN remote_source TEXT DEFAULT 'filesystem';
ALTER TABLE library ADD COLUMN remote_id TEXT;
ALTER TABLE library ADD COLUMN stream_url_cache TEXT;
ALTER TABLE library ADD COLUMN stream_expires_at INTEGER;
ALTER TABLE library ADD COLUMN drm_key_id TEXT;
ALTER TABLE library ADD COLUMN offline_status INTEGER DEFAULT 0;
ALTER TABLE library ADD COLUMN last_streamed_at INTEGER;

-- Indexes for streaming queries
CREATE INDEX idx_library_remote_source ON library(remote_source);
CREATE INDEX idx_library_offline_status ON library(offline_status);
CREATE INDEX idx_library_stream_expires ON library(stream_expires_at);

-- offline_status enum:
-- 0 = CLOUD_ONLY (requires network)
-- 1 = PARTIAL_CACHE (some chunks cached)
-- 2 = FULL_CACHE (complete but temporary)
-- 3 = OFFLINE_PERMANENT (user-saved for offline)
```

**Files Owned**:
```
src/library/dao/
├── trackdao_streaming.cpp    # Extended TrackDAO methods
├── streamingcachemanager.h/.cpp # Cache eviction logic
└── migrations/
    └── 001_streaming_support.sql
```

### 4.7 Agent A-UI Specification

**Boundary**: User interface components for streaming

**Responsibilities**:
1. Streaming service login dialogs
2. Library browser integration (cloud sources)
3. Waveform overlay (download progress)
4. Buffer status indicators
5. Offline management UI

**Inputs**:
- Auth state from A-HOOK
- Buffer status from A-SOURCE
- Cache status from A-BRIDGE

**Outputs**:
- User credentials → A-HOOK
- Track selection → A-SOURCE

**Files Owned**:
```
src/widget/streaming/
├── loginwidget.h/.cpp        # OAuth login UI
├── streaminglibraryfeature.h/.cpp # Library browser extension
├── waveformoverlay.h/.cpp    # Download progress visualization
├── bufferindicator.h/.cpp    # Network status display
└── offlinemanager.h/.cpp     # Offline library UI
```

### 4.8 Agent A-BUILD Specification

**Boundary**: Build system and packaging

**Responsibilities**:
1. CMake module for streaming components
2. Conditional compilation flags
3. AUR PKGBUILD for `mixxx-kinetic-git`
4. Dependency resolution
5. System tuning scripts

**Files Owned**:
```
cmake/
├── FindFuse3.cmake
├── FindLibSecret.cmake
└── StreamingComponents.cmake

packaging/arch/
├── PKGBUILD
├── mixxx-kinetic.install     # Post-install hooks
└── system-tuning/
    ├── 99-mixxx-realtime.conf    # /etc/security/limits.d/
    ├── 99-mixxx-network.conf     # /etc/sysctl.d/
    └── mixxx.conf                # PipeWire profile
```

### 4.9 Agent A-TEST Specification

**Boundary**: Testing infrastructure

**Responsibilities**:
1. Unit tests for each component
2. Integration tests for layer boundaries
3. Mock services for Beatport/Tidal APIs
4. Performance benchmarks
5. FUSE stress testing

**Files Owned**:
```
src/test/streaming/
├── hook/
│   ├── oauthmanager_test.cpp
│   ├── beatportservice_test.cpp
│   └── mock_httpserver.h/.cpp
├── bridge/
│   ├── sparsecache_test.cpp
│   ├── fusedriver_test.cpp
│   └── mock_fuse.h
├── source/
│   ├── soundsourcestream_test.cpp
│   └── ringbuffer_test.cpp
├── integration/
│   ├── full_stream_test.cpp
│   └── offline_transition_test.cpp
└── benchmarks/
    ├── buffer_latency_bench.cpp
    └── cache_throughput_bench.cpp
```

---

## 5. Component Specifications

### 5.1 FUSE Driver (`mixxx-fs`)

#### 5.1.1 Mount Point Structure

```
/run/user/{uid}/mixxx-stream/
├── beatport/
│   ├── {track_id}.aac        # Virtual files
│   └── {track_id}.aac
└── tidal/
    ├── {track_id}.flac
    └── {track_id}.m4a
```

#### 5.1.2 FUSE Operations Implementation

| Operation | Behavior |
|-----------|----------|
| `getattr` | Return `stat` with size from HTTP `Content-Length` or API metadata |
| `open` | Register file handle, initiate prefetch from byte 0 |
| `read` | Serve from SparseCache if hit; block and fetch if miss |
| `release` | Decrement handle count, schedule cache cleanup if zero |
| `statfs` | Report virtual filesystem stats |

#### 5.1.3 Sparse Cache Data Structure

```cpp
// sparsecache.h - Interval Tree for cached ranges
class SparseCache {
public:
    struct CachedRange {
        int64_t start;
        int64_t end;  // exclusive
    };

    // Check if byte range is fully cached
    bool isRangeCached(int64_t start, int64_t length) const;

    // Get list of missing ranges within a request
    std::vector<CachedRange> getMissingRanges(int64_t start, int64_t length) const;

    // Mark range as cached after download
    void markCached(int64_t start, int64_t length);

    // Get total cached percentage
    double getCachedPercentage() const;

    // Get all cached ranges (for waveform rendering)
    std::vector<CachedRange> getAllCachedRanges() const;

private:
    std::map<int64_t, int64_t> m_intervals;  // start -> end
    int64_t m_totalSize;
    std::string m_backingFilePath;

    void mergeOverlappingIntervals();
};
```

#### 5.1.4 CDM Host IPC Protocol

For Widevine DRM decryption (Tidal HiFi/Master):

```
┌──────────────────┐         ┌──────────────────┐
│    mixxx-fs      │   SHM   │    cdm-host      │
│  (FUSE daemon)   │◄───────►│  (separate proc) │
└──────────────────┘         └──────────────────┘
         │                            │
         │  1. Write encrypted chunk  │
         │     to shared memory       │
         │─────────────────────────►  │
         │                            │
         │  2. Signal ready           │
         │─────────────────────────►  │
         │                            │
         │  3. CDM decrypts in-place  │
         │                            │
         │  4. Signal complete        │
         │  ◄─────────────────────────│
         │                            │
         │  5. Read decrypted data    │
         │                            │
```

**Shared Memory Layout**:
```cpp
struct CDMSharedBuffer {
    std::atomic<uint32_t> state;  // 0=idle, 1=pending, 2=ready, 3=error
    uint32_t data_size;
    uint32_t key_id_length;
    char key_id[64];
    char data[1024 * 1024];  // 1MB chunk buffer
};
```

### 5.2 SoundSource Extensions

#### 5.2.1 Class Hierarchy

```
SoundSource (existing abstract base)
    │
    ├── SoundSourceFFmpeg (existing)
    ├── SoundSourceSndFile (existing)
    │
    └── SoundSourceProxy (NEW)
            │
            └── SoundSourceStream (NEW)
```

#### 5.2.2 SoundSourceProxy Interface

```cpp
// soundsourceproxy.h
class SoundSourceProxy : public SoundSource {
public:
    explicit SoundSourceProxy(const QString& virtualPath);

    // SoundSource interface - MUST return immediately
    ReadableSampleFrames readSampleFramesClamped(
        const WritableSampleFrames& writableFrames) override;

    void seekSampleFrame(SINT frameIndex) override;

    // Streaming extensions
    double getCachedPercentage() const;
    QVector<QPair<qint64, qint64>> getCachedRanges() const;
    bool isFullyCached() const;

    // Signals
    signal void bufferUnderrun();
    signal void cacheProgressChanged(double percentage);

private:
    std::unique_ptr<StreamRingBuffer> m_buffer;
    QString m_virtualPath;
    qint64 m_totalFrames;
    std::atomic<bool> m_underrunState;
};
```

#### 5.2.3 Lock-Free Ring Buffer

```cpp
// streamringbuffer.h
template<typename T, size_t Capacity>
class StreamRingBuffer {
public:
    // Producer (Bridge thread)
    bool tryWrite(const T* data, size_t count);
    size_t availableWrite() const;

    // Consumer (Engine thread) - NEVER blocks
    size_t tryRead(T* data, size_t count);
    size_t availableRead() const;

    // Seek support
    void reset();
    void setReadPosition(size_t position);

private:
    alignas(64) std::array<T, Capacity> m_buffer;
    alignas(64) std::atomic<size_t> m_writePos;
    alignas(64) std::atomic<size_t> m_readPos;
};
```

### 5.3 OAuth Manager

#### 5.3.1 Token Lifecycle

```
┌─────────────────────────────────────────────────────────────────┐
│                      TOKEN STATE MACHINE                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│   ┌──────────┐    login()    ┌───────────────┐                 │
│   │  LOGGED  │──────────────►│  AUTHORIZING  │                 │
│   │   OUT    │               │  (browser)    │                 │
│   └──────────┘               └───────┬───────┘                 │
│        ▲                             │                         │
│        │                             │ callback received       │
│        │                             ▼                         │
│        │                     ┌───────────────┐                 │
│        │ logout()            │   EXCHANGING  │                 │
│        │                     │   (tokens)    │                 │
│        │                     └───────┬───────┘                 │
│        │                             │                         │
│        │                             │ tokens received         │
│        │                             ▼                         │
│   ┌────┴─────┐    expires    ┌───────────────┐                 │
│   │  LOGGED  │◄──────────────│  REFRESHING   │◄────┐           │
│   │   IN     │               └───────────────┘     │           │
│   └────┬─────┘                       │             │           │
│        │                             │             │           │
│        │ 401 response                │ success     │ 401       │
│        └─────────────────────────────┴─────────────┘           │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

#### 5.3.2 Token Storage

**Keyring Schema** (libsecret):
```
Schema: org.mixxx.kinetic.oauth
Attributes:
  - service: "beatport" | "tidal"
  - token_type: "access" | "refresh"
Secret: <token_value>
```

**Retrieval**:
```cpp
QString OAuthManager::getToken(Service service, TokenType type) {
    GError* error = nullptr;
    gchar* token = secret_password_lookup_sync(
        &mixxx_schema,
        nullptr,
        &error,
        "service", serviceToString(service),
        "token_type", tokenTypeToString(type),
        nullptr
    );
    // ... error handling
    return QString::fromUtf8(token);
}
```

---

## 6. Interface Contracts

### 6.1 StreamingService Abstract Interface

All streaming service implementations (Beatport, Tidal, future services) MUST implement:

```cpp
// streamingservice.h
class StreamingService : public QObject {
    Q_OBJECT
public:
    virtual ~StreamingService() = default;

    // Identity
    virtual QString serviceName() const = 0;
    virtual QString serviceId() const = 0;  // "beatport", "tidal"

    // Authentication
    virtual void initiateLogin() = 0;
    virtual void logout() = 0;
    virtual bool isAuthenticated() const = 0;
    virtual SubscriptionTier getSubscriptionTier() const = 0;

    // Catalog
    virtual QFuture<TrackMetadata> fetchTrackMetadata(const QString& trackId) = 0;
    virtual QFuture<QVector<TrackMetadata>> search(const SearchQuery& query) = 0;
    virtual QFuture<QVector<TrackMetadata>> getPlaylist(const QString& playlistId) = 0;

    // Streaming
    virtual QFuture<StreamInfo> getStreamInfo(const QString& trackId) = 0;

signals:
    void authStateChanged(AuthState newState);
    void loginError(const QString& message);
    void tokenRefreshed();

protected:
    // Subclasses implement these
    virtual QUrl getAuthorizationUrl() const = 0;
    virtual QFuture<TokenPair> exchangeCodeForTokens(const QString& code) = 0;
    virtual QFuture<TokenPair> refreshTokens() = 0;
};
```

### 6.2 Data Transfer Objects

```cpp
// streamingdto.h

struct TrackMetadata {
    QString remoteId;           // Service-specific ID
    QString title;
    QString artist;             // Normalized, comma-separated
    QString album;
    int bpm;                    // Integer BPM
    QString key;                // Normalized to user preference (Camelot/OpenKey)
    int durationMs;
    QString coverArtUrl;        // Remote URL (Hook downloads and caches)
    QString coverArtLocalPath;  // Local cached path
    QString genre;
    int year;
    QString label;
    StreamingService* source;
};

struct StreamInfo {
    QString trackId;
    QUrl streamUrl;             // Temporary, signed URL
    QDateTime expiresAt;        // URL expiration time
    QString mimeType;           // "application/vnd.apple.mpegurl", "application/dash+xml"
    QString codec;              // "aac", "flac", "mqa"
    int bitrate;                // kbps
    bool encrypted;             // DRM protected
    QString licenseUrl;         // Widevine license server (if encrypted)
    QString keyId;              // DRM key ID (if encrypted)
};

struct SearchQuery {
    QString text;
    QString artist;             // Optional filter
    QString genre;              // Optional filter
    QPair<int, int> bpmRange;   // Optional filter
    QString key;                // Optional filter
    int limit = 50;
    int offset = 0;
};

enum class AuthState {
    LoggedOut,
    Authorizing,
    Exchanging,
    LoggedIn,
    Refreshing,
    Error
};

enum class SubscriptionTier {
    None,
    Basic,
    Premium,      // Beatport Link, Tidal HiFi
    Professional  // Beatport Link Pro, Tidal HiFi Plus
};

enum class OfflineStatus {
    CloudOnly = 0,
    PartialCache = 1,
    FullCache = 2,
    OfflinePermanent = 3
};
```

### 6.3 Error Handling Contract

All components MUST use structured error types:

```cpp
// streamingerror.h

enum class StreamingErrorCode {
    // Network (1xx)
    NetworkUnreachable = 100,
    ConnectionTimeout = 101,
    SSLError = 102,

    // Auth (2xx)
    AuthenticationRequired = 200,
    TokenExpired = 201,
    TokenRefreshFailed = 202,
    InsufficientSubscription = 203,

    // API (3xx)
    TrackNotFound = 300,
    TrackUnavailable = 301,
    RateLimited = 302,
    APIChanged = 303,

    // DRM (4xx)
    DRMUnsupported = 400,
    LicenseAcquisitionFailed = 401,
    DecryptionFailed = 402,

    // Cache (5xx)
    CacheWriteFailed = 500,
    CacheCorrupted = 501,
    DiskFull = 502,

    // Internal (9xx)
    InternalError = 900
};

class StreamingError {
public:
    StreamingErrorCode code;
    QString message;
    QString technicalDetails;  // For logging
    bool isRetryable;
    std::optional<int> retryAfterSeconds;
};
```

---

## 7. Data Flow Specifications

### 7.1 Track Load Flow

```
User clicks track in streaming library
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 1. UI Layer                                                         │
│    - Emit trackLoadRequested(remoteId, serviceId)                  │
└─────────────────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 2. Hook Layer (A-HOOK)                                              │
│    - Check auth state                                               │
│    - Call getStreamInfo(trackId)                                    │
│    - Receive StreamInfo with signed URL                             │
│    - Pass URL to Bridge                                             │
└─────────────────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 3. Bridge Layer (A-BRIDGE)                                          │
│    - Register virtual file: /run/user/1000/mixxx-stream/tidal/123   │
│    - Initiate prefetch from byte 0                                  │
│    - Return virtual path                                            │
└─────────────────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 4. Source Layer (A-SOURCE)                                          │
│    - SoundSourceProxy::open(virtualPath)                            │
│    - FFmpeg probes container format                                 │
│    - Return handle to Engine                                        │
└─────────────────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 5. Engine Layer                                                     │
│    - CachingReader begins populating buffer                         │
│    - EngineWorker starts playback callback                          │
└─────────────────────────────────────────────────────────────────────┘
```

### 7.2 Seek Operation Flow

```
User seeks to 3:00 in a partially downloaded track
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 1. Engine: seekSampleFrame(frame_at_3min)                           │
│    - Calls SoundSourceProxy::seekSampleFrame()                      │
└─────────────────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 2. SoundSourceProxy                                                 │
│    - Translates frame to byte offset                                │
│    - Calls lseek() on FUSE file handle                              │
└─────────────────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 3. FUSE Driver (mixxx-fs)                                           │
│    - Updates internal file position                                 │
│    - Next read() will check SparseCache for this offset            │
└─────────────────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 4. On next read():                                                  │
│    IF offset cached → return data immediately                       │
│    IF offset NOT cached:                                            │
│       - Block this read (NOT the engine thread)                     │
│       - Spawn high-priority Range Request                           │
│       - Wait for data, write to cache, return                       │
└─────────────────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 5. SoundSourceProxy::readSampleFramesClamped()                      │
│    IF data arrives in time → return samples                         │
│    IF timeout (>500ms) → return silence, emit bufferUnderrun()      │
└─────────────────────────────────────────────────────────────────────┘
```

### 7.3 Token Refresh Flow

```
API returns 401 Unauthorized
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 1. TokenInterceptor catches 401                                     │
│    - Pause original request                                         │
│    - Queue any concurrent requests                                  │
└─────────────────────────────────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────────────────────────────────┐
│ 2. Call refreshTokens()                                             │
│    POST /oauth/token                                                │
│    grant_type=refresh_token                                         │
│    refresh_token=<from_keyring>                                     │
└─────────────────────────────────────────────────────────────────────┘
            │
            ├─── Success ──────────────────────┐
            │                                  ▼
            │    ┌─────────────────────────────────────────────────────┐
            │    │ 3a. Store new tokens in keyring                     │
            │    │     Retry original request with new access_token   │
            │    │     Resume queued requests                          │
            │    └─────────────────────────────────────────────────────┘
            │
            └─── Failure ──────────────────────┐
                                               ▼
                 ┌─────────────────────────────────────────────────────┐
                 │ 3b. Clear tokens from keyring                       │
                 │     Emit authStateChanged(LoggedOut)                │
                 │     Prompt user to re-authenticate                  │
                 └─────────────────────────────────────────────────────┘
```

---

## 8. Integration Protocols

### 8.1 Beatport API Protocol

**Base URL**: `https://api.beatport.com/v4`

| Endpoint | Method | Purpose | Auth |
|----------|--------|---------|------|
| `/auth/o/authorize/` | GET | Initiate OAuth | None |
| `/auth/o/token/` | POST | Exchange/refresh tokens | Basic (client) |
| `/catalog/tracks/{id}` | GET | Track metadata | Bearer |
| `/catalog/tracks/{id}/stream` | GET | Stream URL | Bearer |
| `/catalog/search` | GET | Search catalog | Bearer |
| `/my/subscriptions` | GET | Check subscription tier | Bearer |

**Request Headers**:
```http
Authorization: Bearer <access_token>
Accept: application/json
User-Agent: Mixxx-Kinetic/1.0
```

**Stream Response Handling**:
```cpp
void BeatportService::handleStreamResponse(const QJsonObject& json) {
    StreamInfo info;
    info.streamUrl = QUrl(json["stream_url"].toString());
    info.mimeType = "application/vnd.apple.mpegurl";  // HLS
    info.codec = "aac";
    info.bitrate = json["quality"].toString() == "high" ? 256 : 128;
    info.encrypted = false;  // Beatport typically unencrypted

    // URL TTL is typically 1 hour - DO NOT PERSIST
    info.expiresAt = QDateTime::currentDateTime().addSecs(3600);

    emit streamInfoReady(info);
}
```

### 8.2 Tidal API Protocol

**Auth Base URL**: `https://auth.tidal.com/v1`
**API Base URL**: `https://api.tidal.com/v1`

| Endpoint | Method | Purpose | Auth |
|----------|--------|---------|------|
| `/oauth2/device_authorization` | POST | Start device flow | Client ID |
| `/oauth2/token` | POST | Poll for tokens | Client ID |
| `/tracks/{id}` | GET | Track metadata | Bearer |
| `/tracks/{id}/playbackinfopostpaywall` | GET | Stream info | Bearer |
| `/search` | GET | Search catalog | Bearer |

**Device Flow Implementation**:
```cpp
void TidalService::initiateLogin() {
    QNetworkRequest request(QUrl("https://auth.tidal.com/v1/oauth2/device_authorization"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("client_id", m_clientId);
    params.addQueryItem("scope", "r_usr w_usr w_sub");

    auto reply = m_network->post(request, params.query(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        auto json = QJsonDocument::fromJson(reply->readAll()).object();

        QString deviceCode = json["deviceCode"].toString();
        QString userCode = json["userCode"].toString();
        QUrl verificationUrl(json["verificationUriComplete"].toString());
        int interval = json["interval"].toInt();

        // Show user code in UI, open browser to verification URL
        emit deviceCodeReady(userCode, verificationUrl);

        // Start polling for token
        startTokenPolling(deviceCode, interval);
    });
}
```

**Playback Info Response Parsing**:
```cpp
StreamInfo TidalService::parsePlaybackInfo(const QJsonObject& json) {
    StreamInfo info;
    info.trackId = json["trackId"].toString();
    info.mimeType = json["manifestMimeType"].toString();
    info.codec = json["audioQuality"].toString();  // "HI_RES", "LOSSLESS", etc.

    // Decode Base64 manifest
    QByteArray manifestB64 = json["manifest"].toString().toUtf8();
    QByteArray manifest = QByteArray::fromBase64(manifestB64);

    if (info.mimeType == "application/dash+xml") {
        // Parse DASH MPD to extract stream URL
        info.streamUrl = parseDashManifest(manifest);
    } else {
        // Direct URL for lower quality
        info.streamUrl = QUrl(QString::fromUtf8(manifest));
    }

    // DRM handling
    QString encType = json["encryptionType"].toString();
    info.encrypted = (encType == "WIDEVINE");
    if (info.encrypted) {
        info.licenseUrl = json["licenseSecurityToken"].toString();
        info.keyId = json["keyId"].toString();
    }

    return info;
}
```

### 8.3 IPC Protocol: Main Process ↔ FUSE Daemon

**Transport**: Unix Domain Socket at `/run/user/{uid}/mixxx-stream.sock`

**Message Format** (JSON over newline-delimited stream):

```json
// Request: Register new stream
{
    "type": "register_stream",
    "id": "msg-001",
    "payload": {
        "service": "tidal",
        "track_id": "123456",
        "stream_url": "https://...",
        "total_size": 52428800,
        "mime_type": "audio/flac",
        "encrypted": true,
        "license_url": "https://...",
        "key_id": "abc123"
    }
}

// Response
{
    "type": "stream_registered",
    "id": "msg-001",
    "payload": {
        "virtual_path": "/run/user/1000/mixxx-stream/tidal/123456.flac",
        "status": "ready"
    }
}

// Request: Cache status query
{
    "type": "get_cache_status",
    "id": "msg-002",
    "payload": {
        "virtual_path": "/run/user/1000/mixxx-stream/tidal/123456.flac"
    }
}

// Response
{
    "type": "cache_status",
    "id": "msg-002",
    "payload": {
        "cached_percentage": 45.2,
        "cached_ranges": [[0, 23592960], [31457280, 36700160]],
        "state": "downloading"
    }
}

// Event: Buffer underrun notification (daemon → main)
{
    "type": "buffer_warning",
    "payload": {
        "virtual_path": "/run/user/1000/mixxx-stream/tidal/123456.flac",
        "requested_offset": 41943040,
        "available_offset": 36700160,
        "eta_ms": 1500
    }
}
```

---

## 9. Drift Prevention Rules

### 9.1 Code Review Checklist

Before merging ANY code, reviewers MUST verify:

| Rule ID | Check | Violation Response |
|---------|-------|-------------------|
| DR-001 | No network I/O in `EngineWorker` call stack | REJECT |
| DR-002 | No blocking calls in `SoundSource::read()` | REJECT |
| DR-003 | OAuth tokens only in libsecret/keyring | REJECT |
| DR-004 | Stream URLs not persisted to database | REJECT |
| DR-005 | DRM code isolated in separate process | REJECT |
| DR-006 | All API calls go through `StreamingService` interface | REJECT |
| DR-007 | Error types use `StreamingError` enum | REQUEST CHANGES |
| DR-008 | New endpoints documented in this spec | REQUEST CHANGES |
| DR-009 | Unit tests cover new code paths | REQUEST CHANGES |
| DR-010 | Layer boundaries respected per Section 3.2 | REJECT |

### 9.2 API Change Response Protocol

When a streaming service API changes:

```
1. A-TEST detects API change (mock server mismatch or integration test failure)
        │
        ▼
2. A-HOOK implements backward-compatible adapter
   - New endpoint handler
   - Fallback to old endpoint if new fails
   - Version detection logic
        │
        ▼
3. Update this spec document (Section 8)
   - Document new endpoint
   - Document response format changes
   - Note deprecation timeline if known
        │
        ▼
4. A-TEST updates mocks and test cases
        │
        ▼
5. A-BUILD increments version, triggers release
```

### 9.3 Forbidden Patterns

**NEVER DO THIS**:

```cpp
// FORBIDDEN: Network call in SoundSource
class BadSoundSource : public SoundSource {
    ReadableSampleFrames readSampleFramesClamped(...) override {
        // VIOLATION: Blocking network call in audio thread
        auto data = httpClient.get(streamUrl);  // ❌ FORBIDDEN
        return decode(data);
    }
};

// FORBIDDEN: Plain text token storage
void saveToken(const QString& token) {
    QFile file("~/.config/mixxx/tokens.json");  // ❌ FORBIDDEN
    file.write(token.toUtf8());
}

// FORBIDDEN: Persisting stream URL
void saveTrack(const TrackMetadata& meta, const StreamInfo& stream) {
    query.prepare("UPDATE library SET stream_url = ? WHERE id = ?");
    query.bindValue(0, stream.streamUrl.toString());  // ❌ FORBIDDEN
}

// FORBIDDEN: DRM code in main process
void decryptChunk(const QByteArray& encrypted) {
    Widevine::CDM cdm;  // ❌ GPL VIOLATION
    return cdm.decrypt(encrypted);
}
```

**ALWAYS DO THIS**:

```cpp
// CORRECT: Lock-free buffer read
class GoodSoundSource : public SoundSource {
    ReadableSampleFrames readSampleFramesClamped(...) override {
        size_t available = m_ringBuffer.tryRead(dest, requested);
        if (available < requested) {
            std::fill(dest + available, dest + requested, 0);  // Silence
            emit bufferUnderrun();
        }
        return available;
    }
};

// CORRECT: Keyring token storage
void saveToken(const QString& token) {
    secret_password_store_sync(&schema, nullptr, "Mixxx OAuth", token.toUtf8(), ...);
}

// CORRECT: Only cache transient URL in memory
class StreamCache {
    QHash<QString, QPair<QUrl, QDateTime>> m_urlCache;  // In-memory only
};

// CORRECT: IPC to separate CDM process
QByteArray decryptChunk(const QByteArray& encrypted) {
    m_sharedMem->write(encrypted);
    m_cdmSocket->write("decrypt\n");
    m_cdmSocket->waitForReadyRead();  // Safe - not audio thread
    return m_sharedMem->read();
}
```

### 9.4 Dependency Isolation

To prevent upstream Mixxx changes from breaking streaming features:

```
src/
├── sources/
│   ├── soundsource.h         # UPSTREAM - DO NOT MODIFY
│   └── soundsource.cpp       # UPSTREAM - DO NOT MODIFY
│
├── streaming/                # KINETIC - All streaming code here
│   ├── sources/
│   │   ├── soundsourceproxy.h    # Inherits, doesn't modify
│   │   └── soundsourcestream.h
│   ├── hook/
│   ├── bridge/
│   └── CMakeLists.txt        # Separate build target
│
└── library/
    └── dao/
        ├── trackdao.h        # UPSTREAM - DO NOT MODIFY
        └── trackdao_streaming.cpp  # KINETIC - Extension only
```

**Merge Strategy**:
- Upstream Mixxx changes merged into `main` branch
- Kinetic features in `kinetic` branch
- Regular rebasing of `kinetic` onto `main`
- CI validates no upstream files modified

---

## 10. Delivery Roadmap

### 10.1 Phase Overview

| Phase | Name | Duration | Deliverables |
|-------|------|----------|--------------|
| P0 | Foundation | - | Build system, base interfaces |
| P1 | Hook Layer | - | OAuth, Beatport metadata |
| P2 | Bridge Layer | - | FUSE driver, sparse cache |
| P3 | Source Layer | - | SoundSourceProxy, ring buffer |
| P4 | Integration | - | End-to-end Beatport playback |
| P5 | Tidal | - | Tidal API, DRM handling |
| P6 | Polish | - | UI, offline mode, testing |
| P7 | Release | - | AUR package, documentation |

### 10.2 Phase Details

#### P0: Foundation

**Agent**: A-BUILD

**Deliverables**:
1. CMake module for streaming components
2. Directory structure per Section 4
3. Base interface headers
4. CI pipeline for Arch Linux builds

**Exit Criteria**:
- `cmake -DENABLE_STREAMING=ON ..` succeeds
- Empty stub classes compile
- Unit test framework configured

#### P1: Hook Layer

**Agent**: A-HOOK

**Deliverables**:
1. `OAuthManager` with token refresh
2. `BeatportService` implementation
3. `MetadataNormalizer` for keys/artists
4. Token storage via libsecret

**Exit Criteria**:
- Can authenticate with Beatport
- Can fetch track metadata
- Can retrieve stream URL
- Tokens persist across restarts

#### P2: Bridge Layer

**Agent**: A-BRIDGE

**Deliverables**:
1. `mixxx-fs` FUSE daemon
2. `SparseCache` with interval tree
3. `RangeFetcher` for HTTP Range requests
4. IPC protocol implementation

**Exit Criteria**:
- Virtual files appear in mount point
- `cat /run/user/.../beatport/123.aac` streams audio
- Cache persists partial downloads
- Seeking triggers range requests

#### P3: Source Layer

**Agent**: A-SOURCE

**Deliverables**:
1. `SoundSourceProxy` implementation
2. `StreamRingBuffer` lock-free buffer
3. Silence injection on underrun
4. `AnalyzerThrottle` for passive analysis

**Exit Criteria**:
- Engine can load virtual file
- Playback works with prefetched data
- Seek to uncached region recovers gracefully
- No XRUNs under normal network conditions

#### P4: Integration

**Agents**: A-DAO, A-UI, A-TEST

**Deliverables**:
1. Database schema migration
2. Library browser for Beatport
3. Waveform download overlay
4. Integration test suite

**Exit Criteria**:
- Can search Beatport from UI
- Can load and play Beatport track
- Waveform shows download progress
- End-to-end tests pass

#### P5: Tidal

**Agents**: A-HOOK, A-BRIDGE

**Deliverables**:
1. `TidalService` implementation
2. Device flow authentication
3. DASH manifest parsing
4. CDM Host for Widevine

**Exit Criteria**:
- Can authenticate with Tidal
- Can play Tidal HiFi (unencrypted)
- Can play Tidal Master (Widevine)

#### P6: Polish

**Agents**: A-UI, A-TEST, A-BUILD

**Deliverables**:
1. Offline mode UI
2. Cache management settings
3. Performance optimization
4. Stress testing

**Exit Criteria**:
- Offline tracks playable without network
- No memory leaks under prolonged use
- Buffer underrun rate < 0.1% on stable network

#### P7: Release

**Agent**: A-BUILD

**Deliverables**:
1. AUR `PKGBUILD`
2. System tuning scripts
3. User documentation
4. Release notes

**Exit Criteria**:
- `yay -S mixxx-kinetic-git` succeeds
- Post-install configuration works
- README covers all features

---

## 11. Future Integration Framework

### 11.1 Adding New Streaming Services

To add a new service (e.g., SoundCloud, Amazon Music):

**Step 1**: Implement `StreamingService` interface

```cpp
// src/streaming/hook/newservice.h
class NewService : public StreamingService {
    Q_OBJECT
public:
    QString serviceName() const override { return "New Service"; }
    QString serviceId() const override { return "newservice"; }

    // Implement all pure virtual methods...
};
```

**Step 2**: Register service in factory

```cpp
// src/streaming/hook/servicefactory.cpp
void ServiceFactory::registerServices() {
    m_services["beatport"] = std::make_unique<BeatportService>();
    m_services["tidal"] = std::make_unique<TidalService>();
    m_services["newservice"] = std::make_unique<NewService>();  // Add here
}
```

**Step 3**: Update database enum

```sql
-- Add to valid remote_source values
-- (No schema change needed, TEXT column accepts any value)
-- Document in this spec under Section 8
```

**Step 4**: Add UI components

```cpp
// src/widget/streaming/newservicelogin.h
class NewServiceLoginWidget : public StreamingLoginWidget {
    // Service-specific login UI if needed
};
```

**Step 5**: Update tests

```cpp
// src/test/streaming/hook/newservice_test.cpp
TEST(NewServiceTest, FetchMetadata) { ... }
TEST(NewServiceTest, GetStreamUrl) { ... }
```

### 11.2 Handling API Deprecations

When a streaming service deprecates an endpoint:

```
┌─────────────────────────────────────────────────────────────────┐
│                   API DEPRECATION RESPONSE                      │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  1. DETECT                                                      │
│     - Monitor for deprecation headers (Sunset, Deprecation)     │
│     - Log warnings on deprecated endpoint use                   │
│                                                                 │
│  2. ADAPT                                                       │
│     - Implement new endpoint in parallel                        │
│     - Feature flag: streaming.beatport.use_v5_api = false      │
│                                                                 │
│  3. MIGRATE                                                     │
│     - Enable new endpoint by default                            │
│     - Keep old endpoint as fallback                             │
│                                                                 │
│  4. REMOVE                                                      │
│     - After deprecation deadline, remove old code               │
│     - Update this spec document                                 │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### 11.3 Protocol Evolution

For future protocol changes (e.g., HTTP/3, new DRM systems):

**Transport Layer Abstraction**:

```cpp
// src/streaming/bridge/transport.h
class TransportLayer {
public:
    virtual ~TransportLayer() = default;
    virtual QFuture<QByteArray> fetch(const QUrl& url,
                                       const QHash<QString, QString>& headers) = 0;
    virtual QFuture<QByteArray> fetchRange(const QUrl& url,
                                            qint64 start, qint64 end) = 0;
};

class CurlTransport : public TransportLayer { /* Current */ };
class QuicTransport : public TransportLayer { /* Future HTTP/3 */ };
```

**DRM Layer Abstraction**:

```cpp
// src/streaming/bridge/drm.h
class DRMHandler {
public:
    virtual ~DRMHandler() = default;
    virtual bool canHandle(const QString& encryptionType) const = 0;
    virtual QFuture<QByteArray> decrypt(const QByteArray& encrypted,
                                         const QString& keyId,
                                         const QString& licenseUrl) = 0;
};

class WidevineDRM : public DRMHandler { /* Current */ };
class FairPlayDRM : public DRMHandler { /* Future Apple */ };
```

### 11.4 Cross-Platform Considerations

While this spec targets EndeavourOS/Arch, future cross-platform support requires:

| Component | Linux | macOS | Windows |
|-----------|-------|-------|---------|
| FUSE | libfuse3 | macFUSE | WinFsp |
| Keyring | libsecret | Keychain | Credential Manager |
| Audio | PipeWire | CoreAudio | WASAPI |
| DRM Process | fork() | fork() | CreateProcess() |

**Abstraction Points**:
- `SecretStorage` interface for keyring
- `FuseDriver` with platform-specific implementations
- Build system conditionals in CMake

---

## Appendix A: Configuration Schema

**File**: `~/.config/mixxx/kinetic.conf`

```ini
[General]
# Enable streaming features
enabled=true

[Beatport]
# OAuth client credentials (user-supplied if default revoked)
client_id=default
client_secret=default

# Preferred audio quality: low, high
quality=high

[Tidal]
# OAuth client credentials
client_id=default

# Audio quality: LOW, HIGH, LOSSLESS, HI_RES
quality=LOSSLESS

[Cache]
# Sparse cache location (set to tmpfs path for RAM cache)
path=/var/cache/mixxx/streaming

# Maximum cache size in MB (0 = unlimited)
max_size_mb=10240

# Offline library location
offline_path=~/.mixxx/music/offline

[Network]
# Prefetch buffer size in seconds
prefetch_seconds=30

# HTTP request timeout in seconds
timeout_seconds=10

# Retry count for failed requests
retry_count=3

[Debug]
# Enable verbose logging
verbose=false

# Log HTTP requests/responses
log_http=false
```

---

## Appendix B: Glossary

| Term | Definition |
|------|------------|
| **Bridge** | Virtualization layer that presents streams as local files |
| **CDM** | Content Decryption Module (Widevine) |
| **DASH** | Dynamic Adaptive Streaming over HTTP |
| **FUSE** | Filesystem in Userspace |
| **HLS** | HTTP Live Streaming |
| **Hook** | API interaction layer |
| **Interval Tree** | Data structure tracking cached byte ranges |
| **Kinetic Library** | Streaming-based music collection (vs. static local files) |
| **Ring Buffer** | Lock-free circular buffer for audio samples |
| **Sparse Cache** | Partial file cache supporting random access |
| **TTL** | Time To Live (URL expiration) |
| **XRUN** | Audio buffer underrun causing audible glitch |

---

## Appendix C: File Manifest

```
src/streaming/
├── CMakeLists.txt
├── hook/
│   ├── beatportservice.cpp
│   ├── beatportservice.h
│   ├── metadatanormalizer.cpp
│   ├── metadatanormalizer.h
│   ├── oauthmanager.cpp
│   ├── oauthmanager.h
│   ├── streamingdto.h
│   ├── streamingerror.h
│   ├── streamingservice.h
│   ├── tidalservice.cpp
│   ├── tidalservice.h
│   └── tokeninterceptor.cpp
├── bridge/
│   ├── cdmhost.cpp
│   ├── cdmhost.h
│   ├── fusedriver.cpp
│   ├── fusedriver.h
│   ├── mountmanager.cpp
│   ├── mountmanager.h
│   ├── prefetcher.cpp
│   ├── prefetcher.h
│   ├── rangefetcher.cpp
│   ├── rangefetcher.h
│   ├── sparsecache.cpp
│   └── sparsecache.h
├── sources/
│   ├── analyzerthrottle.cpp
│   ├── analyzerthrottle.h
│   ├── soundsourceproxy.cpp
│   ├── soundsourceproxy.h
│   ├── soundsourcestream.cpp
│   ├── soundsourcestream.h
│   ├── streamringbuffer.cpp
│   └── streamringbuffer.h
└── widget/
    ├── bufferindicator.cpp
    ├── bufferindicator.h
    ├── loginwidget.cpp
    ├── loginwidget.h
    ├── offlinemanager.cpp
    ├── offlinemanager.h
    ├── streaminglibraryfeature.cpp
    ├── streaminglibraryfeature.h
    ├── waveformoverlay.cpp
    └── waveformoverlay.h

tools/mixxx-fs/
├── CMakeLists.txt
├── main.cpp
└── mixxx-fs.service

packaging/arch/
├── PKGBUILD
├── mixxx-kinetic.install
└── system-tuning/
    ├── 99-mixxx-network.conf
    ├── 99-mixxx-realtime.conf
    └── mixxx.conf

src/test/streaming/
├── hook/
│   ├── beatportservice_test.cpp
│   ├── mock_httpserver.cpp
│   ├── mock_httpserver.h
│   ├── oauthmanager_test.cpp
│   └── tidalservice_test.cpp
├── bridge/
│   ├── fusedriver_test.cpp
│   ├── mock_fuse.h
│   ├── rangefetcher_test.cpp
│   └── sparsecache_test.cpp
├── sources/
│   ├── ringbuffer_test.cpp
│   └── soundsourcestream_test.cpp
├── integration/
│   ├── full_stream_test.cpp
│   └── offline_transition_test.cpp
└── benchmarks/
    ├── buffer_latency_bench.cpp
    └── cache_throughput_bench.cpp
```

---

**Document Control**

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0.0 | 2026-01-18 | Spec Agent | Initial specification |

**End of Specification**
