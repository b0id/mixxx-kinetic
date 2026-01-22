# Mixxx-Kinetic Developer Log

> **Purpose**: Track architectural decisions, current state, and session progress.
> **Structure**: Current state at top, detailed history at bottom.
> **Anti-Pattern**: Avoid verbose play-by-play - focus on decisions and outcomes.

---

## 🎯 Current State (2026-01-22)

### Build Status
- ✅ **mixxx**: Compiles clean
- ✅ **mixxx-fs**: Compiles clean
- ✅ **mixxx-test**: Links successfully, all TokenInterceptor/RangeFetcher tests passing
- ⚠️ **Source Layer**: Temporarily disabled (see BLOCKERS.md)

### Component Status

| Layer | Component | Status | Notes |
|-------|-----------|--------|-------|
| **A-HOOK** | OAuthManager | ✅ Complete | Device flow, token refresh, keyring storage |
| | BeatportService | ✅ Complete | Metadata, search, stream URLs |
| | TidalService | ❌ Not Started | - |
| | TokenInterceptor | ✅ Complete | Auto-401 refresh handler using Signals |
| **A-BRIDGE** | FuseDriver | ✅ Complete | FUSE ops, URL mapping |
| | SparseCache | ✅ Complete | Interval tree, file backing, JSON persistence |
| | RangeFetcher | ✅ Complete | HTTP Range requests, Thread-safe |
| | **Prefetcher** | ✅ **Complete** | Dual-mode prefetch (NEW) |
| | BridgeClient/Server | ✅ Complete | IPC via Unix socket |
| **A-SOURCE** | SoundSourceKineticProxy | ✅ Complete | Manifest parsing, Bridge integration |
| | StreamRingBuffer | ✅ Complete | Lock-free SPSC queue |
| **A-DAO** | Schema Extensions | ❌ Not Started | - |
| **A-UI** | Login Widgets | ✅ Complete | Browser-based Flow, Sidebar Integration |
| **A-TEST** | Unit Tests | 🟢 Good | E2E Headless Playback Verified |

### Active Blockers
See **[BLOCKERS.md](BLOCKERS.md)** for detailed tracking.

**Summary**: No active blockers ✅
*Warning*: `mixxx-fs` must be running for playback to work.

### Next Session Priorities
1.  **Catalog Integration**: Fetch real data from Beatport API for Sidebar.
2.  **Streaming Playback**: Connect Proxy to real Stream URLs.
3.  **Library Integration**: DAO layer for caching metadata.

---

## 📐 Key Architectural Decisions

### Decision Log

| Date | Decision | Rationale |
|------|----------|-----------|
| 2026-01-22 | **Signal-Based Retry for TokenInterceptor** | `QFuture` is not returned by `QNetworkAccessManager::createRequest`. Using signals (`tokenRefreshed`, `authError`) allows `RetryingNetworkReply` to pause and resume asynchronously without blocking threads or relying on complex future chains for the initial request. |
| 2026-01-22 | **Stack-Allocated QNAM in RangeFetcher** | `rangefetcher` runs in a worker thread. Creating `QNetworkAccessManager` on the stack ensures it lives in the correct thread, avoiding "Thread Affinity" warnings and potential crashes seen when sharing a single instance across threads. |
| 2026-01-20 | Prefetcher uses single worker thread | Simpler than thread pool, sufficient for streaming workload |
| 2026-01-20 | InodeNumber typedef instead of fuse_ino_t | Avoid FUSE header dependency in mixxx-lib |
| 2026-01-19 | FUSE daemon separate from main process | Isolation, privilege separation, crash safety |

### Interface Contracts (Source of Truth)

**TokenInterceptor Signals**:
```cpp
signals:
    void tokenRefreshed(const QString& serviceId);
    void authError(const QString& serviceId, const QString& errorMsg);
```

---

## 🔄 Recent Sessions (Detailed)

### [2026-01-22] Session 3: Vertical Slice Prototype ✅

**Objective**: Verify the complete end-to-end streaming pipeline (Metadata -> Manifest -> Proxy -> Bridge -> FUSE) without UI implementation.

**Accomplished**:
1.  **Manifest Parsing**:
    -   Updated `SoundSourceKineticProxy` to support `.knt` JSON manifests.
    -   Proxy now extracts the remote URL and tracks extension/bitrate instead of trying to play the local JSON file.
2.  **Mock Infrastructure**:
    -   Created `MockSoundSourceProvider` to simulate an MP3 decoder for testing environments lacking system codecs.
    -   Used `FRIEND_TEST` to inject the mock provider into the private `SoundSourceProxy` registry.
3.  **Headless Verification**:
    -   Developed `headless_playback_test.cpp`.
    -   **Result**: Validated that providing a manifest file correctly triggers the Bridge registration, FUSE mount, and audio data read.
    -   The test proved the backend architecture works flawlessly in isolation.

**Key Outcome**: The "Vertical Slice" is complete. We have a working streaming engine that just needs a UI to feed it manifests.

---

### [2026-01-22] Session 2: Production Hardening

**Objective**: Ensure robust networking (auto-refresh tokens) and thread safety.

**Accomplished**:
1.  **TokenInterceptor Implementation**:
    -   Created `TokenInterceptor` (QNAM subclass) and `RetryingNetworkReply`.
    -   Intercepts 401 Unauthorized responses transparently.
    -   Triggers `OAuthManager::refreshTokens` and waits for `tokenRefreshed` signal.
    -   Retries original request with new token automatically.
    -   **Validation**: Created `TokenInterceptorTest` verifying 401 interception, refresh trigger, and successful retry.

2.  **RangeFetcher Threading Fix**:
    -   Identified QNAM thread affinity warning (QNAM created in main, used in worker).
    -   Moved QNAM instantiation to `RangeFetcher::fetch` (stack-allocated) to ensure it runs in the worker thread.
    -   Verified via code review and regression testing.

3.  **Refactoring**:
    -   Switched `TokenInterceptor` from Future-based to Signal-based to handle asynchronous nature of `createRequest` properly.

**Status**: Networking layer is now self-healing (auth) and thread-safe.

---

### [2026-01-20] Integration Testing: Pipeline Validated ✅
*(See previous logs)*

---

### [2026-01-22] Session 4: UI & Auth Integration ✅

**Objective**: Implement user-facing components (Login, Sidebar) and robust Authentication.

**Accomplished**:
1.  **Authentication Overhaul**:
    -   Switched from Device Code flow to **PKCE Browser Flow** (Beatport requirement).
    -   Implemented a temporary **Local HTTP Server** (port 8889) to capture the auth callback code safely.
    -   Integrated `MixxxKeychain` (mocked/stubbed) for secure token storage.
    -   **Result**: Clicking "Login" now opens the browser, user logs in on Beatport, and Mixxx automatically captures the token.

2.  **UI Integration**:
    -   **Preferences**: Created `DlgPrefStreaming` and embedded a status-aware `LoginDialog`.
    -   **Sidebar**: Implemented `BeatportFeature` and registered it in `Library`.
    -   **Visuals**: Sidebar now displays "Beatport" with a "Search" node.

3.  **Architecture**:
    -   Updated `Library` dependency injection to accept `StreamingService`.
    -   Updated build system (`CMakeLists.txt`) and unit tests (`controllerscriptenginelegacy_test`, etc.) to match new signatures.
    -   Ensured `BeatportFeature` uses `TreeItemModel` correctly, fixing build errors.

**Key Outcome**: The UI shell is complete. Users can log in and see the feature. The next step is populating the feature with real data (Catalog).
