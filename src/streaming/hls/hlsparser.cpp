#include "streaming/hls/hlsparser.h"

#include <openssl/evp.h>

#include <QRegularExpression>
#include <QTextStream>

HlsParser::HlsParser()
        : m_targetDuration(0),
          m_mediaSequence(0) {
}

bool HlsParser::parse(const QByteArray& m3u8Content, const QUrl& baseUrl) {
    m_segments.clear();
    m_key = HlsKey();

    QTextStream stream(m3u8Content);
    QString line;
    double currentSegmentDuration = 0.0;

    while (stream.readLineInto(&line)) {
        line = line.trimmed();

        if (line.isEmpty()) {
            continue;
        }

        if (line.startsWith("#")) {
            if (line.startsWith("#EXT-X-TARGETDURATION:")) {
                m_targetDuration = line.mid(22).toDouble();
            } else if (line.startsWith("#EXT-X-MEDIA-SEQUENCE:")) {
                m_mediaSequence = line.mid(22).toInt();
            } else if (line.startsWith("#EXT-X-KEY:")) {
                parseKeyLine(line, baseUrl);
            } else if (line.startsWith("#EXTINF:")) {
                // Format: #EXTINF:duration,title
                QString durationStr = line.mid(8);
                int commaIndex = durationStr.indexOf(',');
                if (commaIndex != -1) {
                    durationStr = durationStr.left(commaIndex);
                }
                currentSegmentDuration = durationStr.toDouble();
            }
        } else {
            // Segment URL
            HlsSegment seg;
            seg.url = baseUrl.resolved(QUrl(line)).toString();
            seg.duration = currentSegmentDuration;
            seg.sequence = m_mediaSequence + m_segments.size();
            m_segments.append(seg);
        }
    }

    return !m_segments.isEmpty();
}

void HlsParser::parseKeyLine(const QString& line, const QUrl& baseUrl) {
    // Example: #EXT-X-KEY:METHOD=AES-128,URI="https://...",IV=0x...

    // METHOD
    QRegularExpression methodRe("METHOD=([^,]+)");
    auto methodMatch = methodRe.match(line);
    if (methodMatch.hasMatch()) {
        m_key.method = methodMatch.captured(1);
    }

    // URI
    QRegularExpression uriRe("URI=\"([^\"]+)\"");
    auto uriMatch = uriRe.match(line);
    if (uriMatch.hasMatch()) {
        QString relativeUri = uriMatch.captured(1);
        m_key.uri = baseUrl.resolved(QUrl(relativeUri)).toString();
    }

    // IV
    QRegularExpression ivRe("IV=0x([0-9a-fA-F]+)");
    auto ivMatch = ivRe.match(line);
    if (ivMatch.hasMatch()) {
        m_key.iv = QByteArray::fromHex(ivMatch.captured(1).toUtf8());
    }
}

QByteArray HlsParser::decryptSegment(const QByteArray& encrypted,
        const QByteArray& key,
        const QByteArray& iv) {
    if (key.size() != 16 || iv.size() != 16) {
        return QByteArray();
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return QByteArray();
    }

    // AES-128-CBC
    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.constData()), reinterpret_cast<const unsigned char*>(iv.constData())) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return QByteArray();
    }

    QByteArray decrypted;
    decrypted.resize(encrypted.size() + EVP_CIPHER_block_size(EVP_aes_128_cbc()));

    int outLen = 0;
    int totalLen = 0;

    if (EVP_DecryptUpdate(ctx,
                reinterpret_cast<unsigned char*>(decrypted.data()),
                &outLen,
                reinterpret_cast<const unsigned char*>(encrypted.constData()),
                encrypted.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return QByteArray();
    }
    totalLen = outLen;

    if (EVP_DecryptFinal_ex(ctx,
                reinterpret_cast<unsigned char*>(decrypted.data()) + outLen,
                &outLen) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        // Decryption failed (padding error?)
        return QByteArray();
    }
    totalLen += outLen;

    EVP_CIPHER_CTX_free(ctx);
    decrypted.resize(totalLen);
    return decrypted;
}
