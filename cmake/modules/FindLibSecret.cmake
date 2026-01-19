# FindLibSecret.cmake
#
# Finds the LibSecret library
#
# This will define the following variables:
#
# LIBSECRET_FOUND
# LIBSECRET_INCLUDE_DIRS
# LIBSECRET_LIBRARIES
# LIBSECRET_VERSION
#
# And the following imported target:
#
# LIBSECRET::LIBSECRET

find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_LIBSECRET QUIET libsecret-1)

find_path(LIBSECRET_INCLUDE_DIR
    NAMES libsecret/secret.h
    PATHS ${PC_LIBSECRET_INCLUDE_DIRS}
    PATH_SUFFIXES libsecret-1
)

find_library(LIBSECRET_LIBRARY
    NAMES secret-1
    PATHS ${PC_LIBSECRET_LIBRARY_DIRS}
)

set(LIBSECRET_VERSION ${PC_LIBSECRET_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibSecret
    REQUIRED_VARS LIBSECRET_LIBRARY LIBSECRET_INCLUDE_DIR
    VERSION_VAR LIBSECRET_VERSION
)

if(LIBSECRET_FOUND AND NOT TARGET LIBSECRET::LIBSECRET)
    add_library(LIBSECRET::LIBSECRET UNKNOWN IMPORTED)
    set_target_properties(LIBSECRET::LIBSECRET PROPERTIES
        IMPORTED_LOCATION "${LIBSECRET_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LIBSECRET_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(LIBSECRET_INCLUDE_DIR LIBSECRET_LIBRARY)
