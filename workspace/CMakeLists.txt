cmake_minimum_required (VERSION 2.8)


# Custom modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/platform/cmake")


# Platform
include (platform)
initialize_platform ()


# Install
include (platform/install)


# Tests
include (platform/tests)
add_project_tests_main_target ()


# Documentation
include (platform/doxygen)
add_project_documentation_main_target ()


# Platform definition
include (platform/options)
platform_options_begin (lvfs/cmake/platform_options.cmake
                        lvfs-arc/cmake/platform_options.cmake
                        xdg/cmake/platform_options.cmake
                        qfm/cmake/platform_options_unix.cmake)
    platform_option_value (DE_KDE                           ON)
    platform_option_value (ENABLE_POSITION_INDEPENDENT_CODE ON)
platform_options_end ()


# 3rdparty
add_subdirectory (platform)
add_subdirectory (brolly)
add_subdirectory (efc)
add_subdirectory (lvfs)
add_subdirectory (liquiddb)
add_subdirectory (xdg)
add_subdirectory (libunrar)
#add_subdirectory (kitemviews)

include_directories (libtorrent/include)
add_subdirectory (libtorrent)

add_subdirectory (lvfs-db)
add_subdirectory (lvfs-arc)
add_subdirectory (lvfs-qimg)
add_subdirectory (lvfs-bits)
add_subdirectory (lvfs-qplayer)
add_subdirectory (lvfs-core)

# Project - qfm
add_subdirectory (qfm)
