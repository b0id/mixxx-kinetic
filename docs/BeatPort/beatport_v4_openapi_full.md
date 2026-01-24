# Beatport API
**Version:** v4

Beatport API documentation

## Authentication & Security Schemes
### `basicAuth`
- Type: `http`
- scheme: `basic`

### `cookieAuth`
- Type: `apiKey`
- in: `cookie`
- name: `sessionid`

### `oauth2`
- Type: `oauth2`
- Flow: `authorizationCode`
  - authorizationUrl: `/v4/auth/o/authorize/`
  - tokenUrl: `/v4/auth/o/token/`
  - Scopes:
    - `app:docs`: I am authorized to authenticate with Beatport Developer Docs
    - `app:control`: I am authorized to authenticate with Beatport Control
    - `app:baseware`: I am authorized to authenticate with Beatport Baseware
    - `app:store`: I am authorized to authenticate with the Beatport Store
    - `app:loadtest`: I am authorized to load test the API
    - `app:pipe`: I am authorized to authenticate with the Beatport Encoding Pipeline (bp-ondemand)
    - `app:ingestion`: I am authorized to authenticate with the Beatsource Ingestion
    - `app:locker`: I am authorized to authenticate with the Beatport Locker
    - `app:djapp`: I am authorized to authenticate with Beatport DJ
    - `app:mobile`: I am authorized to authenticated with Beatport Mobile
    - `app:oauth`: I am authorized to authenticate with an OAuth application
    - `app:microsite`: I am authorized to authenticate with Hype Microsite
    - `app:prostore`: I am authorized to authenticate with the Pro Store
    - `app:external`: I am authorized to authenticate as a third-party application
    - `app:externaltrusted`: I am authorized to authenticate as a trusted third-party application
    - `app:beatportal`: I am authorized to authenticate as Beatportal
    - `app:contentstatusupdater`: I am authorized to authenticate with the Content Status Updater
    - `app:integrations`: I am authorized to authenticate as the salesforce integration services
    - `app:greenroom`: I am authorized to authenticate as the Greenroom dashboard services user
    - `app:greenroomapi`: I am authorized to authenticate as the Greenroom dashboard server
    - `staff:admin`: I am an Admin user
    - `staff:content_ops`: I am a Content Ops staff member user
    - `staff:curation`: I am a Curation staff member user
    - `staff:marketing`: I am a Marketing staff member user
    - `staff:label_mgmt`: I am a Label Management staff member user
    - `staff:support`: I am a Customer Support staff member user
    - `staff:finance`: I am a Finance staff member user
    - `user:supplier`: I am a Supplier user
    - `user:label`: I am a Label user
    - `user:artist`: I am an Artist user
    - `user:dj`: I am a DJ user
    - `user:btsrc_beta`: I am a Beta Tester for the Beatsource store
    - `user:beatport_beta`: I am a Beta Tester for the Beatport store
    - `user:bs_sunset`: I am migrated Beatsource user
    - `user:bs_legacy`: I am registered before Beatsource migration started
    - `user:anon`: I am an anonymous user
    - `catalog:read`: View All Catalog Objects
    - `catalog:write`: Edit All Catalog Objects
    - `curation:read`: View All Curation Objects
    - `curation:write`: Edit All Curation Objects
    - `landing_pages:read`: View All Page Objects
    - `landing_pages:write`: Edit All Page Objects
    - `coupons:read`: View All Coupon Objects
    - `coupons:write`: Edit All Coupon Objects
- Flow: `implicit`
  - authorizationUrl: `/v4/auth/o/authorize/`
  - Scopes:
    - `app:docs`: I am authorized to authenticate with Beatport Developer Docs
    - `app:control`: I am authorized to authenticate with Beatport Control
    - `app:baseware`: I am authorized to authenticate with Beatport Baseware
    - `app:store`: I am authorized to authenticate with the Beatport Store
    - `app:loadtest`: I am authorized to load test the API
    - `app:pipe`: I am authorized to authenticate with the Beatport Encoding Pipeline (bp-ondemand)
    - `app:ingestion`: I am authorized to authenticate with the Beatsource Ingestion
    - `app:locker`: I am authorized to authenticate with the Beatport Locker
    - `app:djapp`: I am authorized to authenticate with Beatport DJ
    - `app:mobile`: I am authorized to authenticated with Beatport Mobile
    - `app:oauth`: I am authorized to authenticate with an OAuth application
    - `app:microsite`: I am authorized to authenticate with Hype Microsite
    - `app:prostore`: I am authorized to authenticate with the Pro Store
    - `app:external`: I am authorized to authenticate as a third-party application
    - `app:externaltrusted`: I am authorized to authenticate as a trusted third-party application
    - `app:beatportal`: I am authorized to authenticate as Beatportal
    - `app:contentstatusupdater`: I am authorized to authenticate with the Content Status Updater
    - `app:integrations`: I am authorized to authenticate as the salesforce integration services
    - `app:greenroom`: I am authorized to authenticate as the Greenroom dashboard services user
    - `app:greenroomapi`: I am authorized to authenticate as the Greenroom dashboard server
    - `staff:admin`: I am an Admin user
    - `staff:content_ops`: I am a Content Ops staff member user
    - `staff:curation`: I am a Curation staff member user
    - `staff:marketing`: I am a Marketing staff member user
    - `staff:label_mgmt`: I am a Label Management staff member user
    - `staff:support`: I am a Customer Support staff member user
    - `staff:finance`: I am a Finance staff member user
    - `user:supplier`: I am a Supplier user
    - `user:label`: I am a Label user
    - `user:artist`: I am an Artist user
    - `user:dj`: I am a DJ user
    - `user:btsrc_beta`: I am a Beta Tester for the Beatsource store
    - `user:beatport_beta`: I am a Beta Tester for the Beatport store
    - `user:bs_sunset`: I am migrated Beatsource user
    - `user:bs_legacy`: I am registered before Beatsource migration started
    - `user:anon`: I am an anonymous user
    - `catalog:read`: View All Catalog Objects
    - `catalog:write`: Edit All Catalog Objects
    - `curation:read`: View All Curation Objects
    - `curation:write`: Edit All Curation Objects
    - `landing_pages:read`: View All Page Objects
    - `landing_pages:write`: Edit All Page Objects
    - `coupons:read`: View All Coupon Objects
    - `coupons:write`: Edit All Coupon Objects
- Flow: `password`
  - tokenUrl: `/v4/auth/o/token/`
  - Scopes:
    - `app:docs`: I am authorized to authenticate with Beatport Developer Docs
    - `app:control`: I am authorized to authenticate with Beatport Control
    - `app:baseware`: I am authorized to authenticate with Beatport Baseware
    - `app:store`: I am authorized to authenticate with the Beatport Store
    - `app:loadtest`: I am authorized to load test the API
    - `app:pipe`: I am authorized to authenticate with the Beatport Encoding Pipeline (bp-ondemand)
    - `app:ingestion`: I am authorized to authenticate with the Beatsource Ingestion
    - `app:locker`: I am authorized to authenticate with the Beatport Locker
    - `app:djapp`: I am authorized to authenticate with Beatport DJ
    - `app:mobile`: I am authorized to authenticated with Beatport Mobile
    - `app:oauth`: I am authorized to authenticate with an OAuth application
    - `app:microsite`: I am authorized to authenticate with Hype Microsite
    - `app:prostore`: I am authorized to authenticate with the Pro Store
    - `app:external`: I am authorized to authenticate as a third-party application
    - `app:externaltrusted`: I am authorized to authenticate as a trusted third-party application
    - `app:beatportal`: I am authorized to authenticate as Beatportal
    - `app:contentstatusupdater`: I am authorized to authenticate with the Content Status Updater
    - `app:integrations`: I am authorized to authenticate as the salesforce integration services
    - `app:greenroom`: I am authorized to authenticate as the Greenroom dashboard services user
    - `app:greenroomapi`: I am authorized to authenticate as the Greenroom dashboard server
    - `staff:admin`: I am an Admin user
    - `staff:content_ops`: I am a Content Ops staff member user
    - `staff:curation`: I am a Curation staff member user
    - `staff:marketing`: I am a Marketing staff member user
    - `staff:label_mgmt`: I am a Label Management staff member user
    - `staff:support`: I am a Customer Support staff member user
    - `staff:finance`: I am a Finance staff member user
    - `user:supplier`: I am a Supplier user
    - `user:label`: I am a Label user
    - `user:artist`: I am an Artist user
    - `user:dj`: I am a DJ user
    - `user:btsrc_beta`: I am a Beta Tester for the Beatsource store
    - `user:beatport_beta`: I am a Beta Tester for the Beatport store
    - `user:bs_sunset`: I am migrated Beatsource user
    - `user:bs_legacy`: I am registered before Beatsource migration started
    - `user:anon`: I am an anonymous user
    - `catalog:read`: View All Catalog Objects
    - `catalog:write`: Edit All Catalog Objects
    - `curation:read`: View All Curation Objects
    - `curation:write`: Edit All Curation Objects
    - `landing_pages:read`: View All Page Objects
    - `landing_pages:write`: Edit All Page Objects
    - `coupons:read`: View All Coupon Objects
    - `coupons:write`: Edit All Coupon Objects

## Tags
_No tags block found; endpoints will be grouped by operation tags if available._

## Quick lookup
- **/v4/catalog/tracks/{id}/stream-sdk**
  - `GET /v4/catalog/tracks/{id}/stream-sdk/` — catalog_tracks_stream_sdk_retrieve
- **/v4/catalog/search**
  - `GET /v4/catalog/search/` — catalog_search_retrieve
- **/v4/catalog/tracks/{id}**
  - `GET /v4/catalog/tracks/{id}/` — catalog_tracks_retrieve

## Endpoints (grouped by tag)

### auxiliary

#### `GET /v4/auxiliary/artist-types/` — auxiliary_artist_types_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |

**Responses**
- **200**
- **application/json**: `PaginatedArtistTypeListList`

#### `GET /v4/auxiliary/artist-types/facets/` — auxiliary_artist_types_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `ArtistType`

#### `GET /v4/auxiliary/artist-types/{id}/` — auxiliary_artist_types_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `ArtistTypeDetail`

#### `GET /v4/auxiliary/commercial-model-types/` — auxiliary_commercial_model_types_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |

**Responses**
- **200**
- **application/json**: `PaginatedCommercialModelTypeListList`

#### `GET /v4/auxiliary/commercial-model-types/facets/` — auxiliary_commercial_model_types_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `CommercialModelType`

### catalog

#### `GET /v4/catalog/artists/` — catalog_artists_list

A Mixin for adding the ability in a view top items by resource.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `created` | query | no | `string:date-time` | Filter by exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `enabled` | query | no | `boolean` | Filter by enabled.

* `false` - False
* `true` - True |
| `id` | query | no | `array`[`integer`] | Filter by artist id exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `name` | query | no | `string` | Filter name by case-insensitive text containment. |
| `name_exact` | query | no | `string` | Filter by name exact match. |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `updated` | query | no | `string:date-time` | Filter by exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |

**Responses**
- **200**
- **application/json**: `PaginatedArtistListList`

#### `GET /v4/catalog/artists/facets/` — catalog_artists_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Artist`

#### `GET /v4/catalog/artists/{id}/` — catalog_artists_retrieve

A Mixin for adding the ability in a view top items by resource.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `ArtistDetail`

#### `GET /v4/catalog/artists/{id}/images/` — catalog_artists_images_retrieve

Return/create this artist's images.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Artist`

#### `GET /v4/catalog/artists/{id}/top/{num}/` — catalog_artists_top_retrieve

Return top <N> tracks for a artist by most popular rank, where N limited between 1 and 100.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |
| `num` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Artist`

#### `GET /v4/catalog/artists/{id}/tracks/` — catalog_artists_tracks_retrieve

Return this artist's tracks

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Artist`

#### `GET /v4/catalog/charts/` — catalog_charts_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `add_date` | query | no | `string:date-time` | Filter by date added.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `curated_playlist_genre_id` | query | no | `array`[`integer`] | Filter on genre ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_id` | query | no | `array`[`integer`] | Filter by the exact dj profile id.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_name` | query | no | `array`[`string`] | Filter by case-insensitive DJ name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_slug` | query | no | `array`[`string`] | Filter by case-insensitive DJ slug containment.
Supports `OR` lookup:

`param=value1,value2` |
| `enabled` | query | no | `boolean` | Filter by enabled boolean match.

* `false` - False
* `true` - True |
| `genre_id` | query | no | `array`[`number`] | Filter on genre ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter on case-insensitive genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter by exact chart ID.
Supports `OR` lookup:

`param=value1,value2` |
| `is_approved` | query | no | `boolean` | Filter by approved boolean match.

* `false` - False
* `true` - True |
| `is_curated_playlist` | query | no | `integer` | Filter by is curated playlist boolean match. |
| `is_indexed` | query | no | `boolean` | Filter by indexed boolean match.

* `false` - False
* `true` - True |
| `is_published` | query | no | `boolean` | Filter by published boolean match.

* `false` - False
* `true` - True |
| `name` | query | no | `array`[`string`] | Filter by case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `publish_date` | query | no | `string:date-time` | Filter by date published.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `register_ip_address` | query | no | `integer:int64` | Filter by exact registration IP address match. |
| `sub_genre_id` | query | no | `array`[`number`] | Filter on sub genre ID.
Supports `OR` lookup:

`param=value1,value2` |
| `sub_genre_name` | query | no | `array`[`string`] | Filter on case-insensitive sub_genre name
Supports `OR` lookup:

`param=value1,value2` |
| `track_id` | query | no | `array`[`number`] | Filter on track_id.
Supports `OR` lookup:

`param=value1,value2` |
| `updated` | query | no | `string:date-time` | Filter by date added.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `user_id` | query | no | `array`[`integer`] | Filter by exact user ID.
Supports `OR` lookup:

`param=value1,value2` |
| `username` | query | no | `array`[`string`] | Filter by case-insensitive username containment.
Supports `OR` lookup:

`param=value1,value2` |

**Responses**
- **200**
- **application/json**: `PaginatedChartListList`

#### `GET /v4/catalog/charts/facets/` — catalog_charts_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Chart`

#### `GET /v4/catalog/charts/genre-overview/` — catalog_charts_genre_overview_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `ChartGenreOverview`

#### `GET /v4/catalog/charts/{id}/` — catalog_charts_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `ChartDetail`

