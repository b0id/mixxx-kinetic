# Streaming API Integration - Lessons Learned

**Date**: 2026-01-22
**Context**: Implementing Beatport and Tidal streaming in Mixxx

---

## 🎯 Key Takeaway

**Not all OAuth is created equal.** Public API credentials designed for browser-based SPAs (Single Page Applications) are fundamentally incompatible with desktop application OAuth flows, even when both use "OAuth 2.0 with PKCE."

---

## 📚 What We Learned

### 1. **Client Credentials Have Hidden Constraints**

When you extract a `client_id` from a public API (like Beatport's Swagger UI), you get:
- ✅ A valid credential that authenticates
- ❌ Redirect URIs locked to specific patterns
- ❌ Scopes that may be implicit (can't request explicitly)
- ❌ OAuth flow expectations (post-message vs localhost)

**Lesson**: Just because a `client_id` exists doesn't mean it works for your use case.

---

### 2. **OAuth Redirect Patterns Are Not Interchangeable**

#### Browser SPA Pattern (Beatport):
```
1. Open auth URL in browser
2. User logs in
3. Redirect to: https://api.service.com/auth/o/post-message/?code=ABC
4. JavaScript calls: window.postMessage(code, parentOrigin)
5. Parent window (SPA) receives code
6. SPA exchanges code for token
```

**Requirements**: Parent window, same-origin policy, `target` parameter

#### Desktop App Pattern (Tidal):
```
1. Start local HTTP server on localhost:PORT
2. Open auth URL in browser with redirect_uri=http://localhost:PORT/callback
3. User logs in
4. Redirect to: http://localhost:PORT/callback?code=ABC
5. Local server captures HTTP request
6. Extract code from URL query parameter
7. Exchange code for token
```

**Requirements**: Redirect URI registered for localhost, ability to run local server

**Lesson**: Desktop apps need localhost redirect support. Post-message endpoints don't work.

---

### 3. **PKCE Is Security, Not Magic**

PKCE (Proof Key for Code Exchange) protects authorization codes from interception, but:
- ✅ Prevents MITM attacks on the authorization code
- ❌ Doesn't make incompatible OAuth flows compatible
- ❌ Doesn't bypass redirect_uri mismatch errors

Beatport's public client doesn't work for desktop **not because of PKCE**, but because of redirect URI restrictions.

**Lesson**: PKCE is orthogonal to redirect URI compatibility.

---

### 4. **Scope Syntax Varies Wildly**

| Service | Scope Format | Example |
|---------|--------------|---------|
| Beatport | Implicit (empty) | `scope=""` or omit entirely |
| Tidal (Legacy) | Space-separated | `scope="r_usr streaming"` |
| Tidal (Modern) | Single keyword | `scope="playback"` |
| Spotify | Space-separated | `scope="user-read-private streaming"` |

**Lesson**: Read the current docs. Legacy scopes may fail silently or cause errors.

---

### 5. **Developer Portals Hide Critical Info**

**Tidal Developer Portal**: Clear, well-documented, works as expected
**Beatport "API"**: No public developer portal, partner-only access, workaround via Swagger UI

**What's missing from public docs**:
- Which redirect URIs are actually allowed
- Production vs development environment differences
- Whether `localhost` is acceptable
- Scope deprecation notices

**Lesson**: Assume the docs are incomplete. Test early, test often.

---

### 6. **Error Codes Are Opaque**

- **Beatport `invalid_scope`**: Could mean wrong scope syntax, disallowed scope, or implicit scope requirement
- **Beatport `invalid_request` → "Provide a valid target URL"**: Post-message endpoint needs `target` parameter
- **Tidal Error 11102**: Redirect URI not registered (not documented anywhere)
- **Tidal Error 1002**: Conflicting info - some say "missing countryCode", likely wrong scope

**Lesson**: Error codes rarely explain the actual fix. Requires trial-and-error + community research.

---

### 7. **"Public API" Doesn't Mean "Free to Use"**

**Beatport**: Has a public Swagger UI, but API access is partner-only
**Tidal**: Has a free developer portal with self-service registration

**Gray Area**: Extracting `client_id` from browser developer tools
- ✅ Technically works (see: beets-beatport4 plugin)
- ⚠️ May violate Terms of Service
- ❌ No guarantee of stability (Beatport can rotate the ID)
- ❌ Not officially supported

**Lesson**: "Public" client IDs found via reverse engineering are not the same as official developer credentials.

---

### 8. **Platform Matters More Than Expected**

Tidal asks: **"What platform is your app?"**
- Apple (mobile)
- Android (mobile)
- **Web** ← This is the one for desktop apps!

**Why**: Desktop OAuth uses browser-based flows, so it's classified as "Web" even though Mixxx is a desktop C++ application.

**Lesson**: "Platform" refers to OAuth mechanism, not where the app runs.

---

### 9. **Manual Code Entry Is a Valid Fallback**

When standard OAuth fails, manual code entry works:
1. User clicks Login
2. Browser opens, user logs in
3. User copies final URL (with `?code=`)
4. Pastes into app
5. App extracts code, completes token exchange

**Pros**: Works when redirect fails
**Cons**: Poor UX, code expires quickly, users make mistakes

**Lesson**: Build manual entry as insurance, but prioritize standard flows.

---

### 10. **Test with Real Credentials Early**

We spent hours debugging Beatport with:
- Placeholder `client_id`
- Wrong scopes
- Wrong redirect URIs

**Better approach**:
1. Day 1: Register for official developer access
2. Day 2: Test with real credentials
3. Day 3+: Build on proven foundation

**Lesson**: Don't assume placeholders are good enough for testing.

---

## 🏆 Success Pattern: Tidal

What made Tidal work:

1. ✅ **Official Developer Portal**: https://developer.tidal.com
2. ✅ **Free Self-Service Registration**: No approval wait
3. ✅ **Clear Documentation**: OAuth 2.1 + PKCE well-documented
4. ✅ **Standard Desktop Flow**: `localhost:PORT/callback` redirect
5. ✅ **Modern Scopes**: Simple `playback` scope
6. ✅ **Helpful Error Messages**: Error codes led to solutions

**Total time from registration to working login**: ~30 minutes (with debugging)

---

## ❌ Failure Pattern: Beatport

Why Beatport didn't work:

1. ❌ **No Public Developer Portal**: Partner-only access
2. ❌ **Reverse-Engineered Credentials**: Extracted from Swagger UI
3. ❌ **Incompatible OAuth Flow**: Post-message redirect for SPA, not desktop
4. ❌ **Implicit Scopes**: Can't request explicit scopes
5. ❌ **Rapid Code Expiry**: Code consumed during redirect loop
6. ❌ **Cryptic Errors**: `invalid_scope`, `invalid_request` with no context

**Alternative approaches**:
- Contact Beatport for partner credentials (recommended)
- Use QWebEngine + HTTP interception (complex, may violate ToS)
- Accept limitation and document it

---

## 📋 Recommendations for Future Streaming Services

### Before Starting:
- [ ] Check if service has public developer portal
- [ ] Verify desktop/localhost OAuth is supported
- [ ] Register app and get official credentials FIRST
- [ ] Read OAuth docs for redirect URI requirements
- [ ] Check what scopes are available/required

### During Implementation:
- [ ] Test OAuth with real credentials immediately
- [ ] Verify redirect URI is registered correctly
- [ ] Use current scope syntax (not legacy)
- [ ] Implement error handling for all OAuth errors
- [ ] Build manual code entry as fallback

### Service Evaluation Checklist:
1. **Developer Access**: Free portal or partner-only?
2. **OAuth Flow**: Standard localhost redirect or custom?
3. **Documentation Quality**: Clear or vague?
4. **Community**: Active GitHub/forum discussions?
5. **API Stability**: Versioned or frequently breaking?

---

## 🎓 Technical Insights

### OAuth 2.1 vs 2.0:
- OAuth 2.1 = OAuth 2.0 + mandatory PKCE + security best practices
- No functional difference for desktop apps (PKCE should already be used)

### Localhost Redirect Security:
- `http://localhost` is acceptable for OAuth (loopback exception)
- No HTTPS required for localhost
- Port doesn't matter (8889, 8080, 3000, etc.) as long as it's registered

### Token Lifetimes:
- Authorization codes: ~5-10 minutes (use immediately)
- Access tokens: 1-24 hours (varies by service)
- Refresh tokens: Days to years (persist to keyring)

### PKCE Without Client Secret:
- Public clients (desktop/mobile apps) use PKCE instead of client_secret
- Beatport's public client: PKCE optional (we tried both)
- Tidal: PKCE mandatory (OAuth 2.1 requirement)

---

## 💡 Quotes to Remember

> "Just because you can extract a client_id from DevTools doesn't mean you should build a product around it."

> "OAuth redirect URIs are like airport security: you can't fake your way through with a ticket to a different destination."

> "When error messages are cryptic, assume it's a configuration issue, not a code bug."

> "Test with production credentials on Day 1, or spend Day 30 rewriting everything."

---

## 📊 Final Score

| Service | OAuth Difficulty | Time to Working Login | Recommendation |
|---------|------------------|----------------------|----------------|
| **Tidal** | ⭐ Easy | 30 mins | ✅ Use This |
| **Beatport** | ⭐⭐⭐⭐⭐ Impossible* | N/A | ⚠️ Needs Partner Access |

*Without official credentials

---

**Next Services to Consider**:
1. ✅ **Tidal** - DONE!
2. 🔄 **SoundCloud** - Has public API, check OAuth support
3. 🔄 **Spotify** - Excellent developer portal (but Web API, not streaming downloads)
4. ⏸️ **Beatport** - Revisit if partner access granted

---

**Conclusion**: Always start with services that have official developer programs and standard OAuth flows. Workarounds are fragile and time-consuming.
