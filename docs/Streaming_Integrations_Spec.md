# Streaming Services Integration Specification

## 4. Beatport Link Integration Specification

Beatport's API strategy has historically oscillated between openness and strict closure. The current v4 API is officially restricted to "trusted partners" (e.g., Pioneer DJ, Denon DJ), making official key acquisition for an open-source project like Mixxx functionally impossible without a formal business agreement that would likely conflict with GPL terms regarding DRM. Consequently, integration requires the utilization of the public credentials exposed by Beatport's own infrastructure.

### 4.1 Credential Acquisition: The Swagger UI "Backdoor"
The primary method for accessing the Beatport v4 API without a partner key is to utilize the `client_id` embedded in the public API documentation (Swagger UI). Beatport's interactive documentation allows developers to test endpoints directly from the browser; to facilitate this, the documentation page itself acts as a registered OAuth client.

*   **Source URL:** `https://api.beatport.com/v4/docs/`
*   **Extraction Method:**
    1.  Inspect the page source or network traffic when "Authorize" is clicked.
    2.  Locate the OAuth2 configuration object.
    3.  Extract the `client_id` string. This string is static for the documentation build but may rotate with major updates.
*   **Redirect URI Constraint:** This specific `client_id` is whitelisted only for the documentation's redirect URI. This is typically `https://api.beatport.com/v4/docs/oauth2-redirect.html` or a similar internal path.
*   **Implication:** Mixxx cannot simply use `http://localhost`. It must use the `response_type=token` (Implicit Flow) or `response_type=code` (Auth Code Flow) and intercept the redirect. Since the redirect goes to a secure Beatport domain, Mixxx cannot host a listener there. The only viable path is the **Embedded Web View (Technique C)**, monitoring for the specific Beatport redirect URL string and extracting the token from the location bar before the 404 or page load completes.

### 4.2 OAuth 2.0 Specifications for Beatport
Unlike Tidal and SoundCloud which heavily enforce PKCE, the Beatport documentation client often uses the standard Authorization Code flow or Implicit flow.

*   **Endpoints:**
    *   Authorization: `https://api.beatport.com/v4/oauth/authorize`
    *   Token: `https://api.beatport.com/v4/oauth/token`
*   **Scope Requirements:**
    Beatport uses distinct scopes to segment access. For streaming and library management, the following are required:
    *   `read`: General catalog access.
    *   `write`: Playlist modification.
    *   `stream`: (If available/applicable) Authorization to call stream endpoints. *Note: The Docs client may have restricted streaming scopes compared to a Partner key.*

### 4.3 API Enumeration (v4)
The Beatport v4 API follows a RESTful design using JSON:API standards.

| Functional Area | Endpoint Pattern | Method | Description |
| :--- | :--- | :--- | :--- |
| **Authentication** | `/v4/oauth/authorize` | GET | Initiates user login and consent flow. |
| **Authentication** | `/v4/oauth/token` | POST | Exchanges auth code for Access/Refresh tokens. |
| **User Identity** | `/v4/my/profile` | GET | Retrieves user ID, subscription tier, and preferences. |
| **Catalog Search** | `/v4/catalog/search` | GET | Global search for tracks, artists, labels. Params: `q`, `type`. |
| **Track Metadata** | `/v4/catalog/tracks/{id}` | GET | Detailed metadata: BPM, Key, Waveform URL. |
| **Streaming** | `/v4/catalog/tracks/{id}/stream` | GET | **Critical Endpoint.** Returns the CDN URL (usually Akamai). |
| **User Library** | `/v4/my/library/tracks` | GET | Access to "My Beatport" collection. |
| **Charts** | `/v4/catalog/charts/{id}` | GET | Retrieval of Top 100 lists and genre charts. |

