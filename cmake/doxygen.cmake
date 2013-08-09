# Copyright (C) 2013  Dmitriy Vilkov <dav.daemon@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file LICENSE for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


# Main doc target
macro (add_project_documentation_main_target)
    find_package (Doxygen REQUIRED)
    add_custom_target (doc)
endmacro ()


macro (add_documentation_ex NAME VERSION_MAJOR VERSION_MINOR VERSION_RELEASE VERSION_BUILD BRIEF_DESCRIPTION)
    set (DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR})
    set (DOXYGEN_PROJECT_NAME   ${NAME})
    set (DOXYGEN_PROJECT_NUMBER ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}.${VERSION_BUILD})
    set (DOXYGEN_PROJECT_BRIEF  ${BRIEF_DESCRIPTION})
    set (DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/${PROJECT_NAME}/doxygen)
    set (DOXYGEN_HTML_EXTRA_FILES)
    file (MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})
    find_file (DOXYGEN_CONF_IN "doxygen.conf.in"
               PATHS ${PROJECT_SOURCE_DIR}/../cmake/platform ${PROJECT_SOURCE_DIR}/../../cmake/platform
               NO_DEFAULT_PATH
               NO_CMAKE_ENVIRONMENT_PATH
               NO_CMAKE_PATH
               NO_SYSTEM_ENVIRONMENT_PATH
               NO_CMAKE_SYSTEM_PATH
	       NO_CMAKE_FIND_ROOT_PATH)
    if (${DOXYGEN_CONF_IN} STREQUAL "DOXYGEN_CONF_IN-NOTFOUND")
        message (FATAL_ERROR "File \"doxygen.conf.in\" not found!")
    endif ()
    configure_file (${DOXYGEN_CONF_IN} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf @ONLY)
    add_custom_target (doc_${PROJECT_NAME}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_dependencies (doc doc_${PROJECT_NAME})
endmacro ()


macro (add_documentation NAME VERSION BRIEF_DESCRIPTION)
    if (${VERSION} MATCHES "^([0-9]+).([0-9]+).([0-9]+)$")
        string (REGEX REPLACE "^([0-9]+).[0-9]+.[0-9]+$" "\\1" VERSION_MAJOR "${VERSION}")
        string (REGEX REPLACE "^[0-9]+.([0-9]+).[0-9]+$" "\\1" VERSION_MINOR "${VERSION}")
        string (REGEX REPLACE "^[0-9]+.[0-9]+.([0-9]+)$" "\\1" VERSION_RELEASE "${VERSION}")
    else ()
        message (FATAL_ERROR "Incorrect format of VERSION argument!")
    endif ()

    if ($ENV{BUILD_NUMBER})
        if ($ENV{BUILD_NUMBER} MATCHES "^[0-9]+$")
            add_documentation_ex (${NAME} ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_RELEASE} $ENV{BUILD_NUMBER} ${BRIEF_DESCRIPTION})
        else ()
            message (FATAL_ERROR "Incorrect format of BUILD_NUMBER environment variable!")
        endif ()
    else ()
        add_documentation_ex (${NAME} ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_RELEASE} 0 ${BRIEF_DESCRIPTION})
    endif ()
endmacro ()


macro (add_documentation_with_extra_files_ex NAME VERSION_MAJOR VERSION_MINOR VERSION_RELEASE VERSION_BUILD BRIEF_DESCRIPTION FILES_LIST)
    set (DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR})
    set (DOXYGEN_PROJECT_NAME   ${NAME})
    set (DOXYGEN_PROJECT_NUMBER ${VERSION})
    set (DOXYGEN_PROJECT_BRIEF  ${BRIEF_DESCRIPTION})
    set (DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/${PROJECT_NAME}/doxygen)
    set (DOXYGEN_HTML_EXTRA_FILES)
    foreach (f ${${FILES_LIST}})
        set (DOXYGEN_HTML_EXTRA_FILES "${DOXYGEN_HTML_EXTRA_FILES} \"${CMAKE_CURRENT_SOURCE_DIR}/${f}\"")
    endforeach ()
    file (MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})
    find_file (DOXYGEN_CONF_IN "doxygen.conf.in"
               PATHS ${PROJECT_SOURCE_DIR}/../cmake/platform ${PROJECT_SOURCE_DIR}/../../cmake/platform
               NO_DEFAULT_PATH
               NO_CMAKE_ENVIRONMENT_PATH
               NO_CMAKE_PATH
               NO_SYSTEM_ENVIRONMENT_PATH
               NO_CMAKE_SYSTEM_PATH
	       NO_CMAKE_FIND_ROOT_PATH)
    if (${DOXYGEN_CONF_IN} STREQUAL "DOXYGEN_CONF_IN-NOTFOUND")
        message (FATAL_ERROR "File \"doxygen.conf.in\" not found!")
    endif ()
    configure_file (${DOXYGEN_CONF_IN} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf @ONLY)
    add_custom_target (doc_${PROJECT_NAME}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_dependencies (doc doc_${PROJECT_NAME})
endmacro ()


macro (add_documentation_with_extra_files NAME VERSION BRIEF_DESCRIPTION FILES_LIST)
    if (${VERSION} MATCHES "^([0-9]+).([0-9]+).([0-9]+)$")
        string (REGEX REPLACE "^([0-9]+).[0-9]+.[0-9]+$" "\\1" VERSION_MAJOR "${VERSION}")
        string (REGEX REPLACE "^[0-9]+.([0-9]+).[0-9]+$" "\\1" VERSION_MINOR "${VERSION}")
        string (REGEX REPLACE "^[0-9]+.[0-9]+.([0-9]+)$" "\\1" VERSION_RELEASE "${VERSION}")
    else ()
        message (FATAL_ERROR "Incorrect format of VERSION argument!")
    endif ()

    if ($ENV{BUILD_NUMBER})
        if ($ENV{BUILD_NUMBER} MATCHES "^[0-9]+$")
            add_documentation_with_extra_files_ex (${NAME} ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_RELEASE} $ENV{BUILD_NUMBER} ${BRIEF_DESCRIPTION} ${FILES_LIST})
        else ()
            message (FATAL_ERROR "Incorrect format of BUILD_NUMBER environment variable!")
        endif ()
    else ()
        add_documentation_with_extra_files_ex (${NAME} ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_RELEASE} 0 ${BRIEF_DESCRIPTION} ${FILES_LIST})
    endif ()
endmacro ()
