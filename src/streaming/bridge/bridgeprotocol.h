#pragma once

#include <QString>

namespace mixxx {
namespace kinetic {
namespace bridge {

// IPC Socket location
// For now, hardcoded to /tmp for simplicity.
// In production, we should respect XDG_RUNTIME_DIR.
static const QString SOCKET_NAME = QStringLiteral("/tmp/mixxx-kinetic-bridge.sock");

// Protocol JSON Keys
namespace json {
static const QString OP = QStringLiteral("op");
static const QString PAYLOAD = QStringLiteral("payload");
static const QString STATUS = QStringLiteral("status");
static const QString ERROR = QStringLiteral("error");
static const QString DATA = QStringLiteral("data");
} // namespace json

// Operations
namespace op {
static const QString REGISTER_TRACK = QStringLiteral("register");
static const QString UNREGISTER_TRACK = QStringLiteral("unregister");
} // namespace op

// Request/Response Fields
namespace field {
static const QString PATH = QStringLiteral("path");
static const QString BACKING_FILE = QStringLiteral("backing_file");
static const QString SIZE = QStringLiteral("size");
static const QString INODE = QStringLiteral("inode");
} // namespace field

} // namespace bridge
} // namespace kinetic
} // namespace mixxx