**Streaming Logic and DRM:**
The `/stream` endpoint response typically provides a URL pointing to an AAC audio stream.
*   **Format:** 128kbps AAC (Basic) or 256kbps AAC (Premium).
*   **HLS vs. Progressive:** Beatport increasingly uses HLS (`.m3u8`). The segments are often AES-128 encrypted.
*   **Workaround:** Some legacy implementations or specific parameters (e.g., simulating an older client) might yield standard HTTP progressive download links (`.mp4` or `.aac`) which can be fed directly into Mixxx's FFmpeg decoder. However, relying on this is unstable. If HLS is enforced, Mixxx must implement an HLS parser that handles key acquisition (via the session token) and decryption—a process that must be carefully audited for GPL compliance.

### 4.4 Metadata Handling and "Offline" Modes
Beatport Link theoretically supports an "Offline Locker" (cache).
*   **Implementation:** This involves downloading the encrypted chunks to the local disk (`~/.mixxx/cache/beatport/`).
*   **Database:** Mixxx's SQLite library must map these cached files to the Beatport Track ID.
*   **Re-validation:** On startup, Mixxx must verify the subscription status. If active, the cached keys are valid. If expired, the cache must be inaccessible. This logic is complex to implement in an open-source project without exposing the keys to the user.

---

## 5. Tidal Integration Specification

Tidal offers perhaps the most technically sophisticated API but also the most rigorous security. The integration relies on the `tidalapi` reverse-engineered documentation and the strict OAuth 2.1 implementation.

### 5.1 Client Identity and Quality negotiation
Tidal differentiates `client_id`s by platform (Web, Desktop, Mobile, Auto). This differentiation dictates the maximum audio quality available.

*   **Web Token:** Restricted to High (320kbps AAC).
*   **Mobile/Desktop Token:** capable of Lossless (FLAC 1411kbps) and Hi-Res (MQA/Hi-Res FLAC).

**Credential Harvesting:**
To enable Hi-Res streaming in Mixxx, developers cannot use a generic web token. They must utilize a `client_id` extracted from the official Android or Desktop application.
*   **Extraction Technique:** The file `/Android/data/com.aspiro.tidal/cache/okhttp` on a rooted Android device or emulator often contains the headers used in recent requests. Grepping this file for `X-Tidal-Token` or `Authorization` headers reveals the currently active `client_id`.
*   **Known Keys:** The Strawberry Music Player community and `tidalapi` maintainers often circulate known-good Client IDs. However, Tidal actively blacklists these keys if abuse is detected, necessitating a dynamic configuration option in Mixxx where users can input their own harvested Client ID.

### 5.2 OAuth 2.1 with PKCE Protocol Details
Tidal's implementation is a textbook example of OAuth 2.1.
*   **Authorization Endpoint:** `https://login.tidal.com/authorize`
*   **Token Endpoint:** `https://auth.tidal.com/v1/oauth2/token`

**The "Link" Flow (Device Authorization):**
For a better user experience (and to avoid user-agent blocking), the Device Authorization Flow (RFC 8628) is preferred if the Client ID supports it.
*   **Request:** POST to `https://auth.tidal.com/v1/oauth2/device_authorization` with `client_id`.
*   **Response:** Returns `verification_uri` (e.g., link.tidal.com), `user_code` (e.g., ABCD-EFGH), and `device_code`.
*   **User Action:** User visits the URL on their phone/PC and enters the code.
*   **Polling:** Mixxx polls the token endpoint with the `device_code` until the user completes the login.
*   **Success:** Tidal returns the Access/Refresh tokens.
*   **Note:** This flow bypasses the need for a redirect URI handler entirely, making it ideal for cross-platform desktop apps like Mixxx.

**Scopes:**
*   `r_usr`: Read user profile.
*   `w_usr`: Write (create playlists, add favorites).
*   `playback`: Mandatory for accessing stream URLs.

### 5.3 Token Refresh Rotation
Tidal access tokens have a very short Time-To-Live (TTL), often one hour or less.
*   **Rotation Logic:** When Mixxx uses a refresh_token to get a new access token, Tidal frequently issues a new `refresh_token` in the response.
*   **Critical Requirement:** The `TokenManager` in Mixxx must immediately discard the old refresh token and persist the new one. Failing to do so will permanently lock the user out of the session once the initial refresh token is invalidated (often after one use or a short window).

