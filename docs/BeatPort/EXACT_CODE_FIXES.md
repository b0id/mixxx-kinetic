# Exact Code Fixes - Copy/Paste Ready

**Goal**: Get streaming working with minimal changes

---

## 🚨 CRITICAL: Test First

```bash
# Does /stream/ need auth? Test now:
curl https://api.beatport.com/v4/catalog/tracks/23050990/stream/

# If you get JSON back: Auth NOT needed for preview
# If you get 401: Auth needed
```

---

## FIX 1: Change Endpoint

**File**: `/home/b0id/repo/src/streaming/hook/beatportservice.cpp`
**Line**: 326-337

**REPLACE**:
```cpp
    // Use /download endpoint instead of /stream
    QUrl url(QString("%1/catalog/tracks/%2/download").arg(m_config.baseUrl, trackId));
    QUrlQuery urlQuery;
    // Request quality based on subscription tier
    // beatportdl uses "lossless" default, but we'll be specific based on tier
    if (m_subscriptionTier >= SubscriptionTier::Professional) {
        urlQuery.addQueryItem("quality", "256-aac"); // or "lossless" if supported
    } else if (m_subscriptionTier >= SubscriptionTier::Premium) {
        urlQuery.addQueryItem("quality", "256-aac");
    } else {
        urlQuery.addQueryItem("quality", "128-aac");
    }
    url.setQuery(urlQuery);
```

**WITH**:
```cpp
    // Use /stream/ endpoint (returns HLS manifest URL)
    QUrl url(QString("%1/catalog/tracks/%2/stream/").arg(m_config.baseUrl, trackId));
```

---

## FIX 2: Parse JSON Response

**File**: `/home/b0id/repo/src/streaming/hook/beatportservice.cpp`
**Lines**: 347-390 (the QNetworkReply::finished handler)

**FIND** the line that starts with:
```cpp
    connect(reply, &QNetworkReply::finished, this, [this, reply, trackId]() {
```

**REPLACE** the entire lambda body with:
```cpp
    connect(reply, &QNetworkReply::finished, this, [this, reply, trackId]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            mixxx::kinetic::StreamingError error;

            if (httpStatus > 0) {
                error = mixxx::kinetic::StreamingError::fromHttpStatus(
                        httpStatus,
                        QString("Failed to get stream info: %1").arg(reply->errorString()));
            } else {
                error = mixxx::kinetic::StreamingError::networkError(reply->errorString());
            }

            error.context = "getting stream URL";
            error.trackId = trackId;
            emit streamingError(error);
            emit streamInfoFailed(trackId, error);
            mixxx::kinetic::StreamingErrorHandler::instance()->reportError(error);
            return;
        }

        // Parse JSON response from /stream/ endpoint
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (!doc.isObject()) {
            qWarning() << "BeatportService: Invalid JSON response from /stream/";
            mixxx::kinetic::StreamingError error = mixxx::kinetic::StreamingError::parseError(
                    "Invalid JSON response from stream endpoint");
            error.context = "parsing stream response";
            error.trackId = trackId;
            emit streamingError(error);
            emit streamInfoFailed(trackId, error);
            mixxx::kinetic::StreamingErrorHandler::instance()->reportError(error);
            return;
        }

        QJsonObject obj = doc.object();
        QString hlsUrl = obj["stream_url"].toString();

        if (hlsUrl.isEmpty()) {
            qWarning() << "BeatportService: No stream_url in response";
            mixxx::kinetic::StreamingError error = mixxx::kinetic::StreamingError::parseError(
                    "No stream_url field in response");
            error.context = "parsing stream response";
            error.trackId = trackId;
            emit streamingError(error);
            emit streamInfoFailed(trackId, error);
            mixxx::kinetic::StreamingErrorHandler::instance()->reportError(error);
            return;
        }

        qInfo() << "BeatportService: Got HLS URL:" << hlsUrl;

        // WORKAROUND: Convert M3U8 manifest to first segment for initial testing
        // Full HLS support will be implemented later
        // Example M3U8: https://needledrop.beatport.com/.../file.128k.aac.m3u8
        // First segment: https://needledrop.beatport.com/.../file.128k.aac-1.aac
        QString segmentUrl = hlsUrl;
        segmentUrl.replace(".m3u8", "-1.aac");

        qInfo() << "BeatportService: Using first segment:" << segmentUrl;

        StreamInfo info;
        info.streamUrl = segmentUrl;
        info.format = "aac";        // Beatport uses AAC
        info.bitrate = 128;         // 128kbps for preview
        info.sampleRate = 44100;    // Standard CD quality
        info.channels = 2;          // Stereo

        emit streamInfoReady(trackId, info);
        qInfo() << "BeatportService: Stream info emitted for track" << trackId;
    });
```

