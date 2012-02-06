cmake_minimum_required (VERSION 2.8)
project (qfm)


# Main information
if (CMAKE_CROSSCOMPILING)
    message (STATUS "Cross compiling to: " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION})
else ()
    message (STATUS "System: " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION})
endif ()
message (STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})
if (MSVC)
	message(STATUS "Compiler: MSVC, version: " ${MSVC_VERSION})

	# C CXX Flags - turn on WARNINGS
    set (PROJECT_COMP_FLAGS "/W3")
    
elseif (BORLAND)
	message(STATUS "Compiler: BCC")
elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUC) 
	message(STATUS "Compiler: GCC")
	
	# C CXX Flags - turn on WARNINGS
    set (PROJECT_COMP_FLAGS "-Wall -Wextra -pedantic -Wundef -Wcast-align -Wpointer-arith -Wformat")

	# C CXX Flags - turn off WARNINGS
    set (PROJECT_COMP_FLAGS "${PROJECT_COMP_FLAGS} -Wno-unused-parameter -Wno-parentheses -Wno-shadow -Wno-overloaded-virtual -Wno-ignored-qualifiers")
	
	# C CXX Flags - WARNINGS to ERRORS
    set (PROJECT_COMP_FLAGS "${PROJECT_COMP_FLAGS} -Werror=return-type -Werror=non-virtual-dtor")
    
	# C CXX Flags - turn on CODE GEN
    set (PROJECT_COMP_FLAGS "${PROJECT_COMP_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
    
	# C CXX Flags - turn off CODE GEN
    set (PROJECT_COMP_FLAGS "${PROJECT_COMP_FLAGS} -fno-exceptions -fno-check-new -fno-common -fno-threadsafe-statics")

    #Useful link: "http://gcc.gnu.org/onlinedocs/gcc/Invoking-GCC.html"
else () 
	message (STATUS "Unknown compiler")
endif ()
message (STATUS "CMake generates " ${CMAKE_GENERATOR})


# Main settings
set (USE_GPROF    NO)
set (UNIT_TESTING NO CACHE BOOL "Compile binary executable for unit testing?")

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


# Custom modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")


# Fix path delimeters
if (WIN32)
    file (TO_CMAKE_PATH ${CMAKE_INSTALL_PREFIX} CMAKE_INSTALL_PREFIX)
endif ()


# Set up compiler flags
set (CMAKE_CXX_FLAGS "${PROJECT_COMP_FLAGS}")


# 3rdparty
if (WIN32)
    find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui QtXml REQUIRED)
else (UNIX)
    find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui QtXml QtDbus REQUIRED)
endif ()
include (${QT_USE_FILE})
set (qfm_LIBS ${qfm_LIBS} ${QT_LIBRARIES})
add_definitions (${QT_DEFINITIONS})

find_package (Stlsoft REQUIRED)
include_directories (${STLSOFT_INCLUDE})

find_package (Sqlite REQUIRED)
include_directories (${SQLITE_INCLUDE})
set (qfm_LIBS ${qfm_LIBS} ${SQLITE_LIBRARY})

if (UNIX)
    find_package (X11 REQUIRED)
    include_directories (${X11_INCLUDE_DIR})
    set (qfm_LIBS ${qfm_LIBS} ${X11_LIBRARIES})
endif ()

find_package (LibArchive REQUIRED)
include_directories (${LIBARCHIVE_INCLUDE_DIR})
set (qfm_LIBS ${qfm_LIBS} ${LIBARCHIVE_LIBRARY})


# Includes

# Link directories

# Libs
if (USE_GPROF)
	set (qfm_LIBS ${qfm_LIBS} gmon)
endif (USE_GPROF)
 
# Definitions
if (UNIX)
    # This is for xdgmimecache
    add_definitions (-DHAVE_MMAP)
    
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
)

# We're require at least Windows 2000 (http://msdn.microsoft.com/en-us/library/Aa383745)
if (WIN32)
    add_definitions (-D_WIN32_WINNT=0x0500 -DWINVER=0x0500)
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


# Set some OS specific settings
if (WIN32)
    set (GUI_TYPE WIN32)
elseif (APPLE)
    set (GUI_TYPE MACOSX_BUNDLE)
endif ()


# Target - qfm
add_executable (qfm ${GUI_TYPE} ${qfm_SOURCES})
target_link_libraries (qfm ${qfm_LIBS})

# It is because of Qt (qglobal.h)
set_target_properties (qfm PROPERTIES COMPILE_FLAGS "-Wno-long-long")

# Compiler flags
if (USE_GPROF)
    set (CMAKE_CXX_FLAGS        ${CMAKE_CXX_FLAGS}        -pg)
    set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -pg)
endif (USE_GPROF)


# Install
include ("cmake/Install.cmake")
