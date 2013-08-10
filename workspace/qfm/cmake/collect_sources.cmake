# Copyright (C) 2012  Dmitriy Vilkov <dav.daemon@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file LICENSE for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


# CMake tools
include (collect_files)
include (exclude_files)


function (collect_sources THIS_HEADERS THIS_SOURCES RELATIVE_DIR)
    # "THIS_HEADERS", "THIS_SOURCES" and "RELATIVE_DIR" contains names of the actual variables passed into here.
    
    # Headers
    collect_files (src_HEADERS "*.h"   ${RELATIVE_DIR})
    collect_files (src_HEADERS "*.hh"  ${RELATIVE_DIR})
    collect_files (src_HEADERS "*.hpp" ${RELATIVE_DIR})
    collect_files (src_HEADERS "*.hxx" ${RELATIVE_DIR})
    set (${THIS_HEADERS} ${${THIS_HEADERS}} ${src_HEADERS} PARENT_SCOPE)
    
    
    # Sources
    collect_files (src_SOURCES "*.c"   ${RELATIVE_DIR})
    collect_files (src_SOURCES "*.cpp" ${RELATIVE_DIR})
    collect_files (src_SOURCES "*.cxx" ${RELATIVE_DIR})
    set (${THIS_SOURCES} ${${THIS_SOURCES}} ${src_SOURCES} PARENT_SCOPE)
    
endfunction (collect_sources)