### 5.4 API Enumeration (v1/v2)
Tidal uses a mix of v1 and v2 endpoints.

| Resource | Endpoint Pattern | Method | Details |
| :--- | :--- | :--- | :--- |
| **Sessions** | `/v1/sessions` | GET | Legacy. Used in older auth flows. Deprecated for OAuth. |
| **User Playlists** | `/v1/users/{userId}/playlists` | GET | Retrieve user collections. |
| **Track Info** | `/v1/tracks/{id}` | GET | Metadata (Artist, Album, Duration). |
| **Stream URL** | `/v1/tracks/{id}/urlpostpaywall` | GET | **Primary Streaming Endpoint.** |
| **Stream Info** | `/v1/tracks/{id}/playbackinfopostpaywall` | GET | Newer endpoint, provides manifest info. |
| **Search** | `/v1/search` | GET | Parameters: `query`, `limit`, `types` (ARTISTS, ALBUMS, TRACKS). |

**Streaming Parameters:**
When calling `urlpostpaywall`, Mixxx must supply the `soundQuality` parameter:
*   `LOW`: 96kbps AAC (HE-AAC).
*   `HIGH`: 320kbps AAC.
*   `LOSSLESS`: 44.1kHz/16bit FLAC.
*   `HI_RES`: MQA or 24bit FLAC (requires specific subscription and Client ID capabilities).

**Error Handling:**
*   `401 Unauthorized`: Token expired.
*   `429 Too Many Requests`: Rate limit hit.
*   `11102`: "Client not found" – The `client_id` is invalid or revoked.

---

## 6. SoundCloud Integration Specification

SoundCloud presents the most significant barrier to entry due to the complete closure of their API registration portal. "Adversarial" techniques are the only path to integration for new applications.

### 6.1 The "Scraping" Identity Strategy
Since Mixxx cannot register for a `client_id`, it must impersonate the official SoundCloud web player. This `client_id` is public (it is sent to every browser) but rotates periodically.

**Automated Scraping Algorithm:**
1.  **Fetch Index:** GET `https://soundcloud.com`.
2.  **Parse Assets:** Use a regular expression to find the script source URL matching `https://a-v2.sndcdn.com/assets/app-[a-f0-9]+.js`.
3.  **Fetch Script:** GET the identified JavaScript asset.
4.  **Extract Key:** Regex match for `client_id:"([a-zA-Z0-9]{32})"`.
5.  **Validation:** Perform a test call to `/resolve` with the key. If 401, repeat scraping.

This logic must be implemented in a Mixxx utility class (e.g., `SoundCloudKeyFetcher`) that runs asynchronously at startup.

### 6.2 Authentication: The Header Migration
SoundCloud has aggressively moved away from query-parameter authentication (`?client_id=xyz`) to Header-based authentication.

**Public Endpoints (Search, Resolve):**
*   **Header:** `Authorization: Client-ID <SCRAPED_KEY>`
*   **Legacy Support:** Some endpoints still accept `client_id` in the query string, but this is deprecated and unreliable.

**Private Endpoints (User Library, Stream High Quality):**
*   **Header:** `Authorization: OAuth <ACCESS_TOKEN>`
*   **Acquisition:** Requires the OAuth 2.1 Authorization Code flow.
*   **Impersonation:** Mixxx must use the scraped `client_id` and the exact redirect URI used by the web player (often `https://soundcloud.com/callback`).
*   **Interceptor:** Mixxx opens the auth page. When the user logs in, SoundCloud redirects to `soundcloud.com/callback?code=...`. Mixxx's `QWebEngineView` or network monitor must catch this request **before** it executes, extract the code, and then manually exchange it for a token using the scraped ID and the web player's redirect URI.

### 6.3 The URN Migration and API Enumeration
SoundCloud is transitioning from integer IDs to URN strings.

| Resource | Endpoint | Description |
| :--- | :--- | :--- |
| **Resolve** | `/resolve` | **Vital.** Converts a permalink to a Track Object. |
| **Track** | `/tracks/{urn}` | e.g., `soundcloud:tracks:123456`. Metadata. |
| **Stream** | `/tracks/{urn}/stream` | Returns the HLS Manifest URL. |
| **Search** | `/search/tracks` | Query catalog. |
| **User Likes** | `/me/likes/tracks` | User's library. |