#### `GET /v4/catalog/charts/{id}/images/` — catalog_charts_images_retrieve

Return/create this chart's images.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Chart`

#### `GET /v4/catalog/charts/{id}/tracks/` — catalog_charts_tracks_retrieve

Return this chart's tracks, updating if necessary.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Chart`

#### `GET /v4/catalog/charts/{id}/tracks/ids/` — catalog_charts_tracks_ids_retrieve

Return this chart's track IDs.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Chart`

#### `GET /v4/catalog/genres/` — catalog_genres_list

A Mixin for adding the ability in a view top items by resource.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `enabled` | query | no | `boolean` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `name` | query | no | `string` | Filter by exact genre name match. |
| `order_by` | query | no | `array`[`string`] | Ordering

* `id` - Genre ID
* `-id` - Genre ID (descending)
* `status` - Genre Status
* `-status` - Genre Status (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |

**Responses**
- **200**
- **application/json**: `PaginatedGenreListList`

#### `GET /v4/catalog/genres/facets/` — catalog_genres_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Genre`

#### `GET /v4/catalog/genres/{id}/` — catalog_genres_retrieve

A Mixin for adding the ability in a view top items by resource.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `GenreDetail`

#### `GET /v4/catalog/genres/{id}/sub-genres/` — catalog_genres_sub_genres_retrieve

Return this genre's sub-genres

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Genre`

#### `GET /v4/catalog/genres/{id}/top/{num}/` — catalog_genres_top_retrieve

Return top <N> tracks for a genre by most popular rank, where N limited between 1 and 100.

Takes optional argument `type=release` for `releases`.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |
| `num` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Genre`

#### `GET /v4/catalog/genres/{id}/tracks/` — catalog_genres_tracks_retrieve

Return this genre's tracks

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Genre`

#### `GET /v4/catalog/labels/` — catalog_labels_list

A Mixin for adding the ability in a view top items by resource.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `created` | query | no | `string:date-time` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `default_pre_order_weeks` | query | no | `array`[`integer`] | Filter by Default Pre-Order Weeks exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `enabled` | query | no | `boolean` | Filter by enabled boolean match.

* `false` - False
* `true` - True |
| `hype_active` | query | no | `string` | Filter on active enrollment in Hype.

* `true` - True
* `false` - False |
| `hype_eligible` | query | no | `string` | Filter on Hype eligibility.

* `true` - True
* `false` - False |
| `hype_trial_end_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `hype_trial_start_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `id` | query | no | `array`[`integer`] | Filter by ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `is_available_for_hype` | query | no | `boolean` | Filter by Is Available for Hype exact match.

* `false` - False
* `true` - True |
| `is_available_for_pre_order` | query | no | `boolean` | Filter by Is Available for Pre-Order exact match.

* `false` - False
* `true` - True |
| `is_available_for_streaming` | query | no | `boolean` | Filter by Is Available for Streaming exact match.

* `false` - False
* `true` - True |
| `label_manager` | query | no | `string` | Filter by case-insensitive Label Manager name containment. |
| `name` | query | no | `array`[`string`] | Filter by case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `name_exact` | query | no | `string` | Filter by exact label name. |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `supplier_id` | query | no | `array`[`integer`] | Filter by exact supplier ID.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_name` | query | no | `array`[`string`] | Filter by case-insensitive supplier name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `updated` | query | no | `string:date-time` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |

**Responses**
- **200**
- **application/json**: `PaginatedLabelListList`

#### `GET /v4/catalog/labels/facets/` — catalog_labels_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Label`

#### `GET /v4/catalog/labels/{id}/` — catalog_labels_retrieve

A Mixin for adding the ability in a view top items by resource.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `LabelDetail`

#### `GET /v4/catalog/labels/{id}/download/` — catalog_labels_download_retrieve

A Mixin for adding the ability in a view top items by resource.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `LabelCSVExport`

#### `GET /v4/catalog/labels/{id}/images/` — catalog_labels_images_retrieve

Return/create this label's images.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Label`

#### `GET /v4/catalog/labels/{id}/releases/` — catalog_labels_releases_retrieve

Return this label's releases

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Label`

#### `GET /v4/catalog/labels/{id}/top/{num}/` — catalog_labels_top_retrieve

Return top <N> tracks for a label by most popular rank, where N limited between 1 and 100.

Takes optional argument `type=release` for `releases`.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |
| `num` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Label`

#### `GET /v4/catalog/playlists/` — catalog_playlists_list

Return list of Playlists.

For LINK users it checks that playlists are public and published
and will further filter according to subscription plan
For CMS users it will return all  playlists and use the catalog fields
Note that this is virtually identitical to the MyPlaylistsViewSet list method,
except the queryset does not filter by the requesting user id.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `artist_name` | query | no | `array`[`string`] | Filter on the artist name
Supports `OR` lookup:

`param=value1,value2` |
| `created_date` | query | no | `string:date-time` | Filter on date range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `genre_id` | query | no | `array`[`integer`] | Filter on Genre ID
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter on Genre name
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter by exact ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `is_public` | query | no | `boolean` | * `false` - False
* `true` - True |
| `name` | query | no | `array`[`string`] | Filter on Playlist name
Supports `OR` lookup:

`param=value1,value2` |
| `order_by` | query | no | `array`[`string`] | Order by a field. Choices: created_date, name, updated_date.             Use -created_datefor descending order

* `create_date` - Create date
* `-create_date` - Create date (descending)
* `update_date` - Update date
* `-update_date` - Update date (descending)
* `name` - Name
* `-name` - Name (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `playlist_status` | query | no | `boolean` | * `false` - False
* `true` - True |
| `playlist_type` | query | no | `array`[`string`] | Filter on Playlist name
Supports `OR` lookup:

`param=value1,value2` |
| `playlist_type_id` | query | no | `array`[`string`] | Filter on Playlist ID
Supports `OR` lookup:

`param=value1,value2` |
| `status` | query | no | `boolean` | * `false` - False
* `true` - True |
| `updated_date` | query | no | `string:date-time` | Filter on date range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `user_id` | query | no | `array`[`integer`] | Filter by exact user ID.
Supports `OR` lookup:

`param=value1,value2` |
| `username` | query | no | `array`[`string`] | Filter by case-insensitive username containment.
Supports `OR` lookup:

`param=value1,value2` |

**Responses**
- **200**
- **application/json**: `PaginatedPlaylistProxyList`

#### `GET /v4/catalog/playlists/facets/` — catalog_playlists_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Playlist`

#### `GET /v4/catalog/playlists/{id}/` — catalog_playlists_retrieve

Return a Playlist.
CMS users
Loads any playlist regardless of ownership and will use the catalog fields
LINK users
Restrict a private playlist that is not owned by requesting user.

Note that this is virtually identitical to the MyPlaylistsViewSet list method,
except the queryset does not filter by the requesting user id.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**: 200
- **application/json**: `Playlist_metadata_response`

#### `GET /v4/catalog/playlists/{id}/chart-mapping/` — catalog_playlists_chart_mapping_retrieve

Get the chart mapping value if any

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Playlist`

#### `GET /v4/catalog/playlists/{id}/partners/` — catalog_playlists_partners_retrieve

Get the playlist Link! partners if any

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Playlist`

#### `GET /v4/catalog/playlists/{playlists_pk}/tracks/` — catalog_playlists_tracks_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `artist_id` | query | no | `array`[`number`] | Filter on artist ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `artist_name` | query | no | `array`[`string`] | Filter on case-insensitive artist name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `available_worldwide` | query | no | `string` | Filter on available worldwide boolean match.

* `false` - False
* `true` - True |
| `bpm` | query | no | `string` | Filter on exact, less/greater than equal and range. |
| `catalog_number` | query | no | `array`[`string`] | Filter on case-insensitive catalog_number exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `change_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `chord_type_id` | query | no | `array`[`number`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `current_status` | query | no | `array`[`number`] | Filter on current_status ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_edits` | query | no | `string` | Filter on dj_edits boolean match.

* `false` - False
* `true` - True |
| `enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `encode_status` | query | no | `array`[`string`] | Filter on case-insensitive encode status exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `encoded_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_period` | query | no | `array`[`number`] | Filter on case-insensitive exclusive period exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `free_download_end_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `free_download_start_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `genre_enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `genre_id` | query | no | `array`[`number`] | Filter on genre ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter on case-insensitive genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `guid` | query | no | `array`[`string`] | filter on exact guid match
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter on ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `is_available_for_streaming` | query | no | `string` | Filter on streaming available boolean match.

* `false` - False
* `true` - True |
| `is_classic` | query | no | `string` | Filter on is_classic boolean match.

* `false` - False
* `true` - True |
| `is_explicit` | query | no | `string` | Filter on is_explicit boolean match.

* `false` - False
* `true` - True |
| `is_hype` | query | no | `string` | Filter on is_hype boolean match.

* `false` - False
* `true` - True |
| `isrc` | query | no | `array`[`string`] | Filter on exact ISRC match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_id` | query | no | `array`[`number`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_name` | query | no | `array`[`string`] | Filter key. Denote sharp as #, flat as b with minor/major separated by a space.
            Available Keys:
                "A Minor"
                "A Major"
                "Ab Minor"
                "Ab Major"
                "A# Minor"
                "A# Major"
                "B Minor"
                "B Major"
                "Bb Minor"
                "Bb Major"
                "C Minor"
                "C Major"
                "C# Minor"
                "C# Major"
                "D Minor"
                "D Major"
                "Db Minor"
                "Db Major"
                "D# Minor"
                "D# Major"
                "E Minor"
                "E Major"
                "Eb Minor"
                "Eb Major"
                "F Minor"
                "F Major"
                "F# Minor"
                "F# Major"
                "G Minor"
                "G Major"
                "Gb Minor"
                "Gb Major"
                "G# Minor"
                "G# Major"
            
Supports `OR` lookup:

`param=value1,value2` |
| `label_enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `label_id` | query | no | `array`[`number`] | Filter on label ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `label_manager` | query | no | `string` | Filter by case-insensitive Label Manager name containment. |
| `label_name` | query | no | `array`[`string`] | Filter on case-insensitive label name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `label_name_exact` | query | no | `array`[`string`] | Filter on case-insensitive label name exact.
Supports `OR` lookup:

`param=value1,value2` |
| `mix_name` | query | no | `array`[`string`] | Filter on case-insensitive remix name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `new_release_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `order_by` | query | no | `array`[`string`] | Order by a field. Choices: publish_date, genre, label, name.             Use -genre for descending order

* `publish_date` - Publish date
* `-publish_date` - Publish date (descending)
* `release_id` - Release id
* `-release_id` - Release id (descending)
* `release_date` - Release date
* `-release_date` - Release date (descending)
* `genre` - Genre
* `-genre` - Genre (descending)
* `label` - Label
* `-label` - Label (descending)
* `name` - Name
* `-name` - Name (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `playlists_pk` | path | yes | `string` |  |
| `pre_order_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_status` | query | no | `array`[`string`] | Filter on publish_status exact match
Supports `OR` lookup:

`param=value1,value2` |
| `release_id` | query | no | `array`[`number`] | Filter on exact release ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `release_name` | query | no | `array`[`string`] | Filter on case-insensitive release name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `sale_type` | query | no | `array`[`string`] | Filter on case-insensitive sale type exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `sub_genre_id` | query | no | `array`[`number`] | Filter on case-insensitive sub-genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_id` | query | no | `array`[`number`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `track_number` | query | no | `array`[`number`] | Filter on exact track_number match.
Supports `OR` lookup:

`param=value1,value2` |
| `type` | query | no | `array`[`string`] | Filter on track type. Either Release, Album or Mix
Supports `OR` lookup:

`param=value1,value2` |
| `type_id` | query | no | `array`[`number`] | Filter on track release type id
Supports `OR` lookup:

`param=value1,value2` |
| `ugc_remixes` | query | no | `string` | Filter on ugc_remixes boolean match.

* `false` - False
* `true` - True |
| `was_ever_exclusive` | query | no | `string` | Filter on was_ever_exclusive boolean match.

* `false` - False
* `true` - True |

**Responses**
- **200**
- **application/json**: `PaginatedPlaylistTrackList`

#### `GET /v4/catalog/playlists/{playlists_pk}/tracks/facets/` — catalog_playlists_tracks_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `playlists_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `PlaylistTrack`

#### `GET /v4/catalog/playlists/{playlists_pk}/tracks/ids/` — catalog_playlists_tracks_ids_retrieve

Return a list of `PlaylistTrack` IDs for the given playlist.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `playlists_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `PlaylistTrack`

#### `GET /v4/catalog/releases/` — catalog_releases_list

A ViewSet Mixin that adds routes for recommendations given one or
many items as a reference. Recommendations only apply to items whose
item_type_id is present in `publish_beatbot`.`source_item_type_id`.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `artist_id` | query | no | `array`[`integer`] | Filter by exact artist ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `artist_name` | query | no | `array`[`string`] | Filter by case-insensitive artist name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `catalog_number` | query | no | `array`[`string`] | Filter by case-insensitive catalog number exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `created` | query | no | `string:date-time` | Filter by exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `current_status` | query | no | `array`[`number`] | Filter on current_status ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `enabled` | query | no | `boolean` | Filter by enabled boolean match.

* `false` - False
* `true` - True |
| `encoded_date` | query | no | `string:date-time` | Filter by exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive` | query | no | `boolean` | Filter by exclusive boolean match.

* `false` - False
* `true` - True |
| `genre_id` | query | no | `array`[`integer`] | Filter by exact genre ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter by case-insensitive genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter by exact ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `is_available_for_streaming` | query | no | `boolean` | Filter on streaming available boolean match.

* `false` - False
* `true` - True |
| `is_explicit` | query | no | `string` | Filter on is_explicit boolean match.

* `false` - False
* `true` - True |
| `is_hype` | query | no | `integer` | Filter on is_hype boolean match.

* `false` - False
* `true` - True |
| `label_enabled` | query | no | `boolean` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `label_id` | query | no | `array`[`integer`] | Filter by exact label ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `label_name` | query | no | `array`[`string`] | Filter by case-insensitive label name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `label_name_exact` | query | no | `array`[`string`] | Filter by case-insensitive label name exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `name` | query | no | `array`[`string`] | Filter by case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `new_release_date` | query | no | `string:date` | Filter by exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `order_by` | query | no | `array`[`string`] | Order by a field. Choices: publish_date, release_date, label, name, id             and status. Use -genre for descending order

* `publish_date` - Release Date
* `-publish_date` - Release Date (descending)
* `label` - Label
* `-label` - Label (descending)
* `name` - Release Name
* `-name` - Release Name (descending)
* `id` - Release ID
* `-id` - Release ID (descending)
* `status` - Release Status
* `-status` - Release Status (descending)
* `release_date` - Release date
* `-release_date` - Release date (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `pre_order_date` | query | no | `string:date` | Filter by exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_date` | query | no | `string:date` | Filter by exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `release_type_id` | query | no | `array`[`integer`] | Filter by exact type ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `sub_genre_id` | query | no | `array`[`integer`] | Filter by exact sub-genre ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_id` | query | no | `array`[`integer`] | Filter by case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_name` | query | no | `array`[`string`] | Filter by case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `track_id` | query | no | `array`[`integer`] | Filter by exact track ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `track_name` | query | no | `array`[`string`] | Filter by case-insensitive track name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `type` | query | no | `array`[`string`] | Filter on track type. Either Release, Album or Mix
Supports `OR` lookup:

`param=value1,value2` |
| `type_id` | query | no | `array`[`integer`] | Filter on release type id
Supports `OR` lookup:

`param=value1,value2` |
| `upc` | query | no | `array`[`string`] | Filter by exact ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `updated` | query | no | `string:date-time` | Filter by exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |

**Responses**
- **200**
- **application/json**: `PaginatedReleaseListList`

#### `GET /v4/catalog/releases/facets/` — catalog_releases_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Release`

#### `GET /v4/catalog/releases/similar/` — catalog_releases_similar_retrieve

Adds the beatbot list route to the ViewSet
:param request: the Request object
:return: a Response

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Release`

#### `GET /v4/catalog/releases/top/{num}/` — catalog_releases_top_retrieve

Return top <N> releases by global most popular rank, where N limited between 1 and 100.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `num` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Release`

#### `GET /v4/catalog/releases/{id}/` — catalog_releases_retrieve

A ViewSet Mixin that adds routes for recommendations given one or
many items as a reference. Recommendations only apply to items whose
item_type_id is present in `publish_beatbot`.`source_item_type_id`.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `ReleaseDetail`

#### `GET /v4/catalog/releases/{id}/beatbot/` — catalog_releases_beatbot_retrieve

Adds the beatbot detail route to the ViewSet
:param request: the Request object
:param pk: the source item's primary key
:return: a Response

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Release`

#### `GET /v4/catalog/releases/{id}/images/` — catalog_releases_images_retrieve

Return this release's images.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Release`

#### `GET /v4/catalog/releases/{id}/uab/` — catalog_releases_uab_retrieve

Adds the users-also-bought detail route to the View
:param request: the Request object
:param pk: the source item's primary key
:return: a Response

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Release`

#### `GET /v4/catalog/releases/{release_pk}/dj-edit-tracks/` — catalog_releases_dj_edit_tracks_list

Behaviour in this method is copied from tracks.ReleaseTrackViewSet.list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `release_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `PaginatedTrackListList`

#### `GET /v4/catalog/releases/{release_pk}/dj-edit-tracks/facets/` — catalog_releases_dj_edit_tracks_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `release_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/releases/{release_pk}/tracks/` — catalog_releases_tracks_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `release_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `PaginatedTrackListList`

#### `GET /v4/catalog/releases/{release_pk}/tracks/facets/` — catalog_releases_tracks_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `release_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/releases/{release_pk}/tracks/ids/` — catalog_releases_tracks_ids_retrieve

Return a list of `Track` IDs for the given release.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `release_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `ReleaseTrackId`

#### `GET /v4/catalog/search/` — catalog_search_retrieve

Search documentation maintained in frontend/js/routes/docs/search.js.

**Security:**
- `oauth2` scopes: `app:control`
- `oauth2` scopes: `app:baseware`
- `oauth2` scopes: `app:loadtest`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:microsite`
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:external`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/catalog/search/facets/` — catalog_search_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `oauth2` scopes: `app:control`
- `oauth2` scopes: `app:baseware`
- `oauth2` scopes: `app:loadtest`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:microsite`
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:external`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/catalog/sub-genres/` — catalog_sub_genres_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `enabled` | query | no | `boolean` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `name` | query | no | `string` | Filter by exact genre name match. |
| `order_by` | query | no | `array`[`string`] | Ordering

* `id` - SubGenre ID
* `-id` - SubGenre ID (descending)
* `status` - SubGenre Status
* `-status` - SubGenre Status (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |

**Responses**
- **200**
- **application/json**: `PaginatedSubGenreListList`

#### `GET /v4/catalog/sub-genres/facets/` — catalog_sub_genres_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `SubGenre`

#### `GET /v4/catalog/sub-genres/{id}/` — catalog_sub_genres_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `SubGenreDetail`

#### `GET /v4/catalog/sub-genres/{id}/tracks/` — catalog_sub_genres_tracks_retrieve

Return this sub-genre's tracks

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `SubGenre`

#### `GET /v4/catalog/tracks/` — catalog_tracks_list

A ViewSet Mixin that adds routes for recommendations given one or
many items as a reference. Recommendations only apply to items whose
item_type_id is present in `publish_beatbot`.`source_item_type_id`.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `artist_id` | query | no | `array`[`integer`] | Filter on artist ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `artist_name` | query | no | `array`[`string`] | Filter on case-insensitive artist name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `available_worldwide` | query | no | `boolean` | Filter on available worldwide boolean match.

* `false` - False
* `true` - True |
| `bpm` | query | no | `integer` | Filter on exact, less/greater than equal and range. |
| `catalog_number` | query | no | `array`[`string`] | Filter on case-insensitive catalog_number exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `change_date` | query | no | `string:date-time` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `chord_type_id` | query | no | `array`[`integer`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `current_status` | query | no | `array`[`number`] | Filter on current_status ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_edits` | query | no | `string` | Filter on dj_edits boolean match.

* `false` - False
* `true` - True |
| `enabled` | query | no | `boolean` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `encode_status` | query | no | `array`[`string`] | Filter on case-insensitive encode status exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `encoded_date` | query | no | `string:date-time` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_period` | query | no | `array`[`integer`] | Filter on case-insensitive exclusive period exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `free_download_end_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `free_download_start_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `genre_enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `genre_id` | query | no | `array`[`integer`] | Filter on genre ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter on case-insensitive genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `guid` | query | no | `array`[`string`] | filter on exact guid match
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter on ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `is_available_for_streaming` | query | no | `boolean` | Filter on streaming available boolean match.

* `false` - False
* `true` - True |
| `is_classic` | query | no | `boolean` | Filter on is_classic boolean match.

* `false` - False
* `true` - True |
| `is_explicit` | query | no | `boolean` | Filter on is_explicit boolean match.

* `false` - False
* `true` - True |
| `is_hype` | query | no | `boolean` | Filter on is_hype boolean match.

* `false` - False
* `true` - True |
| `isrc` | query | no | `array`[`string`] | Filter on exact ISRC match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_id` | query | no | `array`[`number`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_name` | query | no | `array`[`string`] | Filter key. Denote sharp as #, flat as b with minor/major separated by a space.
            Available Keys:
                "A Minor"
                "A Major"
                "Ab Minor"
                "Ab Major"
                "A# Minor"
                "A# Major"
                "B Minor"
                "B Major"
                "Bb Minor"
                "Bb Major"
                "C Minor"
                "C Major"
                "C# Minor"
                "C# Major"
                "D Minor"
                "D Major"
                "Db Minor"
                "Db Major"
                "D# Minor"
                "D# Major"
                "E Minor"
                "E Major"
                "Eb Minor"
                "Eb Major"
                "F Minor"
                "F Major"
                "F# Minor"
                "F# Major"
                "G Minor"
                "G Major"
                "Gb Minor"
                "Gb Major"
                "G# Minor"
                "G# Major"
            
Supports `OR` lookup:

`param=value1,value2` |
| `label_enabled` | query | no | `boolean` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `label_id` | query | no | `array`[`integer`] | Filter on label ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `label_manager` | query | no | `string` | Filter by case-insensitive Label Manager name containment. |
| `label_name` | query | no | `array`[`string`] | Filter on case-insensitive label name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `label_name_exact` | query | no | `array`[`string`] | Filter on case-insensitive label name exact.
Supports `OR` lookup:

`param=value1,value2` |
| `mix_name` | query | no | `array`[`string`] | Filter on case-insensitive remix name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `new_release_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `order_by` | query | no | `array`[`string`] | Order by a field. Choices: publish_date, genre, label, name.             Use -genre for descending order

* `publish_date` - Publish date
* `-publish_date` - Publish date (descending)
* `release_id` - Release id
* `-release_id` - Release id (descending)
* `release_date` - Release date
* `-release_date` - Release date (descending)
* `genre` - Genre
* `-genre` - Genre (descending)
* `label` - Label
* `-label` - Label (descending)
* `name` - Name
* `-name` - Name (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `pre_order_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_status` | query | no | `array`[`string`] | Filter on publish_status exact match
Supports `OR` lookup:

`param=value1,value2` |
| `release_id` | query | no | `array`[`number`] | Filter on exact release ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `release_name` | query | no | `array`[`string`] | Filter on case-insensitive release name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `sale_type` | query | no | `array`[`string`] | Filter on case-insensitive sale type exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `sub_genre_id` | query | no | `array`[`number`] | Filter on case-insensitive sub-genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_id` | query | no | `array`[`integer`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `track_number` | query | no | `array`[`integer`] | Filter on exact track_number match.
Supports `OR` lookup:

`param=value1,value2` |
| `type` | query | no | `array`[`string`] | Filter on track type. Either Release, Album or Mix
Supports `OR` lookup:

`param=value1,value2` |
| `type_id` | query | no | `array`[`integer`] | Filter on track release type id
Supports `OR` lookup:

`param=value1,value2` |
| `ugc_remixes` | query | no | `string` | Filter on ugc_remixes boolean match.

* `false` - False
* `true` - True |
| `was_ever_exclusive` | query | no | `boolean` | Filter on was_ever_exclusive boolean match.

* `false` - False
* `true` - True |

**Responses**
- **200**
- **application/json**: `PaginatedTrackListList`

#### `GET /v4/catalog/tracks/facets/` — catalog_tracks_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/tracks/similar/` — catalog_tracks_similar_retrieve

Adds the beatbot list route to the ViewSet
:param request: the Request object
:return: a Response

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/tracks/store/{isrc}/` — catalog_tracks_store_retrieve

Return the store url for a track given the correct isrc.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `isrc` | path | yes | `string` | The ISRC of the track |

**Responses**
- **200**: 200
- **application/json**: `OK`

#### `GET /v4/catalog/tracks/top/openformat/{num}/` — catalog_tracks_top_openformat_retrieve

Return top <N> Open Format tracks by global most popular rank,
where N limited between 1 and 100.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `num` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/tracks/top/{num}/` — catalog_tracks_top_retrieve

Return top <N> tracks by global most popular rank, where N limited between 1 and 100.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `num` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/tracks/{id}/` — catalog_tracks_retrieve

Return a territory restricted track.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `TrackDetail`

#### `GET /v4/catalog/tracks/{id}/audio-files/` — catalog_tracks_audio_files_retrieve

Return track audio files.

Adds create [POST] helper route, unless provided in request body,
this assumes the file is on disk and has been successfully encoded.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/tracks/{id}/beatbot/` — catalog_tracks_beatbot_retrieve

Adds the beatbot detail route to the ViewSet
:param request: the Request object
:param pk: the source item's primary key
:return: a Response

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/tracks/{id}/images/` — catalog_tracks_images_retrieve

Return this track's images.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/tracks/{id}/publishers/` — catalog_tracks_publishers_retrieve

Return this Track's Publishers, updating if necessary

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/tracks/{id}/stream-sdk/` — catalog_tracks_stream_sdk_retrieve

A ViewSet Mixin that adds routes for recommendations given one or
many items as a reference. Recommendations only apply to items whose
item_type_id is present in `publish_beatbot`.`source_item_type_id`.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

#### `GET /v4/catalog/tracks/{id}/uab/` — catalog_tracks_uab_retrieve

Adds the users-also-bought detail route to the View
:param request: the Request object
:param pk: the source item's primary key
:return: a Response

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Track`

### curation

#### `GET /v4/curation/playlists/` — curation_playlists_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `artist_name` | query | no | `array`[`string`] | Filter on the artist name
Supports `OR` lookup:

`param=value1,value2` |
| `created_date` | query | no | `string:date-time` | Filter on date range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `genre_id` | query | no | `array`[`integer`] | Filter on Genre ID
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter on Genre name
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter by exact ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `is_public` | query | no | `boolean` | * `false` - False
* `true` - True |
| `name` | query | no | `array`[`string`] | Filter on Playlist name
Supports `OR` lookup:

`param=value1,value2` |
| `order_by` | query | no | `array`[`string`] | Order by a field. Choices: created_date, name, updated_date.             Use -created_datefor descending order

* `create_date` - Create date
* `-create_date` - Create date (descending)
* `update_date` - Update date
* `-update_date` - Update date (descending)
* `name` - Name
* `-name` - Name (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `playlist_status` | query | no | `boolean` | * `false` - False
* `true` - True |
| `playlist_type` | query | no | `array`[`string`] | Filter on Playlist name
Supports `OR` lookup:

`param=value1,value2` |
| `playlist_type_id` | query | no | `array`[`string`] | Filter on Playlist ID
Supports `OR` lookup:

`param=value1,value2` |
| `status` | query | no | `boolean` | * `false` - False
* `true` - True |
| `updated_date` | query | no | `string:date-time` | Filter on date range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `user_id` | query | no | `array`[`integer`] | Filter by exact user ID.
Supports `OR` lookup:

`param=value1,value2` |
| `username` | query | no | `array`[`string`] | Filter by case-insensitive username containment.
Supports `OR` lookup:

`param=value1,value2` |

**Responses**
- **200**
- **application/json**: `PaginatedCuratedPlaylistList`

#### `GET /v4/curation/playlists/facets/` — curation_playlists_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `CuratedPlaylist`

#### `GET /v4/curation/playlists/{curatedplaylist_pk}/tracks/` — curation_playlists_tracks_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `artist_id` | query | no | `array`[`number`] | Filter on artist ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `artist_name` | query | no | `array`[`string`] | Filter on case-insensitive artist name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `available_worldwide` | query | no | `string` | Filter on available worldwide boolean match.

* `false` - False
* `true` - True |
| `bpm` | query | no | `string` | Filter on exact, less/greater than equal and range. |
| `catalog_number` | query | no | `array`[`string`] | Filter on case-insensitive catalog_number exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `change_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `chord_type_id` | query | no | `array`[`number`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `curatedplaylist_pk` | path | yes | `string` |  |
| `current_status` | query | no | `array`[`number`] | Filter on current_status ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_edits` | query | no | `string` | Filter on dj_edits boolean match.

* `false` - False
* `true` - True |
| `enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `encode_status` | query | no | `array`[`string`] | Filter on case-insensitive encode status exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `encoded_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_period` | query | no | `array`[`number`] | Filter on case-insensitive exclusive period exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `free_download_end_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `free_download_start_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `genre_enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `genre_id` | query | no | `array`[`number`] | Filter on genre ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter on case-insensitive genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `guid` | query | no | `array`[`string`] | filter on exact guid match
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter on ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `is_available_for_streaming` | query | no | `string` | Filter on streaming available boolean match.

* `false` - False
* `true` - True |
| `is_classic` | query | no | `string` | Filter on is_classic boolean match.

* `false` - False
* `true` - True |
| `is_explicit` | query | no | `string` | Filter on is_explicit boolean match.

* `false` - False
* `true` - True |
| `is_hype` | query | no | `string` | Filter on is_hype boolean match.

* `false` - False
* `true` - True |
| `isrc` | query | no | `array`[`string`] | Filter on exact ISRC match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_id` | query | no | `array`[`number`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_name` | query | no | `array`[`string`] | Filter key. Denote sharp as #, flat as b with minor/major separated by a space.
            Available Keys:
                "A Minor"
                "A Major"
                "Ab Minor"
                "Ab Major"
                "A# Minor"
                "A# Major"
                "B Minor"
                "B Major"
                "Bb Minor"
                "Bb Major"
                "C Minor"
                "C Major"
                "C# Minor"
                "C# Major"
                "D Minor"
                "D Major"
                "Db Minor"
                "Db Major"
                "D# Minor"
                "D# Major"
                "E Minor"
                "E Major"
                "Eb Minor"
                "Eb Major"
                "F Minor"
                "F Major"
                "F# Minor"
                "F# Major"
                "G Minor"
                "G Major"
                "Gb Minor"
                "Gb Major"
                "G# Minor"
                "G# Major"
            
Supports `OR` lookup:

`param=value1,value2` |
| `label_enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `label_id` | query | no | `array`[`number`] | Filter on label ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `label_manager` | query | no | `string` | Filter by case-insensitive Label Manager name containment. |
| `label_name` | query | no | `array`[`string`] | Filter on case-insensitive label name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `label_name_exact` | query | no | `array`[`string`] | Filter on case-insensitive label name exact.
Supports `OR` lookup:

`param=value1,value2` |
| `mix_name` | query | no | `array`[`string`] | Filter on case-insensitive remix name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `new_release_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `order_by` | query | no | `array`[`string`] | Order by a field. Choices: publish_date, genre, label, name.             Use -genre for descending order

* `publish_date` - Publish date
* `-publish_date` - Publish date (descending)
* `release_id` - Release id
* `-release_id` - Release id (descending)
* `release_date` - Release date
* `-release_date` - Release date (descending)
* `genre` - Genre
* `-genre` - Genre (descending)
* `label` - Label
* `-label` - Label (descending)
* `name` - Name
* `-name` - Name (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `pre_order_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_status` | query | no | `array`[`string`] | Filter on publish_status exact match
Supports `OR` lookup:

`param=value1,value2` |
| `release_id` | query | no | `array`[`number`] | Filter on exact release ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `release_name` | query | no | `array`[`string`] | Filter on case-insensitive release name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `sale_type` | query | no | `array`[`string`] | Filter on case-insensitive sale type exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `sub_genre_id` | query | no | `array`[`number`] | Filter on case-insensitive sub-genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_id` | query | no | `array`[`number`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `track_number` | query | no | `array`[`number`] | Filter on exact track_number match.
Supports `OR` lookup:

`param=value1,value2` |
| `type` | query | no | `array`[`string`] | Filter on track type. Either Release, Album or Mix
Supports `OR` lookup:

`param=value1,value2` |
| `type_id` | query | no | `array`[`number`] | Filter on track release type id
Supports `OR` lookup:

`param=value1,value2` |
| `ugc_remixes` | query | no | `string` | Filter on ugc_remixes boolean match.

* `false` - False
* `true` - True |
| `was_ever_exclusive` | query | no | `string` | Filter on was_ever_exclusive boolean match.

* `false` - False
* `true` - True |

**Responses**
- **200**
- **application/json**: `PaginatedPlaylistTrackList`

#### `GET /v4/curation/playlists/{curatedplaylist_pk}/tracks/facets/` — curation_playlists_tracks_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `curatedplaylist_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `PlaylistTrack`

#### `GET /v4/curation/playlists/{curatedplaylist_pk}/tracks/{id}/` — curation_playlists_tracks_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `curatedplaylist_pk` | path | yes | `string` |  |
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `PlaylistTrack`

#### `GET /v4/curation/playlists/{id}/` — curation_playlists_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `PlaylistProxy`

### db-health-check

#### `GET /v4/db-health-check/` — db_health_check_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

### health-check

#### `GET /v4/health-check/` — health_check_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

### my

#### `GET /v4/my/account/` — my_account_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |

**Responses**
- **200**
- **application/json**: `PaginatedPersonAccountList`

#### `GET /v4/my/account/facets/` — my_account_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `PersonAccountUpdate`

#### `PATCH /v4/my/account/myaccount/` — my_account_myaccount_partial_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Request body**
- **application/json**: `PatchedPersonAccountUpdate`

**Responses**
- **200**
- **application/json**: `PersonAccountUpdate`

#### `PUT /v4/my/account/myaccount/` — my_account_myaccount_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Request body**
- **application/json**: `PersonAccountUpdate`

**Responses**
- **200**
- **application/json**: `PersonAccountUpdate`

#### `PATCH /v4/my/account/{id}/` — my_account_partial_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Request body**
- **application/json**: `PatchedPersonAccountUpdate`

**Responses**
- **200**
- **application/json**: `PersonAccountUpdate`

#### `PUT /v4/my/account/{id}/` — my_account_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Request body**
- **application/json**: `PersonAccountUpdate`

**Responses**
- **200**
- **application/json**: `PersonAccountUpdate`

#### `GET /v4/my/beatport/` — my_beatport_retrieve

**Security:**
- `oauth2` scopes: `app:loadtest`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `POST /v4/my/beatport/` — my_beatport_create

Create subscription entries for the requesting user for the provided
artist or label ids.

**Security:**
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **201**: No response body

#### `GET /v4/my/beatport/artists/` — my_beatport_artists_retrieve

**Security:**
- `oauth2` scopes: `app:loadtest`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `DELETE /v4/my/beatport/delete/` — my_beatport_delete_destroy

Delete subscription entries for the requesting user for the provided
artist or label ids.

**Security:**
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **204**: No response body

#### `GET /v4/my/beatport/facets/` — my_beatport_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `oauth2` scopes: `app:loadtest`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/my/beatport/labels/` — my_beatport_labels_retrieve

**Security:**
- `oauth2` scopes: `app:loadtest`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/my/beatport/playlists/` — my_beatport_playlists_retrieve

**Security:**
- `oauth2` scopes: `app:loadtest`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/my/beatport/tracks/` — my_beatport_tracks_retrieve

**Security:**
- `oauth2` scopes: `app:loadtest`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `POST /v4/my/beatport/tracks/` — my_beatport_tracks_create

**Security:**
- `oauth2` scopes: `app:store`
- `oauth2` scopes: `app:prostore`
- `oauth2` scopes: `app:locker`
- `oauth2` scopes: `app:djapp`
- `oauth2` scopes: `app:mobile`
- `oauth2` scopes: `app:docs`
- `oauth2` scopes: `app:externaltrusted`
- `oauth2` scopes: `app:beatportal`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/my/charts/` — my_charts_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `add_date` | query | no | `string:date-time` | Filter by date added.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `curated_playlist_genre_id` | query | no | `array`[`integer`] | Filter on genre ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_id` | query | no | `array`[`integer`] | Filter by the exact dj profile id.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_name` | query | no | `array`[`string`] | Filter by case-insensitive DJ name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_slug` | query | no | `array`[`string`] | Filter by case-insensitive DJ slug containment.
Supports `OR` lookup:

`param=value1,value2` |
| `enabled` | query | no | `boolean` | Filter by enabled boolean match.

* `false` - False
* `true` - True |
| `genre_id` | query | no | `array`[`number`] | Filter on genre ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter on case-insensitive genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter by exact chart ID.
Supports `OR` lookup:

`param=value1,value2` |
| `is_approved` | query | no | `boolean` | Filter by approved boolean match.

* `false` - False
* `true` - True |
| `is_curated_playlist` | query | no | `integer` | Filter by is curated playlist boolean match. |
| `is_indexed` | query | no | `boolean` | Filter by indexed boolean match.

* `false` - False
* `true` - True |
| `is_published` | query | no | `boolean` | Filter by published boolean match.

* `false` - False
* `true` - True |
| `name` | query | no | `array`[`string`] | Filter by case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `publish_date` | query | no | `string:date-time` | Filter by date published.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `register_ip_address` | query | no | `integer:int64` | Filter by exact registration IP address match. |
| `sub_genre_id` | query | no | `array`[`number`] | Filter on sub genre ID.
Supports `OR` lookup:

`param=value1,value2` |
| `sub_genre_name` | query | no | `array`[`string`] | Filter on case-insensitive sub_genre name
Supports `OR` lookup:

`param=value1,value2` |
| `track_id` | query | no | `array`[`number`] | Filter on track_id.
Supports `OR` lookup:

`param=value1,value2` |
| `updated` | query | no | `string:date-time` | Filter by date added.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `user_id` | query | no | `array`[`integer`] | Filter by exact user ID.
Supports `OR` lookup:

`param=value1,value2` |
| `username` | query | no | `array`[`string`] | Filter by case-insensitive username containment.
Supports `OR` lookup:

`param=value1,value2` |

**Responses**
- **200**
- **application/json**: `PaginatedChartListList`

#### `POST /v4/my/charts/` — my_charts_create

**Security:**
- `cookieAuth`
- `basicAuth`

**Request body**
- **application/json**: `MyChartCreateUpdate`

**Responses**
- **201**
- **application/json**: `MyChartCreateUpdate`

#### `GET /v4/my/charts/facets/` — my_charts_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Chart`

#### `DELETE /v4/my/charts/{id}/` — my_charts_destroy

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **204**: No response body

#### `GET /v4/my/charts/{id}/` — my_charts_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `ChartDetail`

#### `PATCH /v4/my/charts/{id}/` — my_charts_partial_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Request body**
- **application/json**: `PatchedMyChartCreateUpdate`

**Responses**
- **200**
- **application/json**: `MyChartCreateUpdate`

#### `PUT /v4/my/charts/{id}/` — my_charts_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Request body**
- **application/json**: `MyChartCreateUpdate`

**Responses**
- **200**
- **application/json**: `MyChartCreateUpdate`

#### `GET /v4/my/charts/{id}/images/` — my_charts_images_retrieve

Return/create this chart's images.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `Chart`

#### `POST /v4/my/charts/{id}/images/` — my_charts_images_create

Return/create this chart's images.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Request body**
- **application/json**: `Chart`

**Responses**
- **200**
- **application/json**: `Chart`

#### `GET /v4/my/charts/{mycharts_pk}/tracks/` — my_charts_tracks_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `mycharts_pk` | path | yes | `string` |  |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |

**Responses**
- **200**
- **application/json**: `PaginatedChartTrackList`

#### `POST /v4/my/charts/{mycharts_pk}/tracks/` — my_charts_tracks_create

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `mycharts_pk` | path | yes | `string` |  |

**Request body**
- **application/json**: `MyChartTrackCreateUpdate`

**Responses**
- **201**
- **application/json**: `MyChartTrackCreateUpdate`

#### `GET /v4/my/charts/{mycharts_pk}/tracks/facets/` — my_charts_tracks_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `mycharts_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `ChartTrack`

#### `DELETE /v4/my/charts/{mycharts_pk}/tracks/{id}/` — my_charts_tracks_destroy

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |
| `mycharts_pk` | path | yes | `string` |  |

**Responses**
- **204**: No response body

#### `PATCH /v4/my/charts/{mycharts_pk}/tracks/{id}/` — my_charts_tracks_partial_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |
| `mycharts_pk` | path | yes | `string` |  |

**Request body**
- **application/json**: `PatchedMyChartTrackCreateUpdate`

**Responses**
- **200**
- **application/json**: `MyChartTrackCreateUpdate`

#### `PUT /v4/my/charts/{mycharts_pk}/tracks/{id}/` — my_charts_tracks_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |
| `mycharts_pk` | path | yes | `string` |  |

**Request body**
- **application/json**: `MyChartTrackCreateUpdate`

**Responses**
- **200**
- **application/json**: `MyChartTrackCreateUpdate`

#### `GET /v4/my/downloads/` — my_downloads_list

**Security:**
- `oauth2` scopes: `app:docs`, `user:dj`
- `oauth2` scopes: `app:locker`, `user:dj`
- `oauth2` scopes: `app:djapp`, `user:dj`
- `oauth2` scopes: `app:mobile`, `user:dj`
- `oauth2` scopes: `app:control`, `user:dj`
- `oauth2` scopes: `app:store`, `user:dj`
- `oauth2` scopes: `app:prostore`, `user:dj`
- `oauth2` scopes: `app:externaltrusted`, `user:dj`
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `artist_id` | query | no | `array`[`integer`] | Filter on artist ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `artist_name` | query | no | `array`[`string`] | Filter on case-insensitive artist name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `available_worldwide` | query | no | `boolean` | Filter on available worldwide boolean match.

* `false` - False
* `true` - True |
| `bpm` | query | no | `integer` | Filter on exact, less/greater than equal and range. |
| `catalog_number` | query | no | `array`[`string`] | Filter on case-insensitive catalog_number exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `change_date` | query | no | `string:date-time` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `chord_type_id` | query | no | `array`[`integer`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `current_status` | query | no | `array`[`number`] | Filter on current_status ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_edits` | query | no | `string` | Filter on dj_edits boolean match.

* `false` - False
* `true` - True |
| `enabled` | query | no | `boolean` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `encode_status` | query | no | `array`[`string`] | Filter on case-insensitive encode status exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `encoded_date` | query | no | `string:date-time` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_period` | query | no | `array`[`integer`] | Filter on case-insensitive exclusive period exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `free_download_end_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `free_download_start_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `genre_enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `genre_id` | query | no | `array`[`integer`] | Filter on genre ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter on case-insensitive genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `guid` | query | no | `array`[`string`] | filter on exact guid match
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter on ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `is_available_for_streaming` | query | no | `boolean` | Filter on streaming available boolean match.

* `false` - False
* `true` - True |
| `is_classic` | query | no | `boolean` | Filter on is_classic boolean match.

* `false` - False
* `true` - True |
| `is_explicit` | query | no | `boolean` | Filter on is_explicit boolean match.

* `false` - False
* `true` - True |
| `is_hype` | query | no | `boolean` | Filter on is_hype boolean match.

* `false` - False
* `true` - True |
| `isrc` | query | no | `array`[`string`] | Filter on exact ISRC match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_id` | query | no | `array`[`number`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_name` | query | no | `array`[`string`] | Filter key. Denote sharp as #, flat as b with minor/major separated by a space.
            Available Keys:
                "A Minor"
                "A Major"
                "Ab Minor"
                "Ab Major"
                "A# Minor"
                "A# Major"
                "B Minor"
                "B Major"
                "Bb Minor"
                "Bb Major"
                "C Minor"
                "C Major"
                "C# Minor"
                "C# Major"
                "D Minor"
                "D Major"
                "Db Minor"
                "Db Major"
                "D# Minor"
                "D# Major"
                "E Minor"
                "E Major"
                "Eb Minor"
                "Eb Major"
                "F Minor"
                "F Major"
                "F# Minor"
                "F# Major"
                "G Minor"
                "G Major"
                "Gb Minor"
                "Gb Major"
                "G# Minor"
                "G# Major"
            
Supports `OR` lookup:

`param=value1,value2` |
| `label_enabled` | query | no | `boolean` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `label_id` | query | no | `array`[`integer`] | Filter on label ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `label_manager` | query | no | `string` | Filter by case-insensitive Label Manager name containment. |
| `label_name` | query | no | `array`[`string`] | Filter on case-insensitive label name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `label_name_exact` | query | no | `array`[`string`] | Filter on case-insensitive label name exact.
Supports `OR` lookup:

`param=value1,value2` |
| `mix_name` | query | no | `array`[`string`] | Filter on case-insensitive remix name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `new_release_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `order_by` | query | no | `array`[`string`] | Order by a field. Choices: publish_date, genre, label, name.             Use -genre for descending order

* `publish_date` - Publish date
* `-publish_date` - Publish date (descending)
* `release_id` - Release id
* `-release_id` - Release id (descending)
* `release_date` - Release date
* `-release_date` - Release date (descending)
* `genre` - Genre
* `-genre` - Genre (descending)
* `label` - Label
* `-label` - Label (descending)
* `name` - Name
* `-name` - Name (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `pre_order_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_date` | query | no | `string:date` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_status` | query | no | `array`[`string`] | Filter on publish_status exact match
Supports `OR` lookup:

`param=value1,value2` |
| `purchase_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `received_status` | query | no | `string` | Filter on download received status boolean.

* `false` - False
* `true` - True |
| `release_id` | query | no | `array`[`number`] | Filter on exact release ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `release_name` | query | no | `array`[`string`] | Filter on case-insensitive release name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `sale_type` | query | no | `array`[`string`] | Filter on case-insensitive sale type exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `sub_genre_id` | query | no | `array`[`number`] | Filter on case-insensitive sub-genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_id` | query | no | `array`[`integer`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `track_number` | query | no | `array`[`integer`] | Filter on exact track_number match.
Supports `OR` lookup:

`param=value1,value2` |
| `type` | query | no | `array`[`string`] | Filter on track type. Either Release, Album or Mix
Supports `OR` lookup:

`param=value1,value2` |
| `type_id` | query | no | `array`[`integer`] | Filter on track release type id
Supports `OR` lookup:

`param=value1,value2` |
| `ugc_remixes` | query | no | `string` | Filter on ugc_remixes boolean match.

* `false` - False
* `true` - True |
| `was_ever_exclusive` | query | no | `boolean` | Filter on was_ever_exclusive boolean match.

* `false` - False
* `true` - True |

**Responses**
- **200**
- **application/json**: `PaginatedTrackDownloadListList`

#### `GET /v4/my/downloads/encode-status/` — my_downloads_encode_status_retrieve

**Security:**
- `oauth2` scopes: `app:docs`, `user:dj`
- `oauth2` scopes: `app:control`, `user:dj`
- `oauth2` scopes: `app:store`, `user:dj`
- `oauth2` scopes: `app:prostore`, `user:dj`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/my/downloads/encode-status/facets/` — my_downloads_encode_status_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `oauth2` scopes: `app:docs`, `user:dj`
- `oauth2` scopes: `app:control`, `user:dj`
- `oauth2` scopes: `app:store`, `user:dj`
- `oauth2` scopes: `app:prostore`, `user:dj`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/my/downloads/facets/` — my_downloads_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `oauth2` scopes: `app:docs`, `user:dj`
- `oauth2` scopes: `app:locker`, `user:dj`
- `oauth2` scopes: `app:djapp`, `user:dj`
- `oauth2` scopes: `app:mobile`, `user:dj`
- `oauth2` scopes: `app:control`, `user:dj`
- `oauth2` scopes: `app:store`, `user:dj`
- `oauth2` scopes: `app:prostore`, `user:dj`
- `oauth2` scopes: `app:externaltrusted`, `user:dj`
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `TrackDownloadList`

#### `GET /v4/my/email-preferences/` — my_email_preferences_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/my/email-preferences/facets/` — my_email_preferences_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `PATCH /v4/my/email-preferences/{id}/` — my_email_preferences_partial_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**: No response body

#### `GET /v4/my/genres/` — my_genres_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |

**Responses**
- **200**
- **application/json**: `PaginatedGenreListList`

#### `POST /v4/my/genres/` — my_genres_create

**Security:**
- `cookieAuth`
- `basicAuth`

**Request body**
- **application/json**: `GenreSubscribe`

**Responses**
- **201**
- **application/json**: `GenreSubscribe`

#### `GET /v4/my/genres/facets/` — my_genres_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `GenreDetail`

#### `DELETE /v4/my/genres/{id}/` — my_genres_destroy

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **204**: No response body

#### `GET /v4/my/playlists/` — my_playlists_list

Return list of Playlists belonging to authenticated user.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |

**Responses**
- **200**
- **application/json**: `PaginatedPlaylistProxyList`

#### `POST /v4/my/playlists/` — my_playlists_create

Create a Playlist for the request User.

**Security:**
- `cookieAuth`
- `basicAuth`

**Request body**
- **POST_PATCH**: `Request_body`

**Responses**
- **200**: 200
- **application/json**: `Playlist_metadata_response`

#### `PATCH /v4/my/playlists/bulk/` — my_playlists_bulk_partial_update

Update playlist positions.

**Security:**
- `cookieAuth`
- `basicAuth`

**Request body**
- **application/json**: `PatchedPlaylistBulkUpdate`

**Responses**
- **200**
- **application/json**: `PlaylistBulkUpdate`

#### `GET /v4/my/playlists/facets/` — my_playlists_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**
- **application/json**: `Playlist`

#### `DELETE /v4/my/playlists/{id}/` — my_playlists_destroy

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **204**: No response body

#### `GET /v4/my/playlists/{id}/` — my_playlists_retrieve

Return a Playlist.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**: 200
- **application/json**: `Playlist_metadata_response`

#### `PATCH /v4/my/playlists/{id}/` — my_playlists_partial_update

Update a Playlist for the request User.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Request body**
- **POST_PATCH**: `PatchedRequest_body`

**Responses**
- **200**: 200
- **application/json**: `Playlist_metadata_response`

#### `GET /v4/my/playlists/{myplaylists_pk}/tracks/` — my_playlists_tracks_list

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `artist_id` | query | no | `array`[`number`] | Filter on artist ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `artist_name` | query | no | `array`[`string`] | Filter on case-insensitive artist name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `available_worldwide` | query | no | `string` | Filter on available worldwide boolean match.

* `false` - False
* `true` - True |
| `bpm` | query | no | `string` | Filter on exact, less/greater than equal and range. |
| `catalog_number` | query | no | `array`[`string`] | Filter on case-insensitive catalog_number exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `change_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `chord_type_id` | query | no | `array`[`number`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `current_status` | query | no | `array`[`number`] | Filter on current_status ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `dj_edits` | query | no | `string` | Filter on dj_edits boolean match.

* `false` - False
* `true` - True |
| `enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `encode_status` | query | no | `array`[`string`] | Filter on case-insensitive encode status exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `encoded_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `exclusive_period` | query | no | `array`[`number`] | Filter on case-insensitive exclusive period exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `free_download_end_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `free_download_start_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `genre_enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `genre_id` | query | no | `array`[`number`] | Filter on genre ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `genre_name` | query | no | `array`[`string`] | Filter on case-insensitive genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `guid` | query | no | `array`[`string`] | filter on exact guid match
Supports `OR` lookup:

`param=value1,value2` |
| `id` | query | no | `array`[`integer`] | Filter on ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `is_available_for_streaming` | query | no | `string` | Filter on streaming available boolean match.

* `false` - False
* `true` - True |
| `is_classic` | query | no | `string` | Filter on is_classic boolean match.

* `false` - False
* `true` - True |
| `is_explicit` | query | no | `string` | Filter on is_explicit boolean match.

* `false` - False
* `true` - True |
| `is_hype` | query | no | `string` | Filter on is_hype boolean match.

* `false` - False
* `true` - True |
| `isrc` | query | no | `array`[`string`] | Filter on exact ISRC match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_id` | query | no | `array`[`number`] | Filter on exact key ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `key_name` | query | no | `array`[`string`] | Filter key. Denote sharp as #, flat as b with minor/major separated by a space.
            Available Keys:
                "A Minor"
                "A Major"
                "Ab Minor"
                "Ab Major"
                "A# Minor"
                "A# Major"
                "B Minor"
                "B Major"
                "Bb Minor"
                "Bb Major"
                "C Minor"
                "C Major"
                "C# Minor"
                "C# Major"
                "D Minor"
                "D Major"
                "Db Minor"
                "Db Major"
                "D# Minor"
                "D# Major"
                "E Minor"
                "E Major"
                "Eb Minor"
                "Eb Major"
                "F Minor"
                "F Major"
                "F# Minor"
                "F# Major"
                "G Minor"
                "G Major"
                "Gb Minor"
                "Gb Major"
                "G# Minor"
                "G# Major"
            
Supports `OR` lookup:

`param=value1,value2` |
| `label_enabled` | query | no | `string` | Filter on enabled boolean match.

* `false` - False
* `true` - True |
| `label_id` | query | no | `array`[`number`] | Filter on label ID exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `label_manager` | query | no | `string` | Filter by case-insensitive Label Manager name containment. |
| `label_name` | query | no | `array`[`string`] | Filter on case-insensitive label name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `label_name_exact` | query | no | `array`[`string`] | Filter on case-insensitive label name exact.
Supports `OR` lookup:

`param=value1,value2` |
| `mix_name` | query | no | `array`[`string`] | Filter on case-insensitive remix name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `myplaylists_pk` | path | yes | `string` |  |
| `name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `new_release_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `order_by` | query | no | `array`[`string`] | Order by a field. Choices: publish_date, genre, label, name.             Use -genre for descending order

* `publish_date` - Publish date
* `-publish_date` - Publish date (descending)
* `release_id` - Release id
* `-release_id` - Release id (descending)
* `release_date` - Release date
* `-release_date` - Release date (descending)
* `genre` - Genre
* `-genre` - Genre (descending)
* `label` - Label
* `-label` - Label (descending)
* `name` - Name
* `-name` - Name (descending) |
| `page` | query | no | `integer` | A page number within the paginated result set. |
| `per_page` | query | no | `integer` | Number of results to return per page. |
| `pre_order_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_date` | query | no | `string` | Filter on exact, less/greater than equal and range.
Supports slice syntax:

`date=1970-01-01` (exact)
`date=:1971-01-01` (less than equal)
`date=1970-01-01:` (greater than equal)
`date=1970-01-01:1971-01-01` (range) |
| `publish_status` | query | no | `array`[`string`] | Filter on publish_status exact match
Supports `OR` lookup:

`param=value1,value2` |
| `release_id` | query | no | `array`[`number`] | Filter on exact release ID match.
Supports `OR` lookup:

`param=value1,value2` |
| `release_name` | query | no | `array`[`string`] | Filter on case-insensitive release name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `sale_type` | query | no | `array`[`string`] | Filter on case-insensitive sale type exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `sub_genre_id` | query | no | `array`[`number`] | Filter on case-insensitive sub-genre exact match.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_id` | query | no | `array`[`number`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `supplier_name` | query | no | `array`[`string`] | Filter on case-insensitive name containment.
Supports `OR` lookup:

`param=value1,value2` |
| `track_number` | query | no | `array`[`number`] | Filter on exact track_number match.
Supports `OR` lookup:

`param=value1,value2` |
| `type` | query | no | `array`[`string`] | Filter on track type. Either Release, Album or Mix
Supports `OR` lookup:

`param=value1,value2` |
| `type_id` | query | no | `array`[`number`] | Filter on track release type id
Supports `OR` lookup:

`param=value1,value2` |
| `ugc_remixes` | query | no | `string` | Filter on ugc_remixes boolean match.

* `false` - False
* `true` - True |
| `was_ever_exclusive` | query | no | `string` | Filter on was_ever_exclusive boolean match.

* `false` - False
* `true` - True |

**Responses**
- **200**
- **application/json**: `PaginatedPlaylistTrackList`

#### `POST /v4/my/playlists/{myplaylists_pk}/tracks/` — my_playlists_tracks_create

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `myplaylists_pk` | path | yes | `string` |  |

**Request body**
- **application/json**: `PlaylistTrackCreate`

**Responses**
- **201**
- **application/json**: `PlaylistTrackCreate`

#### `DELETE /v4/my/playlists/{myplaylists_pk}/tracks/bulk/` — my_playlists_tracks_bulk_destroy

Request Body:<br>
Item IDs to delete. The body should include:<br>
<strong>item_ids</strong> (required): List of playlist tracks to delete from the playlist.
(example: {"item_ids": [<id>, <id>, ...]}
  <br>Example: {"item_ids": [1, 2, 3]}

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `myplaylists_pk` | path | yes | `string` |  |

**Responses**
- **200**: 200
- **application/json**: `Playlist_metadata_response`

#### `PATCH /v4/my/playlists/{myplaylists_pk}/tracks/bulk/` — my_playlists_tracks_bulk_partial_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `myplaylists_pk` | path | yes | `string` |  |

**Request body**
- **PATCH**: `PatchedTracks_to_update`

**Responses**
- **200**: 200
- **application/json**: `Playlist_metadata_response`

#### `POST /v4/my/playlists/{myplaylists_pk}/tracks/bulk/` — my_playlists_tracks_bulk_create

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `myplaylists_pk` | path | yes | `string` |  |

**Request body**
- **POST**: `Add_list_of_tracks_to_playlist`

**Responses**
- **200**: 200
- **application/json**: `Post_response`

#### `GET /v4/my/playlists/{myplaylists_pk}/tracks/facets/` — my_playlists_tracks_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `myplaylists_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `PlaylistTrack`

#### `GET /v4/my/playlists/{myplaylists_pk}/tracks/ids/` — my_playlists_tracks_ids_retrieve

Return a list of `PlaylistTrack` IDs for the given playlist.

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `myplaylists_pk` | path | yes | `string` |  |

**Responses**
- **200**
- **application/json**: `PlaylistTrackId`

#### `DELETE /v4/my/playlists/{myplaylists_pk}/tracks/{id}/` — my_playlists_tracks_destroy

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |
| `myplaylists_pk` | path | yes | `string` |  |

**Responses**
- **204**: No response body

#### `PATCH /v4/my/playlists/{myplaylists_pk}/tracks/{id}/` — my_playlists_tracks_partial_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |
| `myplaylists_pk` | path | yes | `string` |  |

**Request body**
- **application/json**: `PatchedPlaylistTrackUpdate`

**Responses**
- **200**
- **application/json**: `PlaylistTrackUpdate`

#### `GET /v4/my/streaming-quality/` — my_streaming_quality_retrieve

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `GET /v4/my/streaming-quality/facets/` — my_streaming_quality_facets_retrieve

Return all filter facets for this endpoint, including help text.

If no filters are present, an empty response is returned.

**Security:**
- `cookieAuth`
- `basicAuth`

**Responses**
- **200**: No response body

#### `PATCH /v4/my/streaming-quality/{id}/` — my_streaming_quality_partial_update

**Security:**
- `cookieAuth`
- `basicAuth`

**Parameters**

| Name | In | Required | Type | Description |
|---|---|---:|---|---|
| `id` | path | yes | `string` |  |

**Responses**
- **200**: No response body

## Schemas

### `Add_list_of_tracks_to_playlist`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `track_ids` | yes | `array`[`integer`] | List of track IDs to add to the playlist(example: {"track_ids": [<id>, <id>, ...]} |

### `Artist`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `url` | yes | `string:uri` |  |
| `image` | yes | `string` |  |
| `dj_association` | yes | `string` |  |
| `add_by` | no | `integer` |  |
| `add_date` | yes | `string:date-time` |  |
| `bio` | no | `string` |  |
| `change_by` | no | `integer` |  |
| `change_date` | yes | `string:date-time` |  |
| `created` | yes | `string:date-time` |  |
| `enabled` | no | `boolean` |  |
| `latest_active_publish_date` | no | `string:date` |  |
| `name` | yes | `string` |  |
| `updated` | yes | `string:date-time` |  |
| `website` | no | `string` |  |

### `ArtistDetail`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `bio` | yes | `string` |  |
| `dj_association` | yes | `string` |  |
| `enabled` | yes | `boolean` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `website` | yes | `string` |  |

### `ArtistList`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `created` | yes | `string:date-time` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `updated` | yes | `string:date-time` |  |
| `website` | yes | `string` |  |
| `url` | yes | `string:uri` |  |
| `dj_association` | yes | `string` |  |

### `ArtistNested`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |

### `ArtistType`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `url` | yes | `string:uri` |  |
| `updated_date` | yes | `string:date-time` |  |
| `updated_person_id` | no | `integer` |  |
| `created_date` | yes | `string:date-time` |  |
| `created_person_id` | no | `integer` |  |
| `name` | yes | `string` |  |

### `ArtistTypeDetail`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `created_date` | yes | `string:date-time` |  |
| `created_person_id` | yes | `integer` |  |
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `updated_date` | yes | `string:date-time` |  |
| `updated_person_id` | yes | `integer` |  |

### `ArtistTypeList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `AudioFormatNested`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |

### `Chart`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `url` | yes | `string:uri` |  |
| `image` | yes | `string` |  |
| `person` | yes | `string` |  |
| `price` | yes | `object` |  |
| `track_count` | yes | `integer` |  |
| `curated_playlist_genre` | yes | `GenreNested` |  |
| `add_by` | no | `integer` |  |
| `add_date` | yes | `string:date-time` |  |
| `change_by` | no | `integer` |  |
| `change_date` | yes | `string:date-time` |  |
| `description` | no | `string` |  |
| `enabled` | yes | `boolean` |  |
| `item_id` | no | `integer` |  |
| `item_type_id` | no | `integer` |  |
| `is_approved` | no | `boolean` |  |
| `is_indexed` | no | `boolean` |  |
| `is_published` | no | `boolean` |  |
| `name` | yes | `string` |  |
| `publish_date` | no | `string:date-time` |  |
| `updated` | yes | `string:date-time` |  |

### `ChartDetail`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `add_by` | yes | `integer` |  |
| `add_date` | yes | `string:date-time` |  |
| `change_date` | yes | `string:date-time` |  |
| `description` | yes | `string` |  |
| `enabled` | yes | `boolean` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `is_approved` | yes | `boolean` |  |
| `is_indexed` | yes | `boolean` |  |
| `is_published` | yes | `boolean` |  |
| `name` | yes | `string` |  |
| `person` | yes | `string` |  |
| `price` | yes | `object` |  |
| `publish_date` | yes | `string:date-time` |  |
| `slug` | yes | `string` |  |
| `track_count` | yes | `integer` |  |
| `artist` | yes | `ArtistNested` |  |
| `genres` | yes | `string` |  |

### `ChartGenreOverview`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `genre` | yes | `string` |  |

### `ChartList`
A Mixin for dynicmally including fields based on a user's scopes.

If this Mixin is used an attr `include_by_scope` should be included
on the serializer that subclasses this mixin. This attr should be
a dict mapping field -> list of allowed scopes.

For example, the following definition would allow the field named
`dynamic_field` to be visible by admin user in baseware or a
marketing user in control.

```
class SerializerClass(ModelSerializer):
    ...

    include_by_scope = {
        'dynamic_field' = [
            ['app:baseware', 'staff:admin'],
            ['app:control', 'staff:marketing'],
        ]
    }
```
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `add_date` | yes | `string:date-time` |  |
| `change_date` | yes | `string:date-time` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `name` | yes | `string` |  |
| `person` | yes | `string` |  |
| `price` | yes | `object` |  |
| `publish_date` | yes | `string:date-time` |  |
| `slug` | yes | `string` |  |
| `track_count` | yes | `integer` |  |
| `url` | yes | `string:uri` |  |
| `artist` | yes | `ArtistNested` |  |
| `genres` | yes | `string` |  |

### `ChartTrack`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `position` | yes | `integer` |  |
| `track` | yes | `TrackList` |  |

### `ChordTypeList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `CommercialModelType`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `updated_date` | yes | `string:date-time` |  |
| `updated_person_id` | no | `integer` |  |
| `created_date` | yes | `string:date-time` |  |
| `created_person_id` | no | `integer` |  |
| `name` | yes | `string` |  |

### `CommercialModelTypeList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |

### `CuratedPlaylist`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `genre` | no | `GenreNested` |  |
| `updated_date` | yes | `string:date-time` |  |
| `created_date` | yes | `string:date-time` |  |

### `CurrentStatusList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `EncodeStatusEnum`
* `STANDARD` - STANDARD
* `PRIORITY` - PRIORITY
* `CONVERTED` - CONVERTED
* `ERROR` - ERROR
* `TRANSCODING` - TRANSCODING
- Type: `string`

**Enum values:** `STANDARD`, `PRIORITY`, `CONVERTED`, `ERROR`, `TRANSCODING`

### `ExclusivePeriodNested`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `days` | no | `integer` |  |
| `description` | yes | `string` |  |
| `id` | yes | `integer` |  |
| `url` | yes | `string:uri` |  |

### `Genre`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `is_included_in_hype` | yes | `boolean` |  |
| `name` | yes | `string` |  |
| `url` | yes | `string:uri` |  |
| `enabled` | yes | `boolean` |  |
| `latest_active_publish_date` | no | `string:date` |  |
| `updated` | yes | `string:date-time` |  |
| `created` | yes | `string:date-time` |  |
| `slug` | yes | `string` |  |
| `sub_genres` | no | `array`[`SubGenreNested`] |  |

### `GenreCategoryList`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |

### `GenreDetail`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `enabled` | yes | `boolean` |  |
| `id` | yes | `integer` |  |
| `is_included_in_hype` | yes | `boolean` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `sub_genres` | no | `array`[`SubGenreNested`] |  |

### `GenreList`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `category` | no | `GenreCategoryList` |  |
| `name` | yes | `string` |  |
| `enabled` | yes | `boolean` |  |
| `slug` | yes | `string` |  |
| `sub_genres` | no | `array`[`SubGenreNested`] |  |
| `url` | yes | `string:uri` |  |
| `launch_date` | yes | `string:date` |  |

### `GenreNested`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `GenreSubscribe`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |

### `ImageList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `guid` | yes | `string` |  |
| `id` | yes | `integer` |  |
| `media_url` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `Item`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `item_id` | yes | `integer` |  |
| `position` | yes | `integer` |  |

### `KeyList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `camelot_number` | yes | `integer` |  |
| `camelot_letter` | yes | `string` |  |
| `chord_type` | yes | `ChordTypeList` |  |
| `id` | yes | `integer` |  |
| `is_sharp` | yes | `boolean` |  |
| `is_flat` | yes | `boolean` |  |
| `letter` | yes | `string` |  |
| `name` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `Label`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `url` | yes | `string:uri` |  |
| `image` | yes | `string` |  |
| `label_manager` | yes | `allOf`(`LabelManagerNested`) |  |
| `supplier` | yes | `SupplierNested` |  |
| `temp_account` | yes | `allOf`(`TempAccount`) |  |
| `hype_eligible` | yes | `string` |  |
| `add_by` | no | `integer` |  |
| `bio` | no | `string` |  |
| `change_by` | no | `integer` |  |
| `created` | yes | `string:date-time` |  |
| `default_exclusivity_weeks` | no | `integer` |  |
| `default_pre_order_weeks` | no | `integer` |  |
| `enabled` | no | `boolean` |  |
| `hype_trial_start_date` | no | `string:date` |  |
| `hype_trial_end_date` | no | `string:date` |  |
| `is_available_for_hype` | no | `boolean` |  |
| `is_available_for_bundle_subscription` | no | `boolean` |  |
| `is_available_for_pre_order` | no | `boolean` |  |
| `is_available_for_streaming` | no | `boolean` |  |
| `is_included_in_rightsflow` | no | `boolean` |  |
| `latest_active_publish_date` | no | `string:date` |  |
| `name` | yes | `string` |  |
| `updated` | yes | `string:date-time` |  |

### `LabelCSVExport`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `aggregator` | yes | `string` |  |
| `label_name` | yes | `string` |  |
| `release_name` | yes | `string` |  |
| `release_upc` | yes | `string` |  |
| `release_catalog_number` | yes | `string` |  |
| `release_publish_date` | yes | `string:date` |  |
| `track_name` | yes | `string` |  |
| `track_mix_name` | yes | `string` |  |
| `artists` | yes | `string` |  |
| `remixers` | yes | `string` |  |
| `track_isrc` | yes | `string` |  |
| `track_publish_date` | yes | `string:date` |  |
| `track_publish_status` | yes | `string` |  |
| `track_number` | yes | `integer` |  |
| `genre_name` | yes | `string` |  |
| `sub_genre_name` | yes | `string` |  |
| `songwriter_names` | yes | `string` |  |
| `publisher_names` | yes | `string` |  |

### `LabelDetail`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `bio` | yes | `string` |  |
| `created` | yes | `string:date-time` |  |
| `default_pre_order_weeks` | yes | `integer` |  |
| `enabled` | yes | `boolean` |  |
| `has_exclusive_contract` | yes | `string` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `is_available_for_hype` | yes | `boolean` |  |
| `is_available_for_bundle_subscription` | yes | `boolean` |  |
| `is_available_for_pre_order` | yes | `boolean` |  |
| `is_available_for_streaming` | yes | `boolean` |  |
| `is_included_in_rightsflow` | yes | `boolean` |  |
| `latest_active_publish_date` | yes | `string:date` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `updated` | yes | `string:date-time` |  |
| `hype_eligible` | yes | `string` |  |
| `hype_active` | yes | `boolean` | Logic here should mirror hype_active filter method
if we can find a non expired subscription for that label
that means that label is hype active |
| `hype_trial_start_date` | yes | `string:date` |  |
| `hype_trial_end_date` | yes | `string:date` |  |

### `LabelList`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `created` | yes | `string:date-time` |  |
| `default_pre_order_weeks` | yes | `integer` |  |
| `enabled` | yes | `boolean` |  |
| `has_exclusive_contract` | no | `boolean` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `is_available_for_hype` | yes | `boolean` |  |
| `is_available_for_pre_order` | yes | `boolean` |  |
| `is_available_for_streaming` | yes | `boolean` |  |
| `latest_active_publish_date` | yes | `string:date` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `updated` | yes | `string:date-time` |  |
| `url` | yes | `string:uri` |  |
| `hype_eligible` | yes | `string` |  |
| `hype_active` | yes | `boolean` | Logic here should mirror hype_active filter method
if we can find a non expired subscription for that label
that means that label is hype active |
| `hype_trial_start_date` | yes | `string:date` |  |
| `hype_trial_end_date` | yes | `string:date` |  |

### `LabelManagerNested`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `email` | yes | `string` |  |
| `enabled` | yes | `boolean` |  |
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `phone_primary` | yes | `string` |  |

### `LabelNested`
A Mixin for dynicmally including fields based on a user's scopes.

If this Mixin is used an attr `include_by_scope` should be included
on the serializer that subclasses this mixin. This attr should be
a dict mapping field -> list of allowed scopes.

For example, the following definition would allow the field named
`dynamic_field` to be visible by admin user in baseware or a
marketing user in control.

```
class SerializerClass(ModelSerializer):
    ...

    include_by_scope = {
        'dynamic_field' = [
            ['app:baseware', 'staff:admin'],
            ['app:control', 'staff:marketing'],
        ]
    }
```
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `image` | yes | `string` |  |
| `slug` | yes | `string` |  |

### `MyChartCreateUpdate`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `description` | no | `string` |  |
| `enabled` | no | `boolean` |  |
| `is_published` | no | `boolean` |  |
| `name` | yes | `string` |  |

### `MyChartTrackCreateUpdate`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `position` | no | `integer` |  |
| `track` | no | `integer` |  |

### `OK`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `store_url` | yes | `string` | Track store url. |

### `PaginatedArtistListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`ArtistList`] |  |

### `PaginatedArtistTypeListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`ArtistTypeList`] |  |

### `PaginatedChartListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`ChartList`] |  |

### `PaginatedChartTrackList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`ChartTrack`] |  |

### `PaginatedCommercialModelTypeListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`CommercialModelTypeList`] |  |

### `PaginatedCuratedPlaylistList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`CuratedPlaylist`] |  |

### `PaginatedGenreListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`GenreList`] |  |

### `PaginatedLabelListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`LabelList`] |  |

### `PaginatedPersonAccountList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`PersonAccount`] |  |

### `PaginatedPlaylistProxyList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`PlaylistProxy`] |  |

### `PaginatedPlaylistTrackList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`PlaylistTrack`] |  |

### `PaginatedReleaseListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`ReleaseList`] |  |

### `PaginatedSubGenreListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`SubGenreList`] |  |

### `PaginatedTrackDownloadListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`TrackDownloadList`] |  |

### `PaginatedTrackListList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `count` | yes | `integer` |  |
| `next` | no | `string:uri` |  |
| `previous` | no | `string:uri` |  |
| `results` | yes | `array`[`TrackList`] |  |

### `PatchedMyChartCreateUpdate`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `description` | no | `string` |  |
| `enabled` | no | `boolean` |  |
| `is_published` | no | `boolean` |  |
| `name` | no | `string` |  |

### `PatchedMyChartTrackCreateUpdate`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `position` | no | `integer` |  |
| `track` | no | `integer` |  |

### `PatchedPersonAccountUpdate`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `email` | no | `string:email` |  |
| `preferences` | no | `PersonPreferenceUpdate` |  |
| `onboarding_completed` | no | `boolean` |  |

### `PatchedPlaylistBulkUpdate`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `items` | no | `array`[`PlaylistReorder`] |  |

### `PatchedPlaylistTrackUpdate`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `position` | no | `integer` |  |

### `PatchedRequest_body`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `name` | no | `string` | Playlist name |

### `PatchedTracks_to_update`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `items` | no | `array`[`Item`] | List of objects which includes the "item_id" and "position".(example: {"items": [{"item_id": <id>, "position": <pos>}, ...]}) |

### `PersonAccount`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `string` |  |
| `add_by` | no | `integer` |  |
| `add_date` | yes | `string:date` |  |
| `bypass_fraud_service` | yes | `boolean` |  |
| `change_by` | no | `integer` |  |
| `change_date` | yes | `string:date` |  |
| `decline_transactions` | yes | `boolean` |  |
| `dj_profile` | yes | `PersonDjProfileNested` |  |
| `email` | yes | `string:email` |  |
| `email_confirmed` | yes | `boolean` |  |
| `enabled` | yes | `boolean` |  |
| `exclude_purchases_from_charts` | yes | `boolean` |  |
| `failed_login_count` | yes | `integer` |  |
| `first_name` | no | `string` |  |
| `forgot_password_status` | yes | `boolean` |  |
| `last_login` | no | `string:date-time` |  |
| `last_name` | yes | `string` |  |
| `name` | yes | `string` |  |
| `person_id` | yes | `integer` |  |
| `phone_number` | yes | `string` |  |
| `phone_primary` | yes | `string` |  |
| `preferences` | yes | `PersonPreference` |  |
| `register_country_id` | no | `integer` |  |
| `register_ip_address` | yes | `string` |  |
| `registration_date` | yes | `string:date-time` |  |
| `source_type` | yes | `SourceTypeList` |  |
| `total_orders` | yes | `string` |  |
| `username` | yes | `string` |  |
| `onboarding_completed` | no | `boolean` |  |

### `PersonAccountUpdate`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `email` | yes | `string:email` |  |
| `preferences` | yes | `PersonPreferenceUpdate` |  |
| `onboarding_completed` | no | `boolean` |  |

### `PersonDjProfileNested`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `associated_artist` | yes | `string` |  |
| `id` | yes | `integer` |  |
| `bio` | no | `string` |  |
| `enabled` | yes | `boolean` |  |
| `is_indexed` | yes | `boolean` |  |
| `image` | yes | `ImageList` |  |
| `facebook_comments_enabled` | no | `boolean` |  |
| `name` | no | `string` |  |
| `genres` | yes | `string` |  |
| `person` | yes | `string:uri` |  |
| `slug` | no | `string` |  |
| `soundcloud_id` | no | `string` |  |
| `soundcloud_mode` | yes | `boolean` |  |

### `PersonPreference`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `audio_format_id` | no | `integer` |  |
| `default_cart_name` | no | `string` |  |
| `language` | no | `string` |  |
| `person` | yes | `integer` |  |
| `in_app_message_opt_out` | no | `schema` |  |
| `streaming_audio_format_id` | no | `integer` |  |
| `filename_convention` | no | `string` |  |
| `mobile_push_notification_is_enabled` | no | `boolean` |  |
| `mobile_push_notification_settings` | no | `schema` |  |

### `PersonPreferenceUpdate`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `audio_format_id` | no | `integer` |  |
| `default_cart_name` | no | `string` |  |
| `language` | no | `string` |  |
| `in_app_message_opt_out` | no | `schema` |  |
| `streaming_audio_format_id` | no | `integer` |  |
| `filename_convention` | no | `string` |  |
| `mobile_push_notification_is_enabled` | no | `boolean` |  |
| `mobile_push_notification_settings` | no | `schema` |  |

### `Playlist`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `created_date` | yes | `string:date-time` |  |
| `updated_date` | yes | `string:date-time` |  |
| `type` | yes | `integer` |  |
| `is_public` | no | `boolean` |  |

### `Playlist_metadata_response`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` | Playlist ID. |
| `name` | yes | `string` | Name of the playlist. |
| `created_date` | yes | `string` | Date playlist was created. |
| `updated_date` | yes | `string` | Date playlist was last updated. |
| `track_count` | yes | `integer` | The number of track in the playlist. |
| `type` | yes | `Whether_the_playlist_is_a_user_or_curated_playlist.` |  |
| `length_ms` | yes | `integer` | The length of the playlist in milliseconds. |
| `bpm_range` | yes | `array`[`integer`] | A [min, max] range of the playlist. Items are integers. |
| `genres` | yes | `array`[`string`] | A list of genre names that are in this playlist. |
| `keys` | yes | `array`[`string`] | A list of keys that are in this playlist. |
| `release_images` | yes | `array`[`string`] | A list of urls to the first four release images in the playlist. If the track does not have a release image, null is returned. If the playlist has less than four tracks in it, this array will not be full. |
| `is_public` | yes | `boolean` | A boolean value that is true if playlist is public or false if private |
| `is_owner` | yes | `boolean` | A boolean value that represents whether the requesting user owns the playlist |

### `PlaylistBulkUpdate`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `items` | yes | `array`[`PlaylistReorder`] |  |

### `PlaylistProxy`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `created_date` | yes | `string:date-time` |  |
| `updated_date` | yes | `string:date-time` |  |
| `track_count` | yes | `string` |  |
| `type` | yes | `PlaylistType` |  |
| `length_ms` | yes | `string` |  |
| `bpm_range` | yes | `string` |  |
| `genres` | yes | `string` |  |
| `keys` | yes | `string` |  |
| `release_images` | yes | `string` |  |
| `is_public` | no | `boolean` |  |
| `is_owner` | yes | `string` |  |
| `followers` | yes | `string` |  |

### `PlaylistReorder`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `item_id` | yes | `integer` |  |
| `position` | no | `integer` |  |

### `PlaylistTrack`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `position` | yes | `integer` |  |
| `track` | yes | `TrackList` |  |
| `tombstoned` | no | `boolean` |  |

### `PlaylistTrackCreate`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `track_id` | yes | `integer` |  |

### `PlaylistTrackId`
Representational serializer for creates/updates.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `position` | yes | `integer` |  |
| `track_id` | yes | `integer` |  |
| `track_available` | yes | `boolean` |  |

### `PlaylistTrackUpdate`
Overides the create/update methods for models with Audit Columns.

This is to be used by the Create/Update Serializers of models w/
audit columns: created/updated_date, created/updated_person_id, etc.

Example:
  ```
  class BankCreateUpdateSerializer(AuditColumnsSerializerMixin,
                                   serializers.ModelSerializer):
  ```

Note that the Mixin comes before the parent class `ModelSerializer`.
Also, if the class defines a custom `create/update()` method you
must call `super().create/update()`.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `position` | yes | `integer` |  |

### `PlaylistType`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |

### `Post_response`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `items` | yes | `object` | Info on newly added tracks. The new item_id will be here. Includes (id, position, track_id, track_available) |
| `playlist` | no | `object` | Updated metadata about the playlist. Includes (id, name, created_date, updated_date, track_count, type, length_ms, bpm_range, genres, keys, release_images, is_public, is_owner, followers) |

### `PublishStatusEnum`
* `hidden` - hidden
* `published` - published
* `labelRemoved` - labelRemoved
* `takedown` - takedown
- Type: `string`

**Enum values:** `hidden`, `published`, `labelRemoved`, `takedown`

### `Release`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `url` | yes | `string:uri` |  |
| `image` | yes | `string` |  |
| `artists` | yes | `array`[`TrackArtist`] |  |
| `exclusive` | yes | `boolean` |  |
| `label` | yes | `LabelNested` |  |
| `price` | yes | `object` |  |
| `pre_order` | yes | `boolean` |  |
| `remixers` | yes | `array`[`TrackArtist`] |  |
| `tracks` | yes | `array`[`string:uri`] |  |
| `track_count` | yes | `integer` |  |
| `type` | yes | `ReleaseTypesDetail` |  |
| `is_explicit` | no | `boolean` |  |
| `is_ugc_remix` | yes | `boolean` |  |
| `is_dj_edit` | yes | `boolean` |  |
| `updated_date` | yes | `string:date-time` |  |
| `updated_person_id` | no | `integer` |  |
| `created_date` | yes | `string:date-time` |  |
| `created_person_id` | no | `integer` |  |
| `add_by` | no | `integer` |  |
| `catalog_number` | no | `string` |  |
| `change_by` | no | `integer` |  |
| `change_date` | yes | `string:date-time` |  |
| `created` | yes | `string:date-time` |  |
| `ddex_release_id` | no | `integer` |  |
| `desc` | no | `string` |  |
| `enabled` | no | `boolean` |  |
| `encoded_date` | no | `string:date-time` |  |
| `exclusive_date` | no | `string:date` |  |
| `grid` | no | `string` |  |
| `is_available_for_streaming` | yes | `boolean` |  |
| `name` | yes | `string` |  |
| `new_release_date` | yes | `string:date` |  |
| `override_price` | no | `string:decimal` |  |
| `pre_order_date` | no | `string:date` |  |
| `price_override_firm` | no | `boolean` |  |
| `publish_date` | no | `string:date` |  |
| `streaming_date` | no | `string:date` |  |
| `upc` | no | `string` |  |
| `updated` | yes | `string:date-time` |  |
| `was_ever_exclusive` | no | `boolean` |  |

### `ReleaseDetail`
A Mixin for dynicmally including fields based on a user's scopes.

If this Mixin is used an attr `include_by_scope` should be included
on the serializer that subclasses this mixin. This attr should be
a dict mapping field -> list of allowed scopes.

For example, the following definition would allow the field named
`dynamic_field` to be visible by admin user in baseware or a
marketing user in control.

```
class SerializerClass(ModelSerializer):
    ...

    include_by_scope = {
        'dynamic_field' = [
            ['app:baseware', 'staff:admin'],
            ['app:control', 'staff:marketing'],
        ]
    }
```
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `artists` | yes | `array`[`TrackArtist`] |  |
| `bpm_range` | yes | `object` |  |
| `catalog_number` | yes | `string` |  |
| `desc` | yes | `string` |  |
| `enabled` | yes | `boolean` |  |
| `encoded_date` | yes | `string:date-time` |  |
| `exclusive` | yes | `boolean` |  |
| `grid` | yes | `string` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `is_available_for_streaming` | yes | `boolean` |  |
| `is_explicit` | no | `boolean` |  |
| `is_ugc_remix` | yes | `boolean` |  |
| `is_dj_edit` | yes | `boolean` |  |
| `label` | yes | `LabelNested` |  |
| `name` | yes | `string` |  |
| `new_release_date` | yes | `string:date` |  |
| `override_price` | yes | `string:decimal` |  |
| `pre_order` | yes | `boolean` |  |
| `pre_order_date` | yes | `string:date` |  |
| `price` | yes | `object` |  |
| `price_override_firm` | yes | `boolean` |  |
| `publish_date` | yes | `string:date` |  |
| `remixers` | yes | `array`[`TrackArtist`] |  |
| `slug` | yes | `string` |  |
| `tracks` | yes | `array`[`string:uri`] |  |
| `track_count` | yes | `integer` |  |
| `type` | yes | `ReleaseTypesDetail` |  |
| `upc` | yes | `string` |  |
| `updated` | yes | `string:date-time` |  |
| `is_hype` | yes | `string` |  |

### `ReleaseList`
A Mixin for dynicmally including fields based on a user's scopes.

If this Mixin is used an attr `include_by_scope` should be included
on the serializer that subclasses this mixin. This attr should be
a dict mapping field -> list of allowed scopes.

For example, the following definition would allow the field named
`dynamic_field` to be visible by admin user in baseware or a
marketing user in control.

```
class SerializerClass(ModelSerializer):
    ...

    include_by_scope = {
        'dynamic_field' = [
            ['app:baseware', 'staff:admin'],
            ['app:control', 'staff:marketing'],
        ]
    }
```
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `artists` | yes | `array`[`TrackArtist`] |  |
| `bpm_range` | yes | `object` |  |
| `catalog_number` | yes | `string` |  |
| `desc` | yes | `string` |  |
| `enabled` | yes | `boolean` |  |
| `encoded_date` | yes | `string:date-time` |  |
| `exclusive` | yes | `boolean` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `is_explicit` | no | `boolean` |  |
| `is_ugc_remix` | yes | `boolean` |  |
| `is_dj_edit` | yes | `boolean` |  |
| `label` | yes | `LabelNested` |  |
| `name` | yes | `string` |  |
| `new_release_date` | yes | `string:date` |  |
| `override_price` | yes | `string:decimal` |  |
| `pre_order` | yes | `boolean` |  |
| `pre_order_date` | yes | `string:date` |  |
| `price` | yes | `object` |  |
| `price_override_firm` | yes | `boolean` |  |
| `publish_date` | yes | `string:date` |  |
| `remixers` | yes | `array`[`TrackArtist`] |  |
| `slug` | yes | `string` |  |
| `track_count` | yes | `integer` |  |
| `upc` | yes | `string` |  |
| `updated` | yes | `string:date-time` |  |
| `url` | yes | `string:uri` |  |
| `is_hype` | yes | `string` |  |

### `ReleaseTrackId`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `pre_order` | yes | `boolean` |  |
| `is_available_for_streaming` | yes | `boolean` |  |
| `is_explicit` | no | `boolean` |  |

### `ReleaseTypesDetail`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |

### `Request_body`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `name` | yes | `string` | Playlist name |

### `SaleTypeList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `SourceTypeList`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `SubGenre`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `url` | yes | `string:uri` |  |
| `updated_date` | yes | `string:date-time` |  |
| `updated_person_id` | no | `integer` |  |
| `created_date` | yes | `string:date-time` |  |
| `created_person_id` | no | `integer` |  |
| `name` | yes | `string` |  |
| `enabled` | yes | `boolean` |  |
| `genre` | yes | `string:uri` |  |

### `SubGenreDetail`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `enabled` | yes | `boolean` |  |
| `genre` | yes | `string:uri` |  |
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `SubGenreList`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `url` | yes | `string:uri` |  |
| `enabled` | yes | `boolean` |  |

### `SubGenreNested`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `url` | yes | `string:uri` |  |
| `enabled` | yes | `boolean` |  |

### `SupplierNested`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `TempAccount`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `user` | yes | `integer` |  |
| `item_id` | yes | `integer` |  |
| `item_type_id` | yes | `integer` |  |
| `username` | yes | `string` |  |
| `temp_password` | no | `string` |  |
| `account_activated` | yes | `boolean` |  |

### `Track`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `url` | yes | `string:uri` |  |
| `image` | yes | `string` |  |
| `artists` | yes | `array`[`TrackArtist`] |  |
| `catalog_number` | yes | `string` |  |
| `current_status` | yes | `CurrentStatusList` |  |
| `exclusive` | yes | `boolean` |  |
| `exclusive_period` | yes | `ExclusivePeriodNested` |  |
| `free_downloads` | yes | `array`[`TrackFreeDownload`] |  |
| `genre` | yes | `GenreNested` |  |
| `is_explicit` | no | `boolean` |  |
| `is_ugc_remix` | yes | `boolean` |  |
| `is_dj_edit` | yes | `boolean` |  |
| `label` | yes | `LabelNested` |  |
| `key` | yes | `KeyList` |  |
| `publishers` | yes | `string` |  |
| `pre_order` | yes | `boolean` |  |
| `price` | yes | `object` |  |
| `release` | yes | `string` |  |
| `remixers` | yes | `array`[`TrackArtist`] |  |
| `sale_type` | yes | `SaleTypeList` |  |
| `sub_genre` | yes | `SubGenreNested` |  |
| `bsrc_remixer` | no | `array`[`TrackArtist`] |  |
| `updated_date` | yes | `string:date-time` |  |
| `updated_person_id` | no | `integer` |  |
| `created_date` | yes | `string:date-time` |  |
| `created_person_id` | no | `integer` |  |
| `is_hype` | no | `boolean` |  |
| `available_worldwide` | yes | `boolean` |  |
| `bpm` | no | `integer` |  |
| `change_date` | no | `string:date-time` |  |
| `desc` | no | `string` |  |
| `enabled` | no | `boolean` |  |
| `encode_status` | no | `EncodeStatusEnum` |  |
| `encoded_date` | yes | `string:date-time` |  |
| `exclusive_date` | no | `string:date` |  |
| `_exclusive_period_id` | no | `integer` |  |
| `guid` | no | `string` |  |
| `is_available_for_streaming` | yes | `boolean` |  |
| `is_classic` | no | `boolean` |  |
| `isrc` | no | `string` |  |
| `label_track_identifier` | no | `string` |  |
| `length_ms` | no | `integer` |  |
| `lifetime_exclusive` | no | `boolean` |  |
| `mix_name` | no | `string` |  |
| `name` | yes | `string` |  |
| `new_release_date` | yes | `string:date` |  |
| `number` | yes | `integer` |  |
| `pre_order_date` | no | `string:date` |  |
| `publish_date` | yes | `string:date` |  |
| `publish_status` | no | `PublishStatusEnum` |  |
| `streaming_date` | no | `string:date` |  |
| `updated` | no | `string:date-time` |  |
| `was_ever_exclusive` | yes | `boolean` |  |
| `audio_format` | no | `string:uri` |  |

### `TrackArtist`
Serializer for an artist/remixer/bsrc_remixer nested on a track
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `name` | yes | `string` |  |
| `slug` | yes | `string` |  |
| `url` | yes | `string:uri` |  |

### `TrackDetail`
A Mixin for dynicmally including fields based on a user's scopes.

If this Mixin is used an attr `include_by_scope` should be included
on the serializer that subclasses this mixin. This attr should be
a dict mapping field -> list of allowed scopes.

For example, the following definition would allow the field named
`dynamic_field` to be visible by admin user in baseware or a
marketing user in control.

```
class SerializerClass(ModelSerializer):
    ...

    include_by_scope = {
        'dynamic_field' = [
            ['app:baseware', 'staff:admin'],
            ['app:control', 'staff:marketing'],
        ]
    }
```
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `artists` | yes | `array`[`TrackArtist`] |  |
| `audio_format` | yes | `string:uri` |  |
| `available_worldwide` | yes | `boolean` |  |
| `bpm` | yes | `integer` |  |
| `bsrc_remixer` | no | `array`[`TrackArtist`] |  |
| `catalog_number` | yes | `string` |  |
| `current_status` | yes | `CurrentStatusList` |  |
| `desc` | yes | `string` |  |
| `enabled` | yes | `boolean` |  |
| `encode_status` | yes | `allOf`(`EncodeStatusEnum`) |  |
| `encoded_date` | yes | `string:date-time` |  |
| `exclusive` | yes | `boolean` |  |
| `exclusive_period` | yes | `ExclusivePeriodNested` |  |
| `free_downloads` | yes | `array`[`TrackFreeDownload`] |  |
| `free_download_start_date` | yes | `string` |  |
| `free_download_end_date` | yes | `string` |  |
| `genre` | yes | `GenreNested` |  |
| `hidden` | yes | `boolean` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `is_available_for_streaming` | yes | `boolean` |  |
| `is_explicit` | no | `boolean` |  |
| `is_ugc_remix` | yes | `boolean` |  |
| `is_dj_edit` | yes | `boolean` |  |
| `is_classic` | yes | `boolean` |  |
| `isrc` | yes | `string` |  |
| `key` | yes | `KeyList` |  |
| `label_track_identifier` | yes | `string` |  |
| `length` | yes | `string` |  |
| `length_ms` | yes | `integer` |  |
| `mix_name` | yes | `string` |  |
| `name` | yes | `string` |  |
| `new_release_date` | yes | `string:date` |  |
| `number` | yes | `integer` |  |
| `pre_order` | yes | `boolean` |  |
| `pre_order_date` | yes | `string:date` |  |
| `price` | yes | `object` |  |
| `publish_date` | yes | `string:date` |  |
| `publish_status` | yes | `allOf`(`PublishStatusEnum`) |  |
| `release` | yes | `string` |  |
| `remixers` | yes | `array`[`TrackArtist`] |  |
| `sale_type` | yes | `SaleTypeList` |  |
| `sample_url` | yes | `string` |  |
| `sample_start_ms` | yes | `integer` |  |
| `sample_end_ms` | yes | `integer` |  |
| `slug` | yes | `string` |  |
| `sub_genre` | yes | `SubGenreNested` |  |
| `territories` | yes | `array`[`string`] |  |
| `was_ever_exclusive` | yes | `boolean` |  |
| `is_hype` | yes | `boolean` |  |

### `TrackDownloadList`
Returns a partial response based on a `fields` query param.

Dynamically removes serializer fields.
This however, will not add additional fields that do not exist
on the Serializer itself.
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `artists` | yes | `array`[`TrackArtist`] |  |
| `audio_format` | yes | `AudioFormatNested` |  |
| `bpm` | yes | `integer` |  |
| `enabled` | yes | `boolean` |  |
| `genre` | yes | `GenreNested` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `initial_download` | yes | `boolean` |  |
| `is_available_for_streaming` | yes | `boolean` |  |
| `is_explicit` | no | `boolean` |  |
| `is_ugc_remix` | yes | `boolean` |  |
| `is_dj_edit` | yes | `boolean` |  |
| `key` | yes | `KeyList` |  |
| `length_ms` | yes | `integer` |  |
| `mix_name` | yes | `string` |  |
| `name` | yes | `string` |  |
| `new_release_date` | yes | `string:date` |  |
| `order_item_download_id` | yes | `integer` |  |
| `order_item_download_try_count` | yes | `integer` |  |
| `pre_order_date` | yes | `string:date` |  |
| `pre_order` | yes | `boolean` |  |
| `price` | yes | `object` |  |
| `publish_date` | yes | `string:date` |  |
| `purchase_date` | yes | `string:date-time` |  |
| `received_status` | yes | `boolean` |  |
| `release` | yes | `string` |  |
| `remixers` | yes | `array`[`TrackArtist`] |  |
| `sample_url` | yes | `string` |  |
| `sample_start_ms` | yes | `integer` |  |
| `sample_end_ms` | yes | `integer` |  |
| `slug` | yes | `string` |  |
| `sub_genre` | yes | `SubGenreNested` |  |
| `upgrade_fees` | yes | `object` |  |
| `url` | yes | `string:uri` |  |
| `tombstoned` | no | `boolean` |  |
| `is_hype` | yes | `boolean` |  |

### `TrackFreeDownload`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `url` | yes | `string:uri` |  |
| `updated_date` | yes | `string:date-time` |  |
| `updated_person_id` | no | `integer` |  |
| `created_date` | yes | `string:date-time` |  |
| `created_person_id` | no | `integer` |  |
| `start_date` | no | `string:date` |  |
| `end_date` | no | `string:date` |  |
| `track` | yes | `string:uri` |  |

### `TrackList`
A Mixin for dynicmally including fields based on a user's scopes.

If this Mixin is used an attr `include_by_scope` should be included
on the serializer that subclasses this mixin. This attr should be
a dict mapping field -> list of allowed scopes.

For example, the following definition would allow the field named
`dynamic_field` to be visible by admin user in baseware or a
marketing user in control.

```
class SerializerClass(ModelSerializer):
    ...

    include_by_scope = {
        'dynamic_field' = [
            ['app:baseware', 'staff:admin'],
            ['app:control', 'staff:marketing'],
        ]
    }
```
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `artists` | yes | `array`[`TrackArtist`] |  |
| `publish_status` | yes | `allOf`(`PublishStatusEnum`) |  |
| `available_worldwide` | yes | `boolean` |  |
| `bpm` | yes | `integer` |  |
| `bsrc_remixer` | no | `array`[`TrackArtist`] |  |
| `catalog_number` | yes | `string` |  |
| `current_status` | yes | `CurrentStatusList` |  |
| `encoded_date` | yes | `string:date-time` |  |
| `exclusive` | yes | `boolean` |  |
| `free_downloads` | yes | `array`[`TrackFreeDownload`] |  |
| `free_download_start_date` | yes | `string` |  |
| `free_download_end_date` | yes | `string` |  |
| `genre` | yes | `GenreNested` |  |
| `id` | yes | `integer` |  |
| `image` | yes | `string` |  |
| `is_available_for_streaming` | yes | `boolean` |  |
| `is_explicit` | no | `boolean` |  |
| `is_ugc_remix` | yes | `boolean` |  |
| `is_dj_edit` | yes | `boolean` |  |
| `isrc` | yes | `string` |  |
| `key` | yes | `KeyList` |  |
| `label_track_identifier` | yes | `string` |  |
| `length` | yes | `string` |  |
| `length_ms` | yes | `integer` |  |
| `mix_name` | yes | `string` |  |
| `name` | yes | `string` |  |
| `new_release_date` | yes | `string:date` |  |
| `pre_order` | yes | `boolean` |  |
| `pre_order_date` | yes | `string:date` |  |
| `price` | yes | `object` |  |
| `publish_date` | yes | `string:date` |  |
| `release` | yes | `string` |  |
| `remixers` | yes | `array`[`TrackArtist`] |  |
| `sale_type` | yes | `SaleTypeList` |  |
| `sample_url` | yes | `string` |  |
| `sample_start_ms` | yes | `integer` |  |
| `sample_end_ms` | yes | `integer` |  |
| `slug` | yes | `string` |  |
| `sub_genre` | yes | `SubGenreNested` |  |
| `tombstoned` | no | `boolean` |  |
| `url` | yes | `string:uri` |  |
| `is_hype` | yes | `boolean` |  |

### `Whether_the_playlist_is_a_user_or_curated_playlist.`
- Type: `object`

| Property | Required | Type | Description |
|---|---:|---|---|
| `id` | yes | `integer` |  |
| `name` | yes | `string` |  |
