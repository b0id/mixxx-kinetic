You're absolutely right - let me address BeatportAPI.md properly:

What BeatportAPI.md Actually Tells Us

That document IS the official Partner Portal documentation and it provides:

✓ What They Document:

- OAuth Authorization Code + PKCE flow (the "official" way)
- Token lifetimes: Access 10 min, Refresh 31 days (though we know the 10 min is wrong)
- Scope format: app:externaltrusted user:dj openid
- Refresh token rotation (single-use, get new one each refresh)
- /o/introspect/ for token validation
- /o/userinfo/ for user profile (OpenID)  


✗ What They DON'T Document:

- How to GET a client_id (says "provided to client application" - but no registration portal)
- The /catalog/\* endpoints (search, tracks, streaming)
- The /my/\* endpoints (subscriptions, library)
- Password grant flow (which beatportdl uses)
- What the scopes actually MEAN functionally  


Critical Insight: The official docs tell you what credentials you need, but not how to get them OR what to do
with them once authenticated. It's like having a door manual without an address or key.

So yes - BeatportAPI.md gives the official criteria, but beatportdl shows the functional reality. We need  
 both.
