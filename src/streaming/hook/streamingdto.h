#pragma once

#include <QDateTime>
#include <QPair>
#include <QString>
#include <QUrl>

class StreamingService;

enum class AuthState {
    LoggedOut,
    Authorizing,
    Exchanging,
    LoggedIn,
    Refreshing,
    Error
};

enum class SubscriptionTier {
    None,
    Basic,
    Premium,     // Beatport Link, Tidal HiFi
    Professional // Beatport Link Pro, Tidal HiFi Plus
};

enum class OfflineStatus {
    CloudOnly = 0,
    PartialCache = 1,
    FullCache = 2,
    OfflinePermanent = 3
};

struct TrackMetadata {
    QString remoteId; // Service-specific ID
    QString title;
    QString artist; // Normalized, comma-separated
    QString album;
    int bpm;     // Integer BPM
    QString key; // Normalized to user preference (Camelot/OpenKey)
    int durationMs;
    QString coverArtUrl;       // Remote URL (Hook downloads and caches)
    QString coverArtLocalPath; // Local cached path
    QString genre;
    int year;
    QString label;
    StreamingService* source;
};

struct StreamInfo {
    QString trackId;
    QUrl streamUrl;      // Temporary, signed URL
    QDateTime expiresAt; // URL expiration time
    QString mimeType;    // "application/vnd.apple.mpegurl", "application/dash+xml"
    QString codec;       // "aac", "flac", "mqa"
    int bitrate;         // kbps
    bool encrypted;      // DRM protected
    QString licenseUrl;  // Widevine license server (if encrypted)
    QString licenseUrl;  // Widevine license server (if encrypted)
    QString keyId;       // DRM key ID (if encrypted)
    int durationMs;
};

struct SearchQuery {
    QString text;
    QString artist;           // Optional filter
    QString genre;            // Optional filter
    QPair<int, int> bpmRange; // Optional filter
    QString key;              // Optional filter
    int limit = 50;
    int offset = 0;
};
