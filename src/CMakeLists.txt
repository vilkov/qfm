# CMake tools
include ("collect_sources")


collect_sources (src_HEADERS src_SOURCES ${CMAKE_SOURCE_DIR})


# Regex
if (UNIX)
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
endif ()


# Headers
set (qfm_HEADERS ${qfm_HEADERS} ${src_HEADERS} PARENT_SCOPE)


# Sources
set (qfm_SOURCES ${qfm_SOURCES} ${src_SOURCES} PARENT_SCOPE)


# UIs
collect_files (src_UIS "*.ui" ${CMAKE_SOURCE_DIR})
set (qfm_UIS ${qfm_UIS} ${src_UIS} PARENT_SCOPE)
