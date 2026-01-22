#pragma once

#include <QByteArray>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include <QWaitCondition>
#include <functional>
#include <memory>

namespace mixxx {
namespace kinetic {

class RangeFetcher : public QObject {
    Q_OBJECT
  public:
    explicit RangeFetcher(QObject* parent = nullptr);

    // Synchronous fetch for simplicity in FUSE context (blocking the worker thread)
    // In production, we might want async + caching, but FUSE read() blocks anyway.
    QByteArray fetch(const QUrl& url, qint64 start, qint64 length);

    // Get total size (HEAD request)
    qint64 fetchSize(const QUrl& url);

  private:
};

} // namespace kinetic
} // namespace mixxx
