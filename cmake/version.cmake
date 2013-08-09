# Copyright (C) 2012-2013  Dmitriy Vilkov <dav.daemon@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file COPYING-CMAKE-SCRIPTS for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.

include_directories (${CMAKE_BINARY_DIR})

macro (build_version_file_ex NAME VERSION_MAJOR VERSION_MINOR VERSION_RELEASE VERSION_BUILD)
    string (TOUPPER ${NAME} UPPER_NAME)
    string (TOLOWER ${NAME} LOWER_NAME)
    set (VERSION_FILE "${VERSION_FILE}#ifndef ${UPPER_NAME}_VERSION_H_")
    set (VERSION_FILE "${VERSION_FILE}\n#define ${UPPER_NAME}_VERSION_H_\n")
    set (VERSION_FILE "${VERSION_FILE}\n#define ${UPPER_NAME}_VERSION_MAJOR   @VERSION_MAJOR@")
    set (VERSION_FILE "${VERSION_FILE}\n#define ${UPPER_NAME}_VERSION_MINOR   @VERSION_MINOR@")
    set (VERSION_FILE "${VERSION_FILE}\n#define ${UPPER_NAME}_VERSION_RELEASE @VERSION_RELEASE@")
    set (VERSION_FILE "${VERSION_FILE}\n#define ${UPPER_NAME}_VERSION_BUILD   @VERSION_BUILD@")
    set (VERSION_FILE "${VERSION_FILE}\n#define ${UPPER_NAME}_VERSION_STRING  \"@VERSION_MAJOR@.@VERSION_MINOR@.@VERSION_RELEASE@.@VERSION_BUILD@\"")
    set (VERSION_FILE "${VERSION_FILE}\n\n#endif /* ${UPPER_NAME}_VERSION_H_ */")
    file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_NAME}_version.h.in "${VERSION_FILE}\n")
    
    set (VERSION_MAJOR ${VERSION_MAJOR})
    set (VERSION_MINOR ${VERSION_MINOR})
    set (VERSION_RELEASE ${VERSION_RELEASE})
    set (VERSION_BUILD ${VERSION_BUILD})
    
    configure_file (${CMAKE_CURRENT_BINARY_DIR}/${LOWER_NAME}_version.h.in ${CMAKE_BINARY_DIR}/${LOWER_NAME}_version.h @ONLY)
    add_dependencies (${NAME} ${CMAKE_BINARY_DIR}/${LOWER_NAME}_version.h)
endmacro ()

macro (build_version_file NAME VERSION)
    if (${VERSION} MATCHES "^([0-9]+).([0-9]+).([0-9]+)$")
        string (REGEX REPLACE "^([0-9]+).[0-9]+.[0-9]+$" "\\1" VERSION_MAJOR "${VERSION}")
        string (REGEX REPLACE "^[0-9]+.([0-9]+).[0-9]+$" "\\1" VERSION_MINOR "${VERSION}")
        string (REGEX REPLACE "^[0-9]+.[0-9]+.([0-9]+)$" "\\1" VERSION_RELEASE "${VERSION}")
    else ()
        message (FATAL_ERROR "Incorrect format of VERSION argument!")
    endif ()

    if ($ENV{BUILD_NUMBER})
        if ($ENV{BUILD_NUMBER} MATCHES "^[0-9]+$")
            build_version_file_ex (${NAME} ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_RELEASE} $ENV{BUILD_NUMBER})
        else ()
            message (FATAL_ERROR "Incorrect format of BUILD_NUMBER environment variable!")
        endif ()
    else ()
        build_version_file_ex (${NAME} ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_RELEASE} 0)
    endif ()
endmacro ()
