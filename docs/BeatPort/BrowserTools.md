Initial Findings from dj.beatport.com API Analysis
Key API Endpoints Discovered:

Streaming/Audio Delivery:

needledrop.beatport.com - HLS streaming infrastructure
Format: {timestamp}\_{hash}/{uuid}.128k.aac.m3u8
Segmented audio: .aac-1.aac, .aac-2.aac, etc. (chunked streaming)
Encryption key: .128k.aac.key

Core API Endpoints:

https://api.beatport.com/v4/events/play/ (POST)
https://api.beatport.com/v4/events/load/ (POST)
https://dj.beatport.com/api/metadata.php?bp (POST)
https://dj.beatport.com/api/xcueget.php (POST) - Cue point data
https://dj.beatport.com/api/xupdate.php (POST) - State updates

Media/Images:

geo-media.beatport.com/image_size/{width}x{height}/{uuid}.jpg

JavaScript Bridge Functions Found:

beatport*login() / beatport_logout()
beatport_loadsong() / beatport_loadsong_reject()
beatport_set_token() - Authentication token setter
beatport_automix*\* - Automix controls
beatport_player_resize() / beatport_master_volume()

What I Need Next:
I need to capture the actual request/response payloads for the metadata and API calls. The network monitoring shows URLs but I need to see:

Request headers (especially auth tokens)
POST request bodies
Response JSON structures

Would you like me to continue exploring? I can:

Click on different tracks to trigger metadata API calls
Navigate to your library/playlists to see those API patterns
Try to trigger a search to see search API structure

Let me know if you want me to keep digging or if there's a specific area you'd like me to focus on!you have access to the requests in the browser tools? Trigger what you will, maybe I think it should be easier.32 stepsComprehensive dj.beatport.com API Documentation
Based on my exploration, here's what I've discovered about the dj.beatport.com streaming API architecture:

🔐 AUTHENTICATION ENDPOINTS
Primary Auth:

POST https://dj.beatport.com/api/custom/bp/v2/auth-v2.php
POST https://dj.beatport.com/api/xlogin.php?_={timestamp}

JavaScript Bridge Function:

beatport_set_token() - Client-side function to set auth token
beatport_login() / beatport_logout() - Login/logout handlers

🎵 TRACK & METADATA ENDPOINTS
Track Streaming URL:

GET https://api.beatport.com/v4/catalog/tracks/{trackId}/stream/

Returns streaming configuration (likely includes HLS manifest URL)

Track Metadata:

POST https://dj.beatport.com/api/metadata.php?bp

Returns full track metadata (BPM, key, artist, label, etc.)

Cue Points:

POST https://dj.beatport.com/api/xcueget.php

Returns beatgrid and cue point data

State Updates:

POST https://dj.beatport.com/api/xupdate.php?_={timestamp}

Periodic updates for UI state synchronization

📡 STREAMING INFRASTRUCTURE
HLS Audio Delivery:

Base Domain: needledrop.beatport.com
Format: {timestamp}\_{hash}/{uuid}.128k.aac.m3u8
Example: needledrop.beatport.com/1769216371_a2ef756cab0dec37ae31ae7c7fe85ea2970bf561/4/3/a/43a20d85-2085-417c-8143-32b4b38d088d.128k.aac.m3u8

HLS Segments:

Manifest: .m3u8 file
Encryption Key: .128k.aac.key
Audio Chunks: .128k.aac-{n}.aac (numbered segments 1-60+)
Query params: ?start=0&end={duration_ms}

Audio Format:

Codec: AAC (128kbps)
Delivery: HLS (HTTP Live Streaming)
Encryption: AES (key file required)

📊 EVENTS & ANALYTICS
Playback Events:

POST https://api.beatport.com/v4/events/play/ - Track play event
POST https://api.beatport.com/v4/events/load/ - Track load event

🗂️ USER DATA ENDPOINTS
Playlists:

POST https://dj.beatport.com/api/uplaylists.php

User playlists management

🖼️ MEDIA ASSETS
Album/Track Artwork:

Domain: geo-media.beatport.com
Format: /image_size/{width}x{height}/{uuid}.jpg
Common sizes: 200x200, 500x500, 590x404

🧩 API STRUCTURE PATTERNS
Two API Domains:

api.beatport.com (v4 REST API)

Modern RESTful endpoints
/v4/catalog/, /v4/events/

dj.beatport.com/api/ (Legacy PHP endpoints)

Metadata, cue points, authentication
PHP script-based (.php extensions)

Request Pattern:

Most POST requests likely require authentication headers
Timestamps used as cache-busting parameters

🎛️ CLIENT FUNCTIONS (JavaScript Bridge)
Functions exposed on window:

- beatport_login()
- beatport_logout()
- beatport_loadsong()
- beatport_loadsong_reject()
- beatport_set_token()
- beatport_set_compact()
- beatport_automix_toggle()
- beatport_automix_on/off()
- beatport_automix_playpause()
- beatport_automix_autobpm()
- beatport_automix_keylock()
- beatport_automix_shortmix()
- beatport_automix_seek()
- beatport_automix_bpm_change()
- beatport_automix_bpm_value()
- beatport_automix_next()
- beatport_player_resize()
- beatport_master_volume()

🔍 WHAT'S MISSING (Need to capture):

Request Headers - Especially authentication tokens (Authorization, X-Auth-Token, etc.)
POST Request Bodies - JSON structures for metadata, login, playlist requests
Response JSON Schemas - Full structure of API responses
OAuth Flow Details - If they use OAuth, we need the full flow
Genre/Trending/Top100 Endpoints - Specific URLs for these features

💡 RECOMMENDATIONS FOR NEXT STEPS:

Capture Auth Flow: Need to see the login process to understand token acquisition
Request/Response Bodies: Use browser DevTools Network tab to copy actual JSON
Headers Analysis: Check what auth headers are required
Test Endpoints: Try calling endpoints directly with proper auth to understand structure

The architecture appears to be a hybrid system using both modern REST API (api.beatport.com/v4) and legacy PHP scripts (dj.beatport.com/api/\*.php). The streaming uses standard HLS with AES encryption.
Would you like me to try to capture more specific details about any of these endpoints? Or would it be helpful if I attempted to open DevTools properly to see actual request/response payloads?
