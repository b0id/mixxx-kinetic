# Mixxx-Kinetic Active Blockers

This document tracks critical gaps preventing the "Vertical Slice" from being fully usable by a user.

## 🚧 Active Blockers

### BLOCKER-003: Empty Catalog Data
**Status**: 🔴 OPEN
**Priority**: CRITICAL
**Impact**: Users can see "Beatport" in the sidebar but cannot browse or search tracks.
**Dependencies**: `BeatportService` needs to query the API and populate `BeatportFeature`'s `TreeItemModel`.

### BLOCKER-004: Disconnected Playback Pipeline
**Status**: 🔴 OPEN
**Priority**: CRITICAL
**Impact**: Even if a track is listed, loading it does not trigger the "Stream URL -> Manifest -> FUSE" pipeline.
**Dependencies**: `BeatportService` needs to resolve Stream URLs and `ManifestGenerator` needs to create `.knt` files on load.

### BLOCKER-005: Manual Daemon Lifecycle
**Status**: 🟡 OPEN
**Priority**: HIGH
**Impact**: User must manually run `mixxx-fs` in a terminal before starting Mixxx. if it crashes, playback dies.
**Dependencies**: `FuseDaemonManager` (Phase 3) is not implemented.

---

## 📋 Resolved History

- **BLOCKER-001**: SoundSourceKineticProxy Interface Mismatch (Fixed 2026-01-20)
- **BLOCKER-000**: FUSE3 CMake Detection (Fixed 2026-01-18)

---

## 🔍 Watch List
-   **OAuth Token Refresh**: Implemented but needs stress testing (expiry simulation).
-   **Stream URL Expiry**: URLs expire after ~1 hour; no refresh logic implemented yet.
