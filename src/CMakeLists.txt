# CMake tools
include ("collect_sources")

collect_sources (src_HEADERS src_SOURCES ${CMAKE_SOURCE_DIR})
set (USE_WIN_CONTEXTMENU NO)

# Headers
if (NOT USE_WIN_CONTEXTMENU)
    set (tmp_src_HEADERS "")
    foreach (src_HEADER ${src_HEADERS})
        if (NOT ${src_HEADER} MATCHES "contextmenu_win.h$" AND
            NOT ${src_HEADER} MATCHES "ShellContextMenu.h$")
            set (tmp_src_HEADERS ${tmp_src_HEADERS} ${src_HEADER})
        endif ()
    endforeach ()
    set (src_HEADERS ${tmp_src_HEADERS})
endif ()
set (qfm_HEADERS ${qfm_HEADERS} ${src_HEADERS} PARENT_SCOPE)

# Sources
if (NOT USE_WIN_CONTEXTMENU)
    set (tmp_src_SOURCES "")
    foreach (src_SOURCE ${src_SOURCES})
        if (NOT ${src_SOURCE} MATCHES "contextmenu_win.cpp$" AND
            NOT ${src_SOURCE} MATCHES "ShellContextMenu.cpp$")
            set (tmp_src_SOURCES ${tmp_src_SOURCES} ${src_SOURCE})
        endif ()
    endforeach ()
    set (src_SOURCES ${tmp_src_SOURCES})
endif ()
set (qfm_SOURCES ${qfm_SOURCES} ${src_SOURCES} PARENT_SCOPE)

# UIs
file (GLOB_RECURSE src_UIS RELATIVE ${qfm_SOURCE_DIR} FOLLOW_SYMLINKS "*.ui")
set (qfm_UIS ${qfm_UIS} ${src_UIS} PARENT_SCOPE)
