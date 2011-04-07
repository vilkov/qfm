# - Find QtSingleInstance
# Find the QtSingleInstance library
#
#  This module defines the following variables:
#     QTSINGLEINSTANCE_FOUND   - True if QTSINGLEINSTANCE_INCLUDE & QTSINGLEINSTANCE_LIBRARY are found
#     QTSINGLEINSTANCE_INCLUDE - where to find export.h, etc.
#     QTSINGLEINSTANCE_LIBRARY - the QtSingleInstance library
#

find_path (QTSINGLEINSTANCE_INCLUDE
          NAMES "export.h"
          PATH_SUFFIXES "QtSingleInstance"
          DOC "The QtSingleInstance include directory"
)
string (REGEX REPLACE "/QtSingleInstance$" "" QTSINGLEINSTANCE_INCLUDE ${QTSINGLEINSTANCE_INCLUDE})

find_library (QTSINGLEINSTANCE_LIBRARY
          NAMES QtSingleInstance
          DOC "The QtSingleInstance library"
)

# handle the QUIETLY and REQUIRED arguments and set QTSINGLEINSTANCE_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QtSingleInstance DEFAULT_MSG QTSINGLEINSTANCE_LIBRARY QTSINGLEINSTANCE_INCLUDE)

mark_as_advanced(QTSINGLEINSTANCE_INCLUDE QTSINGLEINSTANCE_LIBRARY)
