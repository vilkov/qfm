# CMake tools
include (collect_sources)


collect_sources (src_HEADERS src_SOURCES ${CMAKE_SOURCE_DIR})


# Regex
if (UNIX)
    if (DE_KDE)
        # Headers
        set (tmp_src_HEADERS)
        foreach (src_HEADER ${src_HEADERS})
            if (NOT ${src_HEADER} MATCHES ".*/gtk/.*")
                set (tmp_src_HEADERS ${tmp_src_HEADERS} ${src_HEADER})
            endif ()
        endforeach ()
        set (src_HEADERS ${tmp_src_HEADERS})

        # Sources
        set (tmp_src_SOURCES)
        foreach (src_SOURCE ${src_SOURCES})
            if (NOT ${src_SOURCE} MATCHES ".*/gtk/.*")
                set (tmp_src_SOURCES ${tmp_src_SOURCES} ${src_SOURCE})
            endif ()
        endforeach ()
        set (src_SOURCES ${tmp_src_SOURCES})
    else ()
        # Headers
        set (tmp_src_HEADERS)
        foreach (src_HEADER ${src_HEADERS})
            if (NOT ${src_HEADER} MATCHES ".*/kde/.*")
                set (tmp_src_HEADERS ${tmp_src_HEADERS} ${src_HEADER})
            endif ()
        endforeach ()
        set (src_HEADERS ${tmp_src_HEADERS})

        # Sources
        set (tmp_src_SOURCES)
        foreach (src_SOURCE ${src_SOURCES})
            if (NOT ${src_SOURCE} MATCHES ".*/kde/.*")
                set (tmp_src_SOURCES ${tmp_src_SOURCES} ${src_SOURCE})
            endif ()
        endforeach ()
        set (src_SOURCES ${tmp_src_SOURCES})
    endif ()
else ()
    # Headers
    set (tmp_src_HEADERS)
    foreach (src_HEADER ${src_HEADERS})
        if (NOT ${src_HEADER} MATCHES ".*/gtk/.*" AND NOT ${src_HEADER} MATCHES ".*/kde/.*")
            set (tmp_src_HEADERS ${tmp_src_HEADERS} ${src_HEADER})
        endif ()
    endforeach ()
    set (src_HEADERS ${tmp_src_HEADERS})

    # Sources
    set (tmp_src_SOURCES)
    foreach (src_SOURCE ${src_SOURCES})
        if (NOT ${src_SOURCE} MATCHES ".*/gtk/.*" AND NOT ${src_SOURCE} MATCHES ".*/kde/.*")
            set (tmp_src_SOURCES ${tmp_src_SOURCES} ${src_SOURCE})
        endif ()
    endforeach ()
    set (src_SOURCES ${tmp_src_SOURCES})
endif ()


# Headers
set (${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${src_HEADERS} PARENT_SCOPE)


# Sources
set (${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${src_SOURCES} PARENT_SCOPE)


# UIs
collect_files (src_UIS "*.ui" ${CMAKE_SOURCE_DIR})
set (${PROJECT_NAME}_UIS ${${PROJECT_NAME}_UIS} ${src_UIS} PARENT_SCOPE)
