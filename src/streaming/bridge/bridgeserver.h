#pragma once

#include <QList>
#include <QLocalServer>
#include <QObject>

class QLocalSocket;

class BridgeServer : public QObject {
    Q_OBJECT
  public:
    explicit BridgeServer(QObject* parent = nullptr);
    ~BridgeServer() override;

    bool listen();
    void close();

  private slots:
    void onNewConnection();
    void onReadyRead();
    void onSocketDisconnected();

  private:
    void handleRequest(QLocalSocket* socket, const QByteArray& data);
    void sendResponse(QLocalSocket* socket, const QJsonObject& response);

    // Request Handlers
    QJsonObject handleRegisterTrack(const QJsonObject& payload);
    QJsonObject handleUnregisterTrack(const QJsonObject& payload);

    QLocalServer* m_server;
    QList<QLocalSocket*> m_clients;
};
