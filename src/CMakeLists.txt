# CMake tools
include ("collect_sources")


collect_sources (src_HEADERS src_SOURCES ${CMAKE_SOURCE_DIR})


# Regex
if (UNIX)
    # FIXME: It shouldn't be this way!
    # CMake doesn't create relative path correctly because of SYMLINK. 
    file (GLOB_RECURSE tmp_0 FOLLOW_SYMLINKS "de/unix/xdgmime/*.h")
    set (src_HEADERS ${src_HEADERS} ${tmp_0})
    
    file (GLOB_RECURSE tmp_0 FOLLOW_SYMLINKS "de/unix/xdgmime/*.c")
    set (src_SOURCES ${src_SOURCES} ${tmp_0})


    # Exclude "main.c" file from xdgmime library.
    set (tmp_src_SOURCES "")
    foreach (src_SOURCE ${src_SOURCES})
        if (NOT ${src_SOURCE} MATCHES ".*/main.c" OR ${src_SOURCE} MATCHES ".*/main.cpp")
            set (tmp_src_SOURCES ${tmp_src_SOURCES} ${src_SOURCE})
        endif ()
    endforeach ()
    set (src_SOURCES ${tmp_src_SOURCES})


    if (DESKTOP_ENVIRONMENT_IS_KDE)
        # Headers
        set (tmp_src_HEADERS "")
        foreach (src_HEADER ${src_HEADERS})
            if (NOT ${src_HEADER} MATCHES ".*/gtk/.*")
                set (tmp_src_HEADERS ${tmp_src_HEADERS} ${src_HEADER})
            endif ()
        endforeach ()
        set (src_HEADERS ${tmp_src_HEADERS})

        # Sources
        set (tmp_src_SOURCES "")
        foreach (src_SOURCE ${src_SOURCES})
            if (NOT ${src_SOURCE} MATCHES ".*/gtk/.*")
                set (tmp_src_SOURCES ${tmp_src_SOURCES} ${src_SOURCE})
            endif ()
        endforeach ()
        set (src_SOURCES ${tmp_src_SOURCES})
    else ()
        # Headers
        set (tmp_src_HEADERS "")
        foreach (src_HEADER ${src_HEADERS})
            if (NOT ${src_HEADER} MATCHES ".*/kde/.*")
                set (tmp_src_HEADERS ${tmp_src_HEADERS} ${src_HEADER})
            endif ()
        endforeach ()
        set (src_HEADERS ${tmp_src_HEADERS})

        # Sources
        set (tmp_src_SOURCES "")
        foreach (src_SOURCE ${src_SOURCES})
            if (NOT ${src_SOURCE} MATCHES ".*/kde/.*")
                set (tmp_src_SOURCES ${tmp_src_SOURCES} ${src_SOURCE})
            endif ()
        endforeach ()
        set (src_SOURCES ${tmp_src_SOURCES})
    endif ()


    set (EXCLUDE_FILES_MASK ".*/win32/.*")
elseif (WIN32)
    set (EXCLUDE_FILES_MASK ".*/unix/.*")
endif ()


# Headers
set (tmp_src_HEADERS "")
foreach (src_HEADER ${src_HEADERS})
    if (NOT ${src_HEADER} MATCHES ${EXCLUDE_FILES_MASK})
        set (tmp_src_HEADERS ${tmp_src_HEADERS} ${src_HEADER})
    endif ()
endforeach ()
set (src_HEADERS ${tmp_src_HEADERS})
set (qfm_HEADERS ${qfm_HEADERS} ${src_HEADERS} PARENT_SCOPE)


# Sources
set (tmp_src_SOURCES "")
foreach (src_SOURCE ${src_SOURCES})
    if (NOT ${src_SOURCE} MATCHES ${EXCLUDE_FILES_MASK})
        set (tmp_src_SOURCES ${tmp_src_SOURCES} ${src_SOURCE})
    endif ()
endforeach ()
set (src_SOURCES ${tmp_src_SOURCES})
set (qfm_SOURCES ${qfm_SOURCES} ${src_SOURCES} PARENT_SCOPE)


# UIs
file (GLOB_RECURSE src_UIS RELATIVE ${qfm_SOURCE_DIR} FOLLOW_SYMLINKS "*.ui")
set (qfm_UIS ${qfm_UIS} ${src_UIS} PARENT_SCOPE)
