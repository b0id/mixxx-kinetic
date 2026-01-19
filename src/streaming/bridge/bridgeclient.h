#pragma once

#include <QFuture> // If we use QtConcurrent or just a simple signal for now?
#include <QLocalSocket>
#include <QObject>
// Actually simpler to just use signals/methods for now to avoid complex Future deps if not present.
// Let's use a blocking method for the first pass for simplicity of loading,
// or a signal `trackRegistered(inode)`.

#include <cstdint>

#include "streaming/bridge/bridgeprotocol.h"

class BridgeClient : public QObject {
    Q_OBJECT
  public:
    explicit BridgeClient(QObject* parent = nullptr);
    ~BridgeClient() override;

    bool connectToServer();

    // Blocking call for now to simplify "get inode or fail" logic
    // In strict UI programming we shouldn't block, but for a local socket
    // it should be fast. We can refactor to async later.
    uint64_t registerTrack(const QString& backingPath, qint64 size);

    void unregisterTrack(uint64_t inode);

  private:
    QLocalSocket* m_socket;

    QJsonObject sendRequest(const QJsonObject& req);
};
