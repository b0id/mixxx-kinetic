#include "streaming/bridge/bridgeclient.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

using namespace mixxx::kinetic::bridge;

BridgeClient::BridgeClient(QObject* parent)
        : QObject(parent),
          m_socket(new QLocalSocket(this)) {
}

BridgeClient::~BridgeClient() {
    if (m_socket->isOpen()) {
        m_socket->close();
    }
}

bool BridgeClient::connectToServer() {
    if (m_socket->state() == QLocalSocket::ConnectedState) {
        return true;
    }

    m_socket->connectToServer(SOCKET_NAME);
    if (!m_socket->waitForConnected(1000)) {
        qWarning() << "BridgeClient: Failed to connect to:" << SOCKET_NAME << m_socket->errorString();
        return false;
    }

    qDebug() << "BridgeClient: Connected to" << SOCKET_NAME;
    return true;
}

uint64_t BridgeClient::registerTrack(const QString& backingPath, qint64 size) {
    if (!connectToServer()) {
        return 0; // 0 is invalid/root usually? Root is 1. Invalid is 0.
    }

    QJsonObject req;
    req[json::OP] = op::REGISTER_TRACK;
    req[field::BACKING_FILE] = backingPath;
    req[field::SIZE] = (double)size;

    QJsonObject res = sendRequest(req);

    if (res[json::STATUS].toString() == "ok") {
        return (uint64_t)res[field::INODE].toDouble();
    }

    qWarning() << "BridgeClient: Register failed:" << res[json::ERROR].toString();
    return 0;
}

void BridgeClient::unregisterTrack(uint64_t inode) {
    if (!connectToServer()) {
        return;
    }

    QJsonObject req;
    req[json::OP] = op::UNREGISTER_TRACK;
    req[field::INODE] = (double)inode;

    // We don't necessarily need to wait for response for unregister
    // But sendRequest waits.
    sendRequest(req);
}

QJsonObject BridgeClient::sendRequest(const QJsonObject& req) {
    // Serialization
    QJsonDocument doc(req);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    data.append('\n'); // Delimiter

    if (m_socket->state() != QLocalSocket::ConnectedState) {
        return QJsonObject();
    }

    m_socket->write(data);
    if (!m_socket->waitForBytesWritten(1000)) {
        qWarning() << "BridgeClient: Timeout writing request";
        return QJsonObject();
    }

    // Wait for response
    // Needs a loop to read full line if fragmented?
    // QLocalSocket behavior for local pipes usually atomic for small writes, but better safe.
    // QLocalSocket behavior for local pipes usually atomic for small writes, but better safe.
    if (!m_socket->waitForReadyRead(3000)) { // 3s timeout
        qWarning() << "BridgeClient: Timeout waiting for response";
        return QJsonObject();
    }

    // Read response (assuming single line response)
    // TODO: More robust framing
    QByteArray responseData = m_socket->readLine();

    QJsonParseError error;
    QJsonDocument resDoc = QJsonDocument::fromJson(responseData, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "BridgeClient: JSON Parse error response:" << error.errorString();
        return QJsonObject();
    }

    if (!resDoc.isObject()) {
        return QJsonObject();
    }

    return resDoc.object();
}

#include "moc_bridgeclient.cpp"
