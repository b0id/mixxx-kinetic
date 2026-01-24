# Beatport Account API (extracted from saved docs HTML)

Source page: `https://account.beatport.com/docs` (saved Fri Jan 23 2026 10:49:26 GMT-0500)

Base URL (inferred from docs host): `https://account.beatport.com`

## Tag: OAuth

## GET `/o/authorize/`

**Summary:** Authorization Code Grant Flow

The start of the authorization code flow.
This route should be managed by you and should be configured to accept a code URL parameter which you will exchange for an access token.
If 3rd party authorization is enabled, the user will be asked to allow the client application to access its information.
The endpoint redirects user to the correct location (specified by
redirect_uri
) after Authorization. If the user is not scoped for the particular app, they will be redirected to a 401 Unathorized page.
There is no response, only redirection with a code query parameter. The grant token code will be set as a URL param code on the redirect URI.
Example: http://localhost:8000/o/redirect/?
code
=<CODE>
Exchange grant token code for an access token (or an access token and ID Token by adding
openid
to scopes).

### Parameters

| Name | In | Type | Required | Description | Example/Default |
|---|---|---|---:|---|---|
| response_type | query | string | yes | Must be "code" | code |
| client_id | query | string | yes | A unique client_id is provided to the client application | <CLIENT_ID> |
| code_challenge | query | string | yes | Client application provides this value | <CODE_CHALLENGE> |
| code_challenge_method | query | string | yes | Client application provides this value | S256 |
| redirect_uri | query | string | yes | Client application provides this value | <REDIRECT_URI> |
| scope | query | string | no | Should include "openid" scope alongside other client application scopes if code should be exchanged for access and ID Token | <SCOPE> |


---

## POST `/o/token/`

**Summary:** Client Credentials Grant Flow

An endpoint to provide access and refresh token to the client.
Your OAuth application should have a client id and secret that can be used to obtain an access token.
Anonymous token is used to access and browse our catalog in LoFi previews without the user needing to log in.
Access tokens last 10 minutes.
ID tokens last 30 days.
Refresh tokens last 31 days.
In order for ID token to be provided:
- all parameters
must
be sent in the request body
- your OAuth application must have an algorithm set

### Examples

```json
{
  "code": "<CODE>",
  "client_id": "<CLIENT_ID>",
  "client_secret": "<CLIENT_SECRET>",
  "grant_type": "authorization_code",
  "redirect_uri": "http://localhost:8000/o/redirect/",
  "code_verifier": "<CODE_VERIFIER>"
}
```

```json
{
  "access_token": "<ACCESS_TOKEN>",
  "expires_in": 36000,
  "token_type": "Bearer",
  "scope": "app:prostore user:dj",
  "refresh_token": "<REFRESH_TOKEN>"
}
```

### Responses

| Code | Description |
|---:|---|
| 200 | OK |


---

## GET `/o/introspect/`

**Summary:** Introspect

An endpoint for token introspection based on RFC 7662:
https://tools.ietf.org/html/rfc7662
.
Gets the token from the URL parameters.

### Parameters

| Name | In | Type | Required | Description | Example/Default |
|---|---|---|---:|---|---|
| token | query | string | yes |  | <TOKEN> |
| client_id | query | string | yes |  | <CLIENT_ID> |
| client_secret | query | string | yes |  | <CLIENT_SECRET> |

### Examples

```json
{
  "active": true,
  "scope": "read write",
  "exp": 1644825600,
  "client_id": "<CLIENT_ID>",
  "beatport_application_id": 12345,
  "beatport_application_name": "Beatport App",
  "beatport_application_app_scope": "app:scope",
  "username": "user123",
  "user_id": 1,
  "email": "email@email.com",
  "email_confirmed": true,
  "first_name": "first",
  "last_name": "last",
  "date_joined": "2024-04-03T05:35:48-06:00",
  "is_authenticated": true,
  "is_superuser": true,
  "is_staff": true
}
```

### Responses

| Code | Description |
|---:|---|
| 200 | OK |


---

## POST `/o/introspect/`

**Summary:** Introspect

An endpoint for token introspection based on RFC 7662:
https://tools.ietf.org/html/rfc7662
.
Gets the token from the body form parameters.

### Request body

Content-Type: `application/x-www-form-urlencoded`

| Field | Type | Description |
|---|---|---|
| token | string |  |
| client_id | string |  |
| client_secret | string |  |

### Examples

```json
{
  "active": true,
  "scope": "read write",
  "exp": 1644825600,
  "client_id": "<CLIENT_ID>",
  "beatport_application_id": 12345,
  "beatport_application_name": "Beatport App",
  "beatport_application_app_scope": "app:scope",
  "username": "user123",
  "user_id": 1,
  "email": "email@email.com",
  "email_confirmed": true,
  "first_name": "first",
  "last_name": "last",
  "date_joined": "2024-04-03T05:35:48-06:00",
  "is_authenticated": true,
  "is_superuser": true,
  "is_staff": true
}
```

### Responses

| Code | Description |
|---:|---|
| 200 | OK |


---

## GET `/o/userinfo/`

**Summary:** User Info

An endpoint to provide user information.
Gets user information using ID Token in Authorization header.

### Examples

```json
{
  "sub": "dj",
  "email": "email@email.com",
  "username": "dj",
  "firstName": "John",
  "lastName": "Doe",
  "emailConfirmed": true
}
```

### Responses

| Code | Description |
|---:|---|
| 200 | OK |


---

## Schemas

### TokenBody

| Property | Type | Example |
|---|---|---|
| access_token | string | <ACCESS_TOKEN> |
| expires_in | integer | 36000 |
| token_type | string | Bearer |
| scope | string | app:prostore user:dj |
| refresh_token | string | <REFRESH_TOKEN> |

### IntrospectBody

| Property | Type | Example |
|---|---|---|
| active | boolean | true |
| scope | string | read write |
| exp | integer | 1644825600 |
| client_id | string | <CLIENT_ID> |
| beatport_application_id | integer | 12345 |
| beatport_application_name | string | Beatport App |
| beatport_application_app_scope | string | app:scope |
| username | string | user123 |
| user_id | integer | 1 |
| email | string | email@email.com |
| email_confirmed | boolean | true |
| first_name | string | first |
| last_name | string | last |
| date_joined | string | 2024-04-03T05:35:48-06:00 |
| is_authenticated | boolean | true |
| is_superuser | boolean | true |
| is_staff | boolean | true |

### UserInfoBody

| Property | Type | Example |
|---|---|---|
| sub | string | dj |
| email | string | email@email.com |
| username | string | dj |
| firstName | string | John |
| lastName | string | Doe |
| emailConfirmed | boolean | true |
