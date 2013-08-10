cmake_minimum_required (VERSION 2.8)


# Custom modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/platform/cmake")


# Fix path delimeters
if (WIN32)
    file (TO_CMAKE_PATH ${CMAKE_INSTALL_PREFIX} CMAKE_INSTALL_PREFIX)
endif ()


# Versioning
include (version)


# Include file generator
include (include)


# Project header
include (project_header)
print_system_info ()


# Tests
include (tests)
add_project_tests_main_target ()


# Documentation
include (doxygen)
add_project_documentation_main_target ()


# Platform definition
include (platform)
if (UNIX)
    platform_options_begin (xdg/cmake/platform_options.cmake
                            qfm/cmake/platform_options_unix.cmake)
    platform_option_value (DE_KDE ON)
else (WIN32)
    platform_options_begin (qfm/cmake/platform_options_win32.cmake)
endif ()
platform_options_end ()


# Includes
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

# Definitions

# Libs


# 3rdparty
add_subdirectory (platform)
add_subdirectory (xdg)


# Project - qfm
add_subdirectory (qfm)
