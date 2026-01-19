#include "streaming/bridge/bridgeserver.h"

#include <QDebug>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocalSocket>

#include "streaming/bridge/bridgeprotocol.h"
#include "streaming/bridge/fusedriver.h"
#include "streaming/bridge/sparsecache.h"

using namespace mixxx::kinetic::bridge;

BridgeServer::BridgeServer(QObject* parent)
        : QObject(parent),
          m_server(new QLocalServer(this)) {
    connect(m_server, &QLocalServer::newConnection, this, &BridgeServer::onNewConnection);
}

BridgeServer::~BridgeServer() {
    close();
}

bool BridgeServer::listen() {
    // Clean up potentially stale socket file
    QLocalServer::removeServer(SOCKET_NAME);

    if (!m_server->listen(SOCKET_NAME)) {
        qCritical() << "BridgeServer: Failed to listen on" << SOCKET_NAME << m_server->errorString();
        return false;
    }

    // Set permissions to allow user access (0600 is default, ensuring only user can access)
    // QLocalServer creates u+rw
    qInfo() << "BridgeServer: Listening on" << SOCKET_NAME;
    return true;
}

void BridgeServer::close() {
    if (m_server->isListening()) {
        m_server->close();
    }
    qDeleteAll(m_clients);
    m_clients.clear();
}

void BridgeServer::onNewConnection() {
    QLocalSocket* socket = m_server->nextPendingConnection();
    if (!socket) {
        return;
    }

    qDebug() << "BridgeServer: New client connected";
    connect(socket, &QLocalSocket::readyRead, this, &BridgeServer::onReadyRead);
    connect(socket, &QLocalSocket::disconnected, this, &BridgeServer::onSocketDisconnected);

    m_clients.append(socket);
}

void BridgeServer::onSocketDisconnected() {
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    if (socket) {
        qDebug() << "BridgeServer: Client disconnected";
        m_clients.removeAll(socket);
        socket->deleteLater();
    }
}

void BridgeServer::onReadyRead() {
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    if (!socket) {
        return;
    }

    // Simple line-based or mostly JSON-based protocol
    // For robustness, we should use a length-prefix or newline delimiter.
    // Assuming newline delimited JSON for now.

    while (socket->canReadLine()) {
        QByteArray data = socket->readLine();
        handleRequest(socket, data);
    }
}

void BridgeServer::handleRequest(QLocalSocket* socket, const QByteArray& data) {
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "BridgeServer: JSON parse error:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "BridgeServer: Request must be a JSON object";
        return;
    }

    QJsonObject req = doc.object();
    QString op = req.value(json::OP).toString();

    QJsonObject response;

    if (op == op::REGISTER_TRACK) {
        response = handleRegisterTrack(req);
    } else if (op == op::UNREGISTER_TRACK) {
        response = handleUnregisterTrack(req);
    } else {
        response[json::STATUS] = "error";
        response[json::ERROR] = "Unknown operation: " + op;
    }

    sendResponse(socket, response);
}

void BridgeServer::sendResponse(QLocalSocket* socket, const QJsonObject& response) {
    QJsonDocument doc(response);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    data.append('\n'); // Delimiter
    socket->write(data);
    socket->flush();
}

QJsonObject BridgeServer::handleRegisterTrack(const QJsonObject& payload) {
    QJsonObject response;

    QString backingFile = payload.value(field::BACKING_FILE).toString();
    qint64 size = payload.value(field::SIZE).toDouble(); // JSON integers are doubles

    if (backingFile.isEmpty() || size <= 0) {
        response[json::STATUS] = "error";
        response[json::ERROR] = "Invalid arguments";
        return response;
    }

    // Register with FuseDriver
    if (auto* driver = FuseDriver::instance()) {
        inode = driver->registerFile(backingFile.toStdString(), size);
    } else {
        response[json::STATUS] = "error";
        response[json::ERROR] = "FuseDriver not available";
        return response;
    }

    response[json::STATUS] = "ok";
    response[field::INODE] = (double)inode;

    qInfo() << "BridgeServer: Registered track" << backingFile << "as inode" << inode;

    return response;
}

QJsonObject BridgeServer::handleUnregisterTrack(const QJsonObject& payload) {
    QJsonObject response;

    fuse_ino_t inode = (fuse_ino_t)payload.value(field::INODE).toDouble();
    if (inode == 0) {
        response[json::STATUS] = "error";
        response[json::ERROR] = "Invalid inode";
        return response;
    }

    if (auto* driver = FuseDriver::instance()) {
        driver->unregisterFile(inode);
        qInfo() << "BridgeServer: Unregistered inode" << inode;
        response[json::STATUS] = "ok";
    } else {
        response[json::STATUS] = "error";
        response[json::ERROR] = "FuseDriver not available";
    }

    return response;
}
