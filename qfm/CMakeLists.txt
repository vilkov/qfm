cmake_minimum_required (VERSION 2.8)
project (qfm)
message (STATUS "System: " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION})
message (STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})
if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
	if (MSVC)
		message(STATUS "Compiler: MSVC, version: " ${MSVC_VERSION})
	endif ()
	if (BORLAND) 
		message(STATUS "Compiler: BCC") 
	endif () 
	else () 
		message(STATUS "Only GCC is supported on Linux")
endif () 
message (STATUS "CMake generates " ${CMAKE_GENERATOR})

# Main settings
set (USE_GPROF   NO)

# Custom modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${qfm_SOURCE_DIR}/cmake")


# Fix path delimeters
if (WIN32)
    file (TO_CMAKE_PATH ${CMAKE_INSTALL_PREFIX} CMAKE_INSTALL_PREFIX)
endif ()


# 3rdparty
find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui QtXml REQUIRED)
include (${QT_USE_FILE})
set (qfm_LIBS ${qfm_LIBS} "${QT_LIBRARIES}")
set (qfm_DEFINITIONS ${qfm_DEFINITIONS} ${QT_DEFINITIONS})

#find_package (QtSingleInstance REQUIRED)
#set (qfm_INCLUDES ${qfm_INCLUDES} "${QTSINGLEINSTANCE_INCLUDE}")
#set (qfm_LIBS ${qfm_LIBS} "${QTSINGLEINSTANCE_LIBRARY}")

find_package (Stlsoft REQUIRED)
set (qfm_INCLUDES ${qfm_INCLUDES} "${STLSOFT_INCLUDE}")


# Includes

# Link directories

# Libs
if (USE_GPROF)
	set (qfm_LIBS ${qfm_LIBS} gmon)
endif (USE_GPROF)

 
# Definitions
set (qfm_DEFINITIONS ${qfm_DEFINITIONS} -DUNICODE)

# We're require at least Windows 2000 (http://msdn.microsoft.com/en-us/library/Aa383745)
if (WIN32)
    set (qfm_DEFINITIONS ${qfm_DEFINITIONS} -D_WIN32_WINNT=0x0500 -DWINVER=0x0500)
endif ()


# Sources
add_subdirectory("src")
QT4_WRAP_UI(qfm_UIS_HEADERS ${qfm_UIS})
QT4_WRAP_CPP (qfm_MOC_SOURCES ${qfm_HEADERS} OPTIONS "-nw")
set (qfm_SOURCES ${qfm_SOURCES} ${qfm_MOC_SOURCES} ${qfm_UIS_HEADERS})


# Resources
add_subdirectory("resources")
QT4_ADD_RESOURCES(qfm_RESOURCES_BIN ${qfm_RESOURCES})
set (qfm_SOURCES ${qfm_SOURCES} ${qfm_RESOURCES_BIN})


# Includes and Definitions
include_directories (${qfm_INCLUDES})
link_directories (${qfm_LIB_DIRS})
add_definitions (${qfm_DEFINITIONS})

# Set some OS specific settings
if (WIN32)
    set (GUI_TYPE WIN32)
elseif (APPLE)
    set (GUI_TYPE MACOSX_BUNDLE)
endif ()


# Target - qfm
add_executable (qfm ${GUI_TYPE} ${qfm_SOURCES})
target_link_libraries (qfm ${qfm_LIBS})


# Target propertys - qfm
if (USE_GPROF)
    set (CMAKE_CXX_FLAGS        ${CMAKE_CXX_FLAGS}        -pg)
    set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -pg)
endif (USE_GPROF)


# Install
include ("cmake/Install.cmake")