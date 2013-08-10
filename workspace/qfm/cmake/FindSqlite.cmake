# - Find sqlite
# Find the Sqlite library
#
#  This module defines the following variables:
#     SQLITE_FOUND   - True if SQLITE_INCLUDE are found
#     SQLITE_INCLUDE - where to find export.h, etc.
#     SQLITE_LIBRARY - library.
#
# Copyright (C) 2012  Dmitriy Vilkov <dav.daemon@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file COPYING-CMAKE-SCRIPTS for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.

find_path (SQLITE_INCLUDE
          NAMES "sqlite3.h"
          DOC "The Sqlite include directory"
)

find_library (SQLITE_LIBRARY
          NAMES sqlite3
          DOC "The Sqlite library"
)


# handle the QUIETLY and REQUIRED arguments and set SQLITE_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Sqlite DEFAULT_MSG SQLITE_INCLUDE SQLITE_LIBRARY)

mark_as_advanced(SQLITE_INCLUDE)
