#include "rangefetcher.h"

#include <QDebug>
#include <QEventLoop>
#include <QTimer>

namespace mixxx {
namespace kinetic {

RangeFetcher::RangeFetcher(QObject* parent)
        : QObject(parent) {
}

QByteArray RangeFetcher::fetch(const QUrl& url, qint64 start, qint64 length) {
    QNetworkRequest request(url);
    QString rangeHeader = QString("bytes=%1-%2").arg(start).arg(start + length - 1);
    request.setRawHeader("Range", rangeHeader.toUtf8());
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply* reply = m_nam.get(request);

    // Synchronous wait
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray data;
    if (reply->error() == QNetworkReply::NoError) {
        data = reply->readAll();
        qDebug() << "RangeFetcher: Fetched" << data.size() << "bytes from" << start;
    } else {
        qWarning() << "RangeFetcher: Error fetching" << url << reply->errorString();
    }

    reply->deleteLater();
    return data;
}

qint64 RangeFetcher::fetchSize(const QUrl& url) {
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply* reply = m_nam.head(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qint64 size = -1;
    if (reply->error() == QNetworkReply::NoError) {
        if (reply->hasRawHeader("Content-Length")) {
            size = reply->rawHeader("Content-Length").toLongLong();
        }
        qDebug() << "RangeFetcher: Got size" << size << "for" << url;
    } else {
        qWarning() << "RangeFetcher: Error getting size" << url << reply->errorString();
    }

    reply->deleteLater();
    return size;
}

} // namespace kinetic
} // namespace mixxx
