#pragma once

#include <QDateTime>
#include <QFuture>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVector>

#include "streamingdto.h"

class StreamingService : public QObject {
    Q_OBJECT
  public:
    explicit StreamingService(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~StreamingService() = default;

    // Identity
    virtual QString serviceName() const = 0;
    virtual QString serviceId() const = 0; // "beatport", "tidal"

    // Authentication
    virtual void initiateLogin() = 0;
    virtual void logout() = 0;
    virtual bool isAuthenticated() const = 0;
    virtual SubscriptionTier getSubscriptionTier() const = 0;

    // Catalog
    virtual QFuture<TrackMetadata> fetchTrackMetadata(const QString& trackId) = 0;
    virtual QFuture<QVector<TrackMetadata>> search(const SearchQuery& query) = 0;
    virtual QFuture<QVector<TrackMetadata>> getPlaylist(const QString& playlistId) = 0;

    // Streaming
    virtual QFuture<StreamInfo> getStreamInfo(const QString& trackId) = 0;

  signals:
    void authStateChanged(AuthState newState);
    void loginError(const QString& message);
    void tokenRefreshed();

  protected:
    // Subclasses implement these
    virtual QUrl getAuthorizationUrl() const = 0;
    // Note: specialized structs might need full definition or being separate
    // keeping it abstract for now as per spec
};
