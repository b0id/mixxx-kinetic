Identity Service

1.0.0

OAS 3.0

This is the external Identity service documentation.

The process below describes how to integrate with our Identity Service via an Authorization Code grant flow to retrieve an access token:

    The login action should make a call to the /o/authorize route with your client ID, grant type, and redirect URI

    The user will enter our Identity Service UI where they can log in or create an account.

    Once successfully authorized a code will be returned to the redirect URI. In some cases, the user will be asked to authorize the client to gain access their Beatport resources, once they accept a code will be sent to the redirect URI.

    Use the code in the /o/token route to retrieve an access token. This route requires a client ID, client secret, grant type, code, and redirect URI.

    We will return an access token as well as a refresh token, use the access token to make calls to our API.

    To refresh the token, call the /o/token route with grant_type=refresh_token

More information on our Identity Service can be found here
OAuth
GET
/o/authorize/
Authorization Code Grant Flow

The start of the authorization code flow.

This route should be managed by you and should be configured to accept a code URL parameter which you will exchange for an access token.

If 3rd party authorization is enabled, the user will be asked to allow the client application to access its information.

The endpoint redirects user to the correct location (specified by redirect_uri) after Authorization. If the user is not scoped for the particular app, they will be redirected to a 401 Unathorized page.

There is no response, only redirection with a code query parameter. The grant token code will be set as a URL param code on the redirect URI.

Example: http://localhost:8000/o/redirect/?code=BogqefHC6qZrWexKXbstQNohV2gtS1

Exchange grant token code for an access token (or an access token and ID Token by adding openid to scopes).
Parameters
Name Description
response_type \*
string
(query)

Must be "code"
client_id \*
string
(query)

A unique client_id is provided to the client application
code_challenge \*
string
(query)

Client application provides this value
code_challenge_method \*
string
(query)

Client application provides this value
redirect_uri \*
string
(query)

Client application provides this value
scope
string
(query)

Should include "openid" scope alongside other client application scopes if code should be exchanged for access and ID Token
POST
/o/token/
Client Credentials Grant Flow

An endpoint to provide access and refresh token to the client.

Your OAuth application should have a client id and secret that can be used to obtain an access token.

Anonymous token is used to access and browse our catalog in LoFi previews without the user needing to log in.

Access tokens last 10 minutes.

ID tokens last 30 days.

Refresh tokens last 31 days.

In order for ID token to be provided:

- all parameters must be sent in the request body

- your OAuth application must have an algorithm set
  Parameters

No parameters
Request body

Request body must be of content type application/x-www-form-urlencoded
Examples:

{
"code": "gXzNFsxm1Q783YppwSYYFSYPHhb31J",
"client_id": "FshkzMI9mUeRhH0WK6xWCfvzohkEC2juMW3i0KjY",
"client_secret": "rRry3GCCnvB64O7xkwLAZefYtJJX967iawzGgtHAjR8iZXP6lLmDUmMp8unPo8haou2L6vHXkRS5exCSQAb0NHfY4ReIETUfh6R6NbdtQZJqQstfka6s0mTGmApiySvu",
"grant_type": "authorization_code",
"redirect_uri": "http://localhost:8000/o/redirect/",
"code_verifier": "DIDAPXYDR9B145M9E1KDGERZDX0OF11IXC966OXJZSVG055KF70Q355NZYYBD63W1M9UO21AWNRXS4LBEDIOF6UZ9DGYQ04QE0HUYW"
}

Responses
Code Description Links
200

OK
Media type
Controls Accept header.

{
"access_token": "eyJ0eXAiOiAiSldUIiwgImFsZyI6ICJSUzI1NiIsICJraWQi",
"expires_in": 36000,
"token_type": "Bearer",
"scope": "app:prostore user:dj",
"refresh_token": "bQ0Zqr1USOjY35k59uG69RqM6DHkIF"
}

    No links

GET
/o/introspect/
Introspect

An endpoint for token introspection based on RFC 7662: https://tools.ietf.org/html/rfc7662.

Gets the token from the URL parameters.
Parameters
Name Description
token _
string
(query)
client_id _
string
(query)
client_secret \*
string
(query)
Responses
Code Description Links
200

OK
Media type
Controls Accept header.

