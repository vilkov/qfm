project (qfm)

# Project header
project_cxx_executable_header_default ()

# 3rdparty
if (WIN32)
    find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui QtXml REQUIRED)
else (UNIX)
    find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui QtXml QtDbus REQUIRED)
endif ()
include (${QT_USE_FILE})
list (APPEND ${PROJECT_NAME}_LIBS ${QT_LIBRARIES})
add_definitions (${QT_DEFINITIONS} -DQT_NO_CAST_FROM_ASCII)

find_package (Sqlite REQUIRED)
include_directories (${SQLITE_INCLUDE})
list (APPEND ${PROJECT_NAME}_LIBS ${SQLITE_LIBRARY})

if (UNIX)
    find_package (X11 REQUIRED)
    include_directories (${X11_INCLUDE_DIR})
    list (APPEND ${PROJECT_NAME}_LIBS ${X11_LIBRARIES})
endif ()

find_package (LibArchive REQUIRED)
include_directories (${LIBARCHIVE_INCLUDE_DIR})
list (APPEND ${PROJECT_NAME}_LIBS ${LIBARCHIVE_LIBRARY})

find_package (LibUnrar REQUIRED)
include_directories (${LIBUNRAR_INCLUDE})
list (APPEND ${PROJECT_NAME}_LIBS ${LIBUNRAR_LIBRARY})

list (APPEND ${PROJECT_NAME}_LIBS liquiddb)
list (APPEND ${PROJECT_NAME}_LIBS xdg)
list (APPEND ${PROJECT_NAME}_LIBS efc)

# Includes
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

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
add_dependencies (qfm platform)

# It is because of Qt (qglobal.h)
set_target_properties (qfm PROPERTIES COMPILE_FLAGS "-Wno-long-long")

# Version
build_version_file (qfm 0.1.0)

# Documentation
add_documentation ("QFM" 0.1.0 "Qt File Manager")
