# Copyright (C) 2013  Dmitriy Vilkov <dav.daemon@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file LICENSE for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


# Main tests target
macro (add_project_tests_main_target)
    find_program (LCOV_EXECUTABLE lcov)
    find_program (GENHTML_EXECUTABLE genhtml)
    file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
    include_directories (${CMAKE_BINARY_DIR}/tests)
    
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_SYMBIAN 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_LINUX 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_LINUX_ANDROID 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_MAC 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_CYGWIN 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_SOLARIS 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_WINDOWS 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_WINDOWS_MOBILE 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_WINDOWS_DESKTOP 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_OS_NACL 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_DONT_DEFINE_FAIL 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_DONT_DEFINE_SUCCEED 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_DONT_DEFINE_ASSERT_EQ 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_DONT_DEFINE_ASSERT_NE 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_DONT_DEFINE_ASSERT_LE 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_DONT_DEFINE_ASSERT_LT 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_DONT_DEFINE_ASSERT_GE 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_DONT_DEFINE_ASSERT_GT 0")
    set (SUPPRESS_GTEST_WARNINGS "${SUPPRESS_GTEST_WARNINGS}\n#define GTEST_DONT_DEFINE_TEST 0")
    file (WRITE ${CMAKE_BINARY_DIR}/tests/tests.h "${SUPPRESS_GTEST_WARNINGS}\n\n#include <gtest/gtest.h>\n")
    
    add_custom_target (tests DEPENDS ${CMAKE_BINARY_DIR}/tests/tests.h)
    add_custom_target (coverage DEPENDS tests)
endmacro ()


macro (add_tests NAME SOURCES TESTS_SOURCES DEPENDENCIES)
    set (COVERAGE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/${PROJECT_NAME}/coverage)
    file (MAKE_DIRECTORY ${COVERAGE_OUTPUT_DIRECTORY})
    file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)
    file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/tests/tests.cpp "#include <tests.h>\n\nint main(int argc, char **argv)\n{\n\t::testing::InitGoogleTest(&argc, argv);\n\treturn RUN_ALL_TESTS();\n}\n")

    add_executable (${NAME}_tests ${CMAKE_CURRENT_BINARY_DIR}/tests/tests.cpp ${${SOURCES}} ${${TESTS_SOURCES}})
    set_target_cxx_flags (${NAME}_tests YES YES YES NO YES NO YES NO)
    target_link_libraries (${NAME}_tests gtest ${${DEPENDENCIES}})
    
    add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests_base.info
                        COMMAND ${LCOV_EXECUTABLE} -q -c -i -d . -o ${NAME}_tests_base.info
                        DEPENDS ${NAME}_tests
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_custom_target (${NAME}_tests_perform
                       COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests
                       DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests_base.info
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests.info
                        COMMAND ${LCOV_EXECUTABLE} -q -c -d . -o ${NAME}_tests.info
                        DEPENDS ${NAME}_tests_perform
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    
    add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests_total.info
                        COMMAND ${LCOV_EXECUTABLE} -q -a ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests_base.info -a ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests.info -o ${NAME}_tests_total.info
                        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests.info
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    
    add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests_total_clean.info
                        COMMAND ${LCOV_EXECUTABLE} -q -e ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests_total.info "*${CMAKE_CURRENT_SOURCE_DIR}*" -o ${NAME}_tests_total_clean.info
                        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests_total.info
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_custom_target (${NAME}_tests_coverage
                       COMMAND ${GENHTML_EXECUTABLE} --demangle-cpp --legend --no-branch-coverage -q -t "${NAME} unit tests" ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests_total_clean.info
                       DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_tests_total_clean.info
                       WORKING_DIRECTORY ${COVERAGE_OUTPUT_DIRECTORY})

    add_dependencies (tests ${NAME}_tests)
    add_dependencies (coverage ${NAME}_tests_coverage)
endmacro ()
