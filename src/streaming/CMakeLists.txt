# New streaming module sources
target_sources(mixxx-lib PRIVATE
    src/streaming/hook/streamingservice.h
    src/streaming/hook/streamingservice.cpp
    src/streaming/hook/streamingdto.h
    src/streaming/hook/oauthmanager.h
    src/streaming/hook/oauthmanager.cpp
    src/streaming/hook/beatportservice.h
    src/streaming/hook/beatportservice.cpp
    src/sources/soundsourcekineticproxy.h
    src/sources/soundsourcekineticproxy.cpp
    src/sources/soundsourcestream.h
    src/sources/soundsourcestream.cpp
)

# Disable AUTOMOC for files that manually include their moc files
# (following Mixxx coding standard)
set_source_files_properties(
    src/streaming/hook/streamingservice.h
    src/streaming/hook/oauthmanager.h
    src/streaming/hook/beatportservice.h
    PROPERTIES
    SKIP_AUTOMOC ON
)
