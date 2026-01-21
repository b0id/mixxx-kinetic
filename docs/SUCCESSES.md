📋 SBAR Analysis & 4-5 Day Roadmap

Situation (Current State)

- ✅ All 4 architectural layers working (Hook, Bridge, Source, partial  
  DAO)
- ✅ Integration tests prove priority queue works - the hard part is  
  DONE
- ✅ Zero active blockers - clean build, 2,700 lines of production code
- ⚠️ Missing: Real stream testing, additional services, production  
  hardening  


Background (Context)

- Architecture is validated: Tests proved prefetcher interrupts  
  sequential fetches for seeks
- Service abstraction works: Bridge is URL-agnostic  
  (Beatport/Tidal/SoundCloud all use same pipeline)
- Strategic value: Users want Beatport + Tidal + SoundCloud
- Technical debt: Minor threading issue (QNetworkAccessManager), no  
  real-stream validation yet  


Assessment (Risk Analysis)

HIGH VALUE, LOW RISK (Do First):

1. TokenInterceptor - Production blocker (sessions expire, need  
   auto-refresh)
2. Real Stream Test - Validate with actual Beatport API (prove it works
   end-to-end)
3. TidalService - High user value, low complexity (copy BeatportService
   pattern)  


HIGH VALUE, MEDIUM RISK:  
 4. SoundCloudService - User demand, similar to Tidal  
 5. Threading Fix - QNetworkAccessManager affinity warning

HIGH VALUE, HIGH RISK (Defer):  
 6. CDM Host (Widevine DRM) - Complex, only needed for Tidal HiFi/Master 7. UI Integration - Requires QML knowledge, design decisions

Recommendation (4-5 Day Plan)

Day 1: Production Hardening (3-4 hours)

Priority: Critical for production reliability

1. Implement TokenInterceptor (~2 hours)
   - Wrap QNetworkAccessManager
   - Detect 401 responses
   - Call OAuthManager::refreshTokens()
   - Retry request with new token  

2. Fix RangeFetcher threading (~1 hour)
   - Create QNetworkAccessManager in worker thread
   - Eliminate affinity warnings  

3. Test: Verify token refresh flow  


Deliverable: Sessions never expire, no threading warnings

---

Day 2: Proof of Concept (2-3 hours)

Priority: Validate architecture with real data

1. Get Beatport API credentials (~30 min)
   - Register developer account
   - Obtain client_id/client_secret  

2. Real Stream Test (~2 hours)
   - Run OAuth device flow
   - Fetch real track metadata
   - Get real HLS stream URL
   - Register with mixxx-fs
   - PLAY AUDIO through full pipeline!  

3. Tune Prefetcher based on results
   - Adjust chunk size if needed
   - Measure cache hit rate  


Deliverable: First real Beatport track plays through Mixxx

---

Day 3: Service Expansion (3-4 hours)

Priority: Maximize user value

1. TidalService (~2 hours)
   - Copy beatportservice.cpp → tidalservice.cpp
   - Update API endpoints:
     - /v1/tracks/{id}
     - /v1/tracks/{id}/streamUrl
   - Update OAuth scopes
   - Test with Tidal Standard (no DRM)  

2. SoundCloudService (~2 hours)
   - Similar copy-paste pattern
   - API: https://api.soundcloud.com/tracks/{id}
   - Usually simpler than Tidal (no tiered quality)  


Deliverable: 3 streaming services working (Beatport, Tidal, SoundCloud)

---

Day 4: Integration & Polish (3-4 hours)

Priority: Make it usable

1. Unified Service Registry (~1 hour)
   - Create ServiceManager
   - Register all 3 services
   - Handle service selection  

2. Library Integration Stub (~2 hours)
   - Create StreamingTrackSource
   - Hook into Mixxx library browser
   - Basic search UI (text input → service search)
   - Display results in track table  

3. Documentation
   - Update KINETIC_DEVLOG.md
   - Add usage instructions  


Deliverable: Users can search and play from all 3 services

---

Day 5: Refinement (2-3 hours)

Priority: Production readiness

1. Error Handling Polish
   - Network timeout handling
   - Graceful degradation on service outage
   - User-friendly error messages  

2. Metrics & Logging
   - Add prefetch efficiency metrics
   - Log cache hit rates
   - Track bandwidth usage  

3. Performance Testing
   - Multiple concurrent streams
   - Seek performance under load
   - Memory usage validation  


Deliverable: Production-ready streaming system

---

🎯 Critical Path (Must-Do)

Day 1: TokenInterceptor → Day 2: Real Stream Test → Day 3:  
 Tidal/SoundCloud

Everything else is gravy. If you only do Days 1-3, you'll have 3 working
streaming services with auto-token-refresh.

🚫 Explicit Deprioritization

DEFER (Complex, Low Immediate Value):

- ❌ Widevine CDM Host (only for Tidal HiFi/Master - most users use  
  Standard)
- ❌ Advanced UI (QML learning curve, design bikeshedding)
- ❌ Database schema extensions (works without it via in-memory  
  metadata)
- ❌ Waveform rendering for streams (complex, not essential)  


Why defer: These are "nice-to-haves" that multiply complexity. Get music
playing first.

---

📝 Note for Future Me

State on 2026-01-20 @ 22:45:

- All docs updated: KINETIC_DEVLOG.md, BLOCKERS.md current
- Integration tests prove architecture: kinetic_integration_test.cpp  
  (240 lines, 6 tests)
- Prefetcher priority queue VALIDATED: seeks interrupt sequential  
  fetches
- Build clean: mixxx (567MB), mixxx-test (721MB), mixxx-fs (3.1MB)
- Zero blockers, ready for production hardening  


Next Session: Start Day 1 (TokenInterceptor) - code is in  
 src/streaming/hook/

---

🎯 What's Next: TokenInterceptor (Start Here)

Want me to implement TokenInterceptor now, or are you good to pick this
up next session?

The architecture is PROVEN. Now we make it bulletproof.
