cmake_minimum_required (VERSION 2.8)
project (qfm)


# Custom modules
list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")


# Project header
include (project_cxx_header)


# Main settings
option (USE_GPROF    "Use GProf for profiling?"                    NO)
option (UNIT_TESTING "Compile binary executable for unit testing?" NO)

if (UNIX)
    set (DESKTOP_ENVIRONMENT "KDE" CACHE STRING "Desktop environment")
    set_property(CACHE DESKTOP_ENVIRONMENT PROPERTY STRINGS
        "KDE"
        "GNOME"
        "Xfce4")
else (WIN32)
    set (DESKTOP_ENVIRONMENT "Windows")
endif ()

set (PROJECT_GENERATED_FILES_DIR "${PROJECT_BINARY_DIR}/generated")


# Fix path delimeters
if (WIN32)
    file (TO_CMAKE_PATH ${CMAKE_INSTALL_PREFIX} CMAKE_INSTALL_PREFIX)
endif ()


# 3rdparty
if (WIN32)
    find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui QtXml REQUIRED)
else (UNIX)
    find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui QtXml QtDbus REQUIRED)
endif ()
include (${QT_USE_FILE})
list (APPEND ${PROJECT_NAME}_LIBS ${QT_LIBRARIES})
add_definitions (${QT_DEFINITIONS})

find_package (Sqlite REQUIRED)
include_directories (${SQLITE_INCLUDE})
list (APPEND ${PROJECT_NAME}_LIBS ${SQLITE_LIBRARY})

if (UNIX)
    find_package (X11 REQUIRED)
    include_directories (${X11_INCLUDE_DIR})
    list (APPEND ${PROJECT_NAME}_LIBS ${X11_LIBRARIES})
    
    find_package (XDG REQUIRED)
    include_directories (${XDG_INCLUDE})
    list (APPEND ${PROJECT_NAME}_LIBS ${XDG_LIBRARY})
endif ()

find_package (LibArchive REQUIRED)
include_directories (${LIBARCHIVE_INCLUDE_DIR})
list (APPEND ${PROJECT_NAME}_LIBS ${LIBARCHIVE_LIBRARY})

find_package (LibUnrar REQUIRED)
include_directories (${LIBUNRAR_INCLUDE})
list (APPEND ${PROJECT_NAME}_LIBS ${LIBUNRAR_LIBRARY})


# Includes
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link directories

# Libs
if (USE_GPROF)
	list (APPEND ${PROJECT_NAME}_LIBS gmon)
endif (USE_GPROF)
 
# Definitions
if (UNIX)
    if (DESKTOP_ENVIRONMENT STREQUAL "KDE")
        set (DESKTOP_ENVIRONMENT_IS_KDE YES)
        add_definitions (-DDESKTOP_ENVIRONMENT_IS_KDE)
    else ()
        set (DESKTOP_ENVIRONMENT_IS_GTK YES)
        add_definitions (-DDESKTOP_ENVIRONMENT_IS_GTK)
    endif ()

    add_definitions (-D_FILE_OFFSET_BITS=64)
elseif (WIN32)
    set (DESKTOP_ENVIRONMENT_IS_WINDOWS YES)
    add_definitions (-DDESKTOP_ENVIRONMENT_IS_WINDOWS)
endif ()

add_definitions (
    -DUNICODE
    -DTOOLS_THREADS_BACKEND_QT
    -DTOOLS_TYPES_BACKEND_QT
    -DTOOLS_POINTERS_BACKEND_QT
    
    -DQT_NO_CAST_FROM_ASCII
)

# We're require at least Windows 2000 (http://msdn.microsoft.com/en-us/library/Aa383745)
if (WIN32)
    add_definitions (-D_WIN32_WINNT=0x0500 -DWINVER=0x0500)
endif ()


# Sources
add_subdirectory (src)
QT4_WRAP_UI (${PROJECT_NAME}_UIS_HEADERS ${${PROJECT_NAME}_UIS})
QT4_WRAP_CPP (${PROJECT_NAME}_MOC_SOURCES ${${PROJECT_NAME}_HEADERS} OPTIONS "-nw")
list (APPEND ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_MOC_SOURCES} ${${PROJECT_NAME}_UIS_HEADERS})


# Resources
add_subdirectory (resources)
QT4_ADD_RESOURCES (${PROJECT_NAME}_RESOURCES_BIN ${${PROJECT_NAME}_RESOURCES})
list (APPEND ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_RESOURCES_BIN})


# Set some OS specific settings
if (WIN32)
    set (GUI_TYPE WIN32)
elseif (APPLE)
    set (GUI_TYPE MACOSX_BUNDLE)
endif ()


# Target - qfm
add_executable (qfm ${GUI_TYPE} ${${PROJECT_NAME}_SOURCES})
target_link_libraries (qfm ${${PROJECT_NAME}_LIBS})

# It is because of Qt (qglobal.h)
set_target_properties (qfm PROPERTIES COMPILE_FLAGS "-Wno-long-long")

# Compiler flags
if (USE_GPROF)
    list (CMAKE_CXX_FLAGS        APPEND -pg)
    list (CMAKE_EXE_LINKER_FLAGS APPEND -pg)
endif (USE_GPROF)


# Install
include ("cmake/Install.cmake")
