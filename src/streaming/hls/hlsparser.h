#pragma once

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVector>

struct HlsSegment {
    QString url;
    double duration;
    int sequence;
};

struct HlsKey {
    QByteArray key; // 16 bytes
    QByteArray iv;  // 16 bytes
    QString method;
    QString uri;
};

class HlsParser {
  public:
    HlsParser();

    bool parse(const QByteArray& m3u8Content, const QUrl& baseUrl);

    QVector<HlsSegment> segments() const {
        return m_segments;
    }

    HlsKey key() const {
        return m_key;
    }

    double targetDuration() const {
        return m_targetDuration;
    }

    static QByteArray decryptSegment(const QByteArray& encrypted,
            const QByteArray& key,
            const QByteArray& iv);

  private:
    void parseKeyLine(const QString& line, const QUrl& baseUrl);

    QVector<HlsSegment> m_segments;
    HlsKey m_key;
    double m_targetDuration;
    int m_mediaSequence;
};
