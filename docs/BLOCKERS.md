# Mixxx-Kinetic Active Blockers

This document tracks components that are temporarily disabled or broken to prevent architectural drift.

## 🚧 Active Blockers

_No active blockers at this time._

---

## 📋 Completed Blockers

### BLOCKER-001: SoundSourceKineticProxy Interface Mismatch
**Status**: ✅ RESOLVED
**Priority**: HIGH (Blocked end-to-end playback testing)
**Created**: 2026-01-18
**Resolved**: 2026-01-20
**Time to Resolve**: ~1 hour

**Problem**:
`SoundSourceKineticProxy` didn't match Mixxx's actual `AudioSource`/`SoundSource` interface hierarchy.

**Solution**:
Studied `src/sources/audiosource.h` and `src/sources/soundsourceffmpeg.cpp` to understand correct API usage:

1. **SampleBuffer construction**:
   ```cpp
   // Fixed: Use size-only constructor
   mixxx::SampleBuffer sampleBuffer(chunkSizeSamples);
   ```

2. **WritableSlice construction**:
   ```cpp
   // Fixed: Wrap buffer in WritableSlice
   mixxx::SampleBuffer::WritableSlice writableSlice(sampleBuffer);
   ```

3. **WritableSampleFrames construction**:
   ```cpp
   // Fixed: Pass slice to WritableSampleFrames
   mixxx::WritableSampleFrames writableFrames(indexRange, writableSlice);
   ```

4. **Reading data**:
   ```cpp
   // Fixed: Read from ReadableSampleFrames.readableData()
   const CSAMPLE* pSrc = readFrames.readableData();
   ```

5. **Seeking**: Removed invalid `seekSampleFrame()` call - proxy handles discontinuities via frame index tracking

**Files Modified**:
- `src/sources/soundsourcekineticproxy.cpp` - Fixed `readWorker()` method
- `src/streaming/CMakeLists.txt` - Re-enabled Source layer files
- `CMakeLists.txt` - Re-enabled test file
- `src/sources/soundsourceproxy.cpp` - Re-enabled provider registration

**Verification**:
- ✅ Build successful: `mixxx` (567MB), `mixxx-test` (721MB)
- ✅ No compilation errors
- ✅ All components linked successfully

**Key Insight**:
The Mixxx audio pipeline uses a slice-based API to avoid copying. Always wrap buffers in `WritableSlice` / `ReadableSlice` before passing to `WritableSampleFrames` / `ReadableSampleFrames`.

### BLOCKER-000: FUSE3 CMake Detection
**Status**: RESOLVED
**Resolved**: 2026-01-18
**Solution**: Used `pkg_check_modules(FUSE3 REQUIRED fuse3)` instead of FindFUSE3.cmake

---

## 🔍 Monitoring for Drift

### Watch List
Components that work but need monitoring:

1. **MOC Integration**: Manual `#include "moc_*.cpp"` pattern
   - If MOC warnings appear, revisit `SKIP_AUTOMOC` settings

2. **Qt6 Keychain**: Conditionally compiled with `#ifdef __QTKEYCHAIN__`
   - Ensure fallback behavior is acceptable if unavailable

3. **Prefetcher Thread Lifecycle**: Single worker thread per FuseDriver instance
   - Monitor for thread leaks or deadlocks in production

---

## 📝 How to Use This Document

**When Disabling Code**:
1. Add entry to "Active Blockers" section
2. Include file locations with line numbers
3. Document symptoms and root cause
4. Define clear resolution path
5. Update `Last Updated` timestamp

**When Resolving**:
1. Move to "Completed Blockers" section
2. Document solution
3. Remove comments from source files
4. Verify build passes
5. Update KINETIC_DEVLOG.md with resolution

**Weekly Review**:
- Check if blockers older than 2 weeks need escalation
- Re-prioritize based on impact to delivery roadmap
