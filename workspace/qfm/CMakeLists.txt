project (qfm)

# Project header
project_executable_header_default ()

# 3rdparty
find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui REQUIRED)
include (${QT_USE_FILE})
list (APPEND ${PROJECT_NAME}_LIBS ${QT_LIBRARIES})
add_definitions (${QT_DEFINITIONS} -DQT_NO_CAST_FROM_ASCII)

list (APPEND ${PROJECT_NAME}_LIBS lvfs)
list (APPEND ${PROJECT_NAME}_LIBS lvfs-core)
list (APPEND ${PROJECT_NAME}_LIBS efc)

# Sources
add_subdirectory (src)
QT4_WRAP_UI (${PROJECT_NAME}_UIS_HEADERS ${${PROJECT_NAME}_UIS})
QT4_WRAP_CPP (${PROJECT_NAME}_MOC_SOURCES ${${PROJECT_NAME}_HEADERS} OPTIONS "-nw")
list (APPEND ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_MOC_SOURCES} ${${PROJECT_NAME}_UIS_HEADERS})

# Resources
#add_subdirectory (resources)
#QT4_ADD_RESOURCES (${PROJECT_NAME}_RESOURCES_BIN ${${PROJECT_NAME}_RESOURCES})
#list (APPEND ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_RESOURCES_BIN})

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

# Version
build_version_file (qfm 1.0.0)

# Documentation
add_documentation ("QFM" 1.0.0 "Qt File Manager")

# Install rules
install_target (qfm)

# Prerequisites
#include (InstallRequiredSystemLibraries)

#set (EXECUTABLE_PATH "${CMAKE_INSTALL_PREFIX}/bin/qfm${CMAKE_EXECUTABLE_SUFFIX}")
#set (MY_BUNDLE_UTILITIES_PATH "${qfm_SOURCE_DIR}/cmake/MyBundleUtilities.cmake")
#configure_file("${qfm_SOURCE_DIR}/cmake/FixBundle.cmake.in" "${qfm_BINARY_DIR}/FixBundle.cmake" @ONLY)

#install (SCRIPT "${qfm_BINARY_DIR}/FixBundle.cmake")
