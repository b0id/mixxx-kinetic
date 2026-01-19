# New streaming module sources
target_sources(mixxx-lib PRIVATE
    src/streaming/hook/streamingservice.h
    src/streaming/hook/streamingservice.cpp
    src/streaming/hook/streamingdto.h
    src/streaming/hook/oauthmanager.h
    src/streaming/hook/oauthmanager.cpp
    src/streaming/hook/beatportservice.h
    src/streaming/hook/beatportservice.cpp
    src/streaming/bridge/sparsecache.h
    src/streaming/bridge/sparsecache.cpp
    src/streaming/bridge/bridgeclient.h
    src/streaming/bridge/bridgeclient.cpp
    # Temporarily disabled until interface fix
    # src/sources/soundsourcekineticproxy.h
    # src/sources/soundsourcekineticproxy.cpp
    # src/sources/soundsourcestream.h
    # src/sources/soundsourcestream.cpp
)


