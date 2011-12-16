# Find KDE4 core library.
#
#  This module defines the following variables:
#     KDE4CORE_FOUND   - True if KDE4CORE_INCLUDE were found.
#     KDE4CORE_INCLUDE - Where to find includes.
#     KDE4CORE_LIB     - Binary library.
#
# Author: Dmitriy Vilkov <dav.daemon@gmail.com>

#=============================================================================
# Copyright 2011 Dmitriy Vilkov <dav.daemon@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_path (KDE4CORE_INCLUDE
          NAMES "kdecore_export.h"
          DOC "The KDE4 Core include directory"
)

find_library (KDE4CORE_LIB
          NAMES kdecore
          DOC "The KDE4 Core library"
)

# handle the QUIETLY and REQUIRED arguments and set KDE4CORE_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(KDE4Core DEFAULT_MSG KDE4CORE_INCLUDE KDE4CORE_LIB)

mark_as_advanced(KDE4CORE_INCLUDE KDE4CORE_LIB)