{
"active": true,
"scope": "read write",
"exp": 1644825600,
"client_id": "client123",
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

    No links

POST
/o/introspect/
Introspect

An endpoint for token introspection based on RFC 7662: https://tools.ietf.org/html/rfc7662.

Gets the token from the body form parameters.
Parameters

No parameters
Request body
token
string
client_id
string
client_secret
string
Responses
Code Description Links
200

OK
Media type
Controls Accept header.

{
"active": true,
"scope": "read write",
"exp": 1644825600,
"client_id": "client123",
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

    No links

GET
/o/userinfo/
User Info

An endpoint to provide user information.

Gets user information using ID Token in Authorization header.
Parameters

No parameters
Responses
Code Description Links
200

OK
Media type
Controls Accept header.

{
"sub": "dj",
"email": "email@email.com",
"username": "dj",
"firstName": "John",
"lastName": "Doe",
"emailConfirmed": true
}

    No links

Schemas
TokenBody{
access_token [...]
expires_in [...]
token_type [...]
scope [...]
refresh_token [...]
}
IntrospectBody{
active [...]
scope [...]
exp [...]
client_id [...]
beatport_application_id [...]
beatport_application_name [...]
beatport_application_app_scope [...]
username [...]
user_id [...]
email [...]
email_confirmed [...]
first_name [...]
last_name [...]
date_joined [...]
is_authenticated [...]
is_superuser [...]
is_staff [...]
}
UserInfoBody{
sub [...]
email [...]
username [...]
firstName [...]
lastName [...]
emailConfirmed [...]
}

How to get a JWT access token

The process below is a summary of how to retrieve an access token via an Authorization Code grant flow with our Identity Service:

    The login action should send the user to the account.beatport.com/o/authorize URL. Include the following parameters:
        response_type=code
        client_id={client_id provided}
        redirect_uri={redirect_uri shared with us, can not be different}
        code_challenge={generated by your system}
        code_challenge_method=S256
        scope (optional) - see OpenID section below
    The user will enter our Identity Service UI where they can log in or create an account.
    Once successfully logged in, an authorize code will be returned to the redirect URI provided.
    You will now retrieve a JWT token by making a POST call to the /o/token route. Include the following:
        grant_type=authorization_code
        code={login code received}
        client_id
        client_secret={client_secret provided) *note this is only required if you are set as a confidential client
        code_verifier={generated by your system}
        redirect_uri={must be the same redirect URI from the /o/authorize/ URL)
    The response will return the following
        access_token: A JWT token used for make Beatport API calls
        expires_in: This will be set to 600s and indicates that the access token will not longer be valid after 600s
        scope: The scope assigned to the token which determines the level of access you have
        refresh_token: Used in the /o/token route with grant_type=refresh_token to receive a new access token. See refreshing access token section below.

Example authorize URL excluding scope:

https://account.beatport.com/o/authorize/
?response_type=code
&client_id=••••••
&code_challenge=••••••
&code_challenge_method=S256
&redirect_uri=www.beatport.com
&scope=app%3Aexternaltrusted%20user%3Adj%20openid

Example cURL for the POST /o/token call:

curl --location 'https://account.beatport.com/o/token/' \
--header 'Content-Type: application/x-www-form-urlencoded' \
--header 'Cookie: ••••••' \
--data-urlencode 'code=••••••' \
--data-urlencode 'code_verifier=••••••' \
--data-urlencode 'client_id=••••••' \
--data-urlencode 'client_secret=••••••' \
--data-urlencode 'grant_type=authorization_code' \
--data-urlencode 'redirect_uri=https://www.beatport.com/'

Response example from the POST /o/token call:

"access_token": ••••••
"expires_in": 600,
"token_type": "Bearer",
"scope": "app:externaltrusted user:dj",
"refresh_token": ••••••

Identity Service API documentation

Our Identity Service API documentation can be found here and has examples of how to retrieve an:

    Anonymous token:  this token is used to access and browse our catalog in LoFi previews without the user needing to log in.
    JWT token: this token lasts 10 minutes and will have a specific scope assigned based on your Client ID. Your scope will determine which API routes you will have access to.
    Refresh token: this token is used to obtain a new access token, it lasts 31 days unless specifically revoked by the user via the "Log out" action.

Using your JWT token with Beatport API

Once you have obtained a JWT token you can use this token to make calls to the Beatport API. Documentation on Beatport API can be found here

Your access token will have an assigned scope which will determine which API routes you can access.

Refreshing your access token

JWT access tokens last 10 minutes. After 10 mins the access token is no longer valid and API calls will fail. For uninterrupted use you should be refreshing the access token within 10 mins. To refresh a token, use the refresh token received and make a POST /o/token/ with the following in the body:

    grant_type=refresh_token
    client_id={client_id provided}
    client_secret={client_secret provided) *note this is only required if you are set as a confidential client
    refresh_toekn={refresh_token received}

A refresh token can be stored for up to 31 days and can only be used once. Once the refresh token is used successfully it is revoked, you will receive a new refresh token alongside a new access token.

The refresh token is also revoked if the user explicitly logs out.

Here is a cURL example:

curl --location 'https://account.beatport.com/o/token/' \
--header 'Content-Type: application/x-www-form-urlencoded' \
--header 'Cookie: ••••••' \
--data-urlencode 'client_id=••••••' \
--data-urlencode 'client_secret=••••••' \
--data-urlencode 'grant_type=refresh_token' \
--data-urlencode 'refresh_token=••••••'

Profile information - OpenID

You can retrieve a users information by making a call to the /o/userinfo/ route using the OpenID token. This token is not your JWT access token and is only received if you add a scope to the /o/authorize URL. If you intend to use this route please reach out to use for the correct scope to use.

To get an Open ID token, add a scope to the authorize URL and include the following value:

{your scope} user:dj openid

ex: app:externaltrusted user:dj openid

Example authorize URL including scope:

https://account.beatport.com/o/authorize/
?response_type=code
&client_id=••••••
&code_challenge=••••••
&code_challenge_method=S256
&redirect_uri=www.beatport.com
&scope=app%3Aexternaltrusted%20user%3Adj%20openid

Once the user logs in successfully, you will receive 2 tokens:

    access_token - used for API calls
    id_token - used only for the o/userinfo/ route

Response example from the POST /o/token call that includes open_id

"access_token": ••••••
"expires_in": 600,
"token_type": "Bearer",
"scope": "app:externaltrusted user:dj openid",
"refresh_token": ••••••
"id_token": ••••••

Depending on your token's scope, you may also have more access to a user's information which can be retrieved from the /my/ API4 routes. Details on the /my/ route can be found on our API4 docs.

Account settings

Once a user is logged in, we require that you provide an option for them to be able to access their Beatport account at account.beatport.com from your application. This will allow the user to manage their settings such as updating their email address, name, password, etc.

Logout a user

It is required to offer a logout option on your application. The action should do 2 things:

    From your client app, clear the user's cookies and anything else related to authentication
    Send the user to the following URL with {YourSiteURL} to redirect them back to your application. This will act as a session logout and logout the user from the Identity Service on this browser session only
    account.beatport.com/identity/v1/logout/?callbackUrl={YourSiteURL}

    If this redirect is not performed, the user will remain logged in on account.beatport.com which is a security issue. Please ensure this action is taken.
