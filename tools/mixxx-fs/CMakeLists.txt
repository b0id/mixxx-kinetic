# mixxx-fs - FUSE daemon for stream virtualization
# Note: No project() call - inherits from parent to access CMAKE_MODULE_PATH

# Use pkg-config directly for FUSE3
find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE3 REQUIRED fuse3)

find_package(CURL REQUIRED)

set(MIXXX_FS_SOURCES
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/streaming/bridge/fusedriver.cpp
    ${CMAKE_SOURCE_DIR}/src/streaming/bridge/sparsecache.cpp
)

add_executable(mixxx-fs ${MIXXX_FS_SOURCES})

target_link_libraries(mixxx-fs
    PRIVATE
    ${FUSE3_LIBRARIES}
    CURL::libcurl
)

target_include_directories(mixxx-fs
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${FUSE3_INCLUDE_DIRS}
)

target_compile_options(mixxx-fs
    PRIVATE
    ${FUSE3_CFLAGS_OTHER}
)

target_compile_definitions(mixxx-fs
    PRIVATE
    FUSE_USE_VERSION=31
)
