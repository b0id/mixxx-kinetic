# New streaming module sources
target_sources(mixxx-lib PRIVATE
    src/streaming/hook/streamingservice.h
    src/streaming/hook/streamingservice.cpp
    src/streaming/hook/streamingdto.h
    src/streaming/manifestgenerator.h
    src/streaming/manifestgenerator.cpp
    src/streaming/ui/streambrowserwidget.h
    src/streaming/ui/streambrowserwidget.cpp
    src/streaming/ui/logindialog.h
    src/streaming/ui/logindialog.cpp
    src/preferences/dialog/dlgprefstreaming.h
    src/preferences/dialog/dlgprefstreaming.cpp
    src/streaming/hook/oauthmanager.h
    src/streaming/hook/oauthmanager.cpp
    src/streaming/hook/beatportservice.h
    src/streaming/hook/beatportservice.cpp
    src/streaming/hook/beatportfeature.h
    src/streaming/hook/beatportfeature.cpp
    src/streaming/hook/tokeninterceptor.h
    src/streaming/hook/tokeninterceptor.cpp
    src/streaming/bridge/sparsecache.h
    src/streaming/bridge/sparsecache.cpp
    src/streaming/bridge/bridgeclient.h
    src/streaming/bridge/bridgeclient.cpp
    src/streaming/bridge/prefetcher.h
    src/streaming/bridge/prefetcher.cpp
    src/sources/soundsourcekineticproxy.h
    src/sources/soundsourcekineticproxy.cpp
    src/sources/soundsourcestream.h
    src/sources/soundsourcestream.cpp
)


