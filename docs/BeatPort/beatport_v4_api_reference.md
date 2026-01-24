# Beatport API v4 Reference (from uploaded docs snapshot)

> Extracted from the provided HTML snapshot of Beatport’s API Docs v4 page.

**Snapshot source URL:** https://api.beatport.com/v4/docs/v4/auxiliary/artist-types/facets/

## Navigation index (v4)

- Introduction
- API Reference
  - v4
    - auxiliary
      - artist-types
  - {id}
- facets
      - commercial-model-types
  - facets
    - catalog
      - artists
  - {id}
    - images
    - top
      - {num}
    - tracks
  - facets
      - charts
  - {id}
    - images
    - tracks
      - ids
  - facets
  - genre-overview
      - genres
  - {id}
    - sub-genres
    - top
      - {num}
    - tracks
  - facets
      - labels
  - {id}
    - download
    - images
    - releases
    - top
      - {num}
  - facets
      - playlists
  - {id}
    - chart-mapping
    - partners
  - {playlists_pk}
    - tracks
      - facets
      - ids
  - facets
      - releases
  - {id}
    - beatbot
    - images
    - uab
  - {release_pk}
    - dj-edit-tracks
      - facets
    - tracks
      - facets
      - ids
  - facets
  - similar
  - top
    - {num}
      - search
  - facets
      - sub-genres
  - {id}
    - tracks
  - facets
      - tracks
  - {id}
    - audio-files
    - beatbot
    - images
    - publishers
    - stream-sdk
    - uab
  - facets
  - similar
  - store
    - {isrc}
  - top
    - {num}
    - openformat
      - {num}
    - curation
      - playlists
  - {curatedplaylist_pk}
    - tracks
      - {id}
      - facets
  - {id}
  - facets
    - db-health-check
    - health-check
    - my
      - account
  - {id}
  - facets
  - myaccount
      - beatport
  - artists
  - delete
  - facets
  - labels
  - playlists
  - tracks
      - charts
  - {mycharts_pk}
    - tracks
      - {id}
      - facets
  - {id}
    - images
  - facets
      - downloads
  - encode-status
    - facets
  - facets
      - email-preferences
  - {id}
  - facets
      - genres
  - {id}
  - facets
      - playlists
  - {myplaylists_pk}
    - tracks
      - {id}
      - bulk
      - facets
      - ids
  - {id}
  - bulk
  - facets
      - streaming-quality
  - {id}
  - facets
- Logout

## Endpoint details present in snapshot

### `GET /v4/auxiliary/artist-types/facets/`

Return all filter facets for this endpoint, including help text.
If no filters are present, an empty response is returned.

## Notes on completeness

This HTML snapshot contains the full **navigation tree** for the v4 docs, but it only includes **one endpoint’s** rendered detail panel (`GET /v4/auxiliary/artist-types/facets/`) and does not include the full per-endpoint parameter/response documentation for the rest of v4.

If you want a *complete* v4 reference in one Markdown file, you’ll need either:
1) an exported OpenAPI/Swagger spec (JSON/YAML), or
2) additional saved HTML pages for each v4 endpoint section so they can be merged.