---

## FIX 3: Optional - Skip Auth for Testing

**File**: `/home/b0id/repo/src/streaming/hook/beatportservice.cpp`
**Lines**: 312-323 (in getStreamInfo function)

**IF** `/stream/` endpoint doesn't need auth (test with curl first), you can comment out the auth check:

**FIND**:
```cpp
    if (!isAuthenticated()) {
        qWarning() << "BeatportService: Not authenticated, cannot get stream info";
        mixxx::kinetic::StreamingError error = mixxx::kinetic::StreamingError::authError(
                "Not authenticated");
        error.context = "getting stream info";
        error.trackId = trackId;
        emit streamingError(error);
        emit streamInfoFailed(trackId, error);
        mixxx::kinetic::StreamingErrorHandler::instance()->reportError(error);
        return QFuture<StreamInfo>();
    }
```

**REPLACE WITH** (temporary - remove later when auth works):
```cpp
    // TEMPORARY: Skip auth check for testing if preview streaming is public
    // TODO: Re-enable once we confirm auth requirements
    /*
    if (!isAuthenticated()) {
        qWarning() << "BeatportService: Not authenticated, cannot get stream info";
        mixxx::kinetic::StreamingError error = mixxx::kinetic::StreamingError::authError(
                "Not authenticated");
        error.context = "getting stream info";
        error.trackId = trackId;
        emit streamingError(error);
        emit streamInfoFailed(trackId, error);
        mixxx::kinetic::StreamingErrorHandler::instance()->reportError(error);
        return QFuture<StreamInfo>();
    }
    */
    qInfo() << "BeatportService: Auth check skipped for testing";
```

**ALSO** update the Authorization header section (line 340):

**FIND**:
```cpp
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());
```

**REPLACE WITH**:
```cpp
    QNetworkRequest request(url);
    // Only add auth if we have a token (preview may not need it)
    QString accessToken = m_pOAuthManager->getAccessToken(serviceId());
    if (!accessToken.isEmpty()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());
    }
```

---

## BUILD & TEST

```bash
cd /home/b0id/repo/build
cmake --build . --target mixxx
./mixxx
```

---

## EXPECTED RESULT

### If it works:
- ✅ Track loads
- ✅ ~3 seconds of audio plays (first segment)
- ✅ Then silence (need full HLS implementation)

### If it fails:
- Check console for error messages
- Run curl tests from DEBUG_AUTH_STEP_BY_STEP.md
- Report error message

---

## NEXT PHASE: Full HLS Support

Once first segment plays, implement full HLS:

1. Fetch M3U8 manifest
2. Parse segment list
3. Stream all segments sequentially
4. Full track playback

See `Implementing_HLS_Parser.md` (to be created) for details.

---

## Summary

**3 changes needed**:
1. Change endpoint from `/download` to `/stream/`
2. Parse JSON response to extract HLS URL
3. Convert M3U8 to first segment (workaround)

**Result**: ~3 seconds of audio (proof of concept)

**Time**: 10-15 minutes
