# FindFuse3.cmake
#
# Finds the Fuse3 library
#
# This will define the following variables:
#
# FUSE3_FOUND
# FUSE3_INCLUDE_DIRS
# FUSE3_LIBRARIES
# FUSE3_VERSION
#
# And the following imported target:
#
# FUSE3::FUSE3

find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_FUSE3 QUIET fuse3)

find_path(FUSE3_INCLUDE_DIR
    NAMES fuse_lowlevel.h
    PATHS ${PC_FUSE3_INCLUDE_DIRS}
    PATH_SUFFIXES fuse3
)

find_library(FUSE3_LIBRARY
    NAMES fuse3
    PATHS ${PC_FUSE3_LIBRARY_DIRS}
)

set(FUSE3_VERSION ${PC_FUSE3_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FUSE3
    REQUIRED_VARS FUSE3_LIBRARY FUSE3_INCLUDE_DIR
    VERSION_VAR FUSE3_VERSION
)

if(FUSE3_FOUND AND NOT TARGET FUSE3::FUSE3)
    add_library(FUSE3::FUSE3 UNKNOWN IMPORTED)
    set_target_properties(FUSE3::FUSE3 PROPERTIES
        IMPORTED_LOCATION "${FUSE3_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${FUSE3_INCLUDE_DIR}"
        INTERFACE_COMPILE_DEFINITIONS "FUSE_USE_VERSION=31"
    )
endif()

mark_as_advanced(FUSE3_INCLUDE_DIR FUSE3_LIBRARY)