**Important Note on `urn`:**
While legacy integer IDs (e.g., `123456`) still work on some endpoints, the API responses now return `urn` fields (e.g., `soundcloud:tracks:123456`). Mixxx's database schema must store these as strings, not integers, to ensure forward compatibility.

### 6.4 Streaming and HLS Handling
SoundCloud streaming is almost exclusively HLS (`.m3u8`) now.
*   **Flow:** GET `/tracks/{urn}/stream` (Auth Header required).
*   **Response:** `{ "url": "https://cf-media.sndcdn.com/playlist.m3u8?token=..." }`.
*   **Mixxx must download this M3U8.**
*   **Parsing:** The M3U8 contains URLs to 128kbps MP3 chunks or 256kbps AAC chunks (Go+).
*   **Access:** The chunks themselves require authentication tokens (embedded in the URL query string). These tokens expire quickly, so the M3U8 cannot be cached long-term; it must be refreshed for every playback session.

---

## 7. Cross-Platform Implementation Challenges

Developing this integration within Mixxx's C++ codebase introduces platform-specific hurdles.

### 7.1 Proxy Support and Geo-Blocking
Streaming availability varies by region. The `QNetworkAccessManager` in Mixxx must be configurable to route requests through SOCKS5 or HTTP proxies. This is particularly relevant for the "Scraping" logic, as SoundCloud or Beatport may rate-limit or block IP ranges associated with data centers or VPNs. The `StreamingSoundSource` should inherit proxy settings from the main Mixxx preferences.

### 7.2 Secure Token Storage
Storing OAuth refresh tokens in `mixxx.cfg` (plain text XML/INI) is a security risk and bad practice.
*   **Linux:** Use `libsecret` (Secret Service API) or KWallet.
*   **macOS:** Use Keychain Services API.
*   **Windows:** Use the Data Protection API (DPAPI) to encrypt the token before writing it to disk.

Mixxx should abstract this via a `SecureStorage` class to ensure that user credentials are not exposed if the configuration file is shared.

### 7.3 User Agent Spoofing
To avoid being blocked by the service's Web Application Firewalls (WAF), Mixxx's network requests must spoof a valid User Agent.
*   **Target:** Mimic the User Agent of the official client being emulated.
*   **Beatport:** A standard modern browser UA (e.g., `Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36...`).
*   **Tidal:** If using a mobile Client ID, use `okhttp/4.9.1` or the specific UA string of the Tidal Android app.
*   **SoundCloud:** Mimic the current Chrome/Firefox UA.

---

## 8. Conclusion and Strategic Recommendations

The integration of Beatport, Tidal, and SoundCloud into Mixxx requires a sophisticated, multi-layered approach that goes beyond standard API consumption. It necessitates a hybrid architecture that combines rigorous OAuth 2.1/PKCE implementation with the flexibility to handle undocumented and volatile API behaviors.

**Key Recommendations for Implementation:**
1.  **Modular Plugin Architecture:** Implement the authentication and URL resolution logic as scriptable plugins (JavaScript/ECMAScript via `QJSEngine`) rather than hard-coded C++. This allows the community to rapidly patch the integration when Beatport rotates keys or SoundCloud changes their regex patterns, without requiring a full Mixxx binary recompilation.
2.  **Dynamic Client Configuration:** Do not ship default API keys. Create a UI in Preferences > Streaming that allows users to input their own `client_id` (for Tidal) or automatically runs the scraper (for SoundCloud).
3.  **Strict Buffer Management:** Prioritize the development of the threaded `StreamingSoundSource`. Network instability is the primary enemy of live performance; aggressive caching and look-ahead buffering are non-negotiable requirements.
4.  **Legal Insulation:** Explicitly segregate the logic for DRM decryption (if any) from the core codebase to maintain GPL compliance, potentially relying on user-provided libraries or strictly adhering to unencrypted legacy endpoints.
