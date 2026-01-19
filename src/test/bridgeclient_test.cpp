#include "streaming/bridge/bridgeclient.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>
#include <future>
#include <iostream>

#include "streaming/bridge/bridgeprotocol.h"

using namespace mixxx::kinetic::bridge;

class BridgeClientTest : public testing::Test {
  protected:
    void SetUp() override {
        // Ensure we have a QCoreApplication for events
        if (!QCoreApplication::instance()) {
            static int argc = 1;
            static char* argv[] = {(char*)"test"};
            app = new QCoreApplication(argc, argv);
        }
    }

    QCoreApplication* app = nullptr;
};

TEST_F(BridgeClientTest, ConnectsToMockServer) {
    // Create a mock server
    QLocalServer server;
    // Remove if exists
    QLocalServer::removeServer(SOCKET_NAME);

    ASSERT_TRUE(server.listen(SOCKET_NAME));

    BridgeClient client;
    bool connected = client.connectToServer();
    EXPECT_TRUE(connected);

    server.close();
}

TEST_F(BridgeClientTest, RegisterTrackSendReceive) {
    QLocalServer server;
    QLocalServer::removeServer(SOCKET_NAME);
    ASSERT_TRUE(server.listen(SOCKET_NAME));

    // Run blocking client in a separate thread
    std::future<uint64_t> result = std::async(std::launch::async, []() {
        // Delay slightly to ensure server is listening? No, listen() is synchronous.
        BridgeClient client;
        return client.registerTrack("/tmp/test.mp3", 1024);
    });

    // Server logic: synchronous wait
    if (!server.waitForNewConnection(5000)) {
        // Check result to see if client failed
        if (result.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
            result.get(); // Should probably be 0
        }
        FAIL() << "Server timed out";
    }

    QLocalSocket* socket = server.nextPendingConnection();
    ASSERT_TRUE(socket != nullptr);

    if (!socket->waitForReadyRead(5000)) {
        FAIL() << "Server read timeout";
    }

    // Read loop
    while (socket->bytesAvailable()) {
        if (!socket->canReadLine()) {
            // Wait more if line not complete?
            socket->waitForReadyRead(1000);
        }
        QByteArray data = socket->readLine();
        // std::cerr << "TestServer: Received: " << data.toStdString() << std::endl;

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject req = doc.object();

        // Reply
        QJsonObject res;
        res[json::STATUS] = "ok";
        res[field::INODE] = 42.0;

        QJsonDocument resDoc(res);
        QByteArray respData = resDoc.toJson(QJsonDocument::Compact);
        respData.append('\n');
        socket->write(respData);
        socket->flush();
        socket->waitForBytesWritten(1000);
        // std::cerr << "TestServer: Sent response: " << respData.toStdString() << std::endl;
    }

    uint64_t inode = result.get();
    EXPECT_EQ(inode, 42);

    server.close();
}

TEST_F(BridgeClientTest, UnregisterTrackSendReceive) {
    QLocalServer server;
    QLocalServer::removeServer(SOCKET_NAME);
    ASSERT_TRUE(server.listen(SOCKET_NAME));

    // Client thread
    std::future<void> result = std::async(std::launch::async, []() {
        BridgeClient client;
        client.unregisterTrack(42);
    });

    // Server logic
    ASSERT_TRUE(server.waitForNewConnection(5000));
    QLocalSocket* socket = server.nextPendingConnection();
    ASSERT_TRUE(socket != nullptr);

    ASSERT_TRUE(socket->waitForReadyRead(5000));
    QByteArray data = socket->readLine();
    // Verify request
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject req = doc.object();
    EXPECT_EQ(req[json::OP].toString(), op::UNREGISTER_TRACK);
    EXPECT_EQ(req[field::INODE].toDouble(), 42.0);

    // Send reply
    QJsonObject res;
    res[json::STATUS] = "ok";
    QJsonDocument resDoc(res);
    QByteArray respData = resDoc.toJson(QJsonDocument::Compact);
    respData.append('\n');
    socket->write(respData);
    socket->flush();
    socket->waitForBytesWritten(1000);

    // Wait for client to finish
    result.get();
    server.close();
}
