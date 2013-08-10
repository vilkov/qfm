# Copyright (C) 2012-2013  Dmitriy Vilkov <dav.daemon@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file LICENSE for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


# Sets up Compiler and Linker flags in temporary variables. 
include (compiler_flags)

# Set up BUILD_TYPE variables.
include (project_build_type)


macro (print_system_info)
    if (CMAKE_CROSSCOMPILING)
        message (STATUS "Cross compiling to: " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION})
    else ()
        message (STATUS "System: " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION})
    endif ()
    message (STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

    if (MSVC)
        message(STATUS "Compiler: MSVC, version: " ${MSVC_VERSION})
    elseif (BORLAND)
        message(STATUS "Compiler: BCC")
    elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUC)
        message(STATUS "Compiler: GCC")
    else ()
        message (FATAL_ERROR "Unknown compiler")
    endif ()

    message (STATUS "CMake generates " ${CMAKE_GENERATOR})
endmacro ()


macro (set_cxx_flags CXX_FLAGS CXX_LINK_FLAGS STRICT_WARNINGS STD11 EXCEPTIONS POSITION_INDEPENDENT_CODE STATIC_CRT RTTI COVERAGE PROFILE ABI)
    if (NOT ${ARGC} EQUAL 11)
        message (FATAL_ERROR "Wrong number of arguments passed to \"set_cxx_flags\" macros!")
    endif ()
    
    # Set up compiler flags
    set_compiler_and_linker_flags ()
    set (${CXX_FLAGS} "${CXX_BASE_FLAGS}")
    
    if (${STRICT_WARNINGS})
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_STRICT_WARNINGS_FLAGS}")
    endif ()

    if (${STD11})
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_STD11_FLAGS}")
    endif ()

    if (${EXCEPTIONS})
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_EXCEPTION_FLAGS}")
    else ()
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_NO_EXCEPTION_FLAGS}")
    endif ()

    if (${POSITION_INDEPENDENT_CODE})
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_POSITION_INDEPENDENT_CODE_FLAGS}")
    endif ()

    if (${STATIC_CRT})
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_STATIC_CRT_FLAGS}")
    else ()
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_DYNAMIC_CRT_FLAGS}")
    endif ()

    if (${RTTI})
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_RTTI_FLAGS}")
    else ()
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_NO_RTTI_FLAGS}")
    endif ()
    
    if (${COVERAGE})
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_COVERAGE_FLAGS}")
        set (${CXX_LINK_FLAGS} "${${CXX_LINK_FLAGS}} ${CXX_LINK_COVERAGE_FLAGS}")
    endif ()

    if (${PROFILE})
        set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_PROFILE_FLAGS}")
        set (${CXX_LINK_FLAGS} "${${CXX_LINK_FLAGS}} ${CXX_LINK_PROFILE_FLAGS}")
    endif ()

    if (${ABI})
        if (${ABI} STREQUAL "32")
            set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_ABI_X32_FLAGS}")
        elseif (${ABI} STREQUAL "64")
            set (${CXX_FLAGS} "${${CXX_FLAGS}} ${CXX_ABI_X32_64_FLAGS}")
        endif ()
    endif ()
endmacro ()


macro (project_cxx_header_with_abi STRICT_WARNINGS STD11 EXCEPTIONS POSITION_INDEPENDENT_CODE STATIC_CRT RTTI COVERAGE PROFILE ABI)
    set_cxx_flags (CMAKE_CXX_FLAGS
                   CMAKE_CXX_LINK_FLAGS
                   ${STRICT_WARNINGS}
                   ${STD11}
                   ${EXCEPTIONS}
                   ${POSITION_INDEPENDENT_CODE}
                   ${STATIC_CRT}
                   ${RTTI}
                   ${COVERAGE}
                   ${PROFILE}
                   "${ABI}")
endmacro ()


macro (project_cxx_header STRICT_WARNINGS STD11 EXCEPTIONS POSITION_INDEPENDENT_CODE STATIC_CRT RTTI COVERAGE PROFILE)
    project_cxx_header_with_abi (${STRICT_WARNINGS}
                                 ${STD11}
                                 ${EXCEPTIONS}
                                 ${POSITION_INDEPENDENT_CODE}
                                 ${STATIC_CRT}
                                 ${RTTI}
                                 ${COVERAGE}
                                 ${PROFILE}
                                 "")
endmacro ()


macro (project_cxx_executable_header_default)
    project_cxx_header (YES YES NO NO YES NO NO NO)
endmacro ()


macro (project_cxx_static_library_header_default)
    project_cxx_header (YES YES NO ${ENABLE_POSITION_INDEPENDENT_CODE} YES NO NO NO)
endmacro ()


macro (project_cxx_shared_library_header_default)
    project_cxx_header (YES YES NO ${ENABLE_POSITION_INDEPENDENT_CODE} NO NO NO NO)
endmacro ()


macro (project_cxx_library_header_default)
    if (BUILD_SHARED_LIBS)
        project_cxx_header (YES YES NO ${ENABLE_POSITION_INDEPENDENT_CODE} NO NO NO NO)
    else ()
        project_cxx_header (YES YES NO ${ENABLE_POSITION_INDEPENDENT_CODE} YES NO NO NO)
    endif ()
endmacro ()


macro (set_target_cxx_flags_with_abi NAME STRICT_WARNINGS STD11 EXCEPTIONS POSITION_INDEPENDENT_CODE STATIC_CRT RTTI COVERAGE PROFILE ABI)
    set_cxx_flags (_CXX_FLAGS_ _CXX_LINK_FLAGS_
                   ${STRICT_WARNINGS}
                   ${STD11}
                   ${EXCEPTIONS}
                   ${POSITION_INDEPENDENT_CODE}
                   ${STATIC_CRT}
                   ${RTTI}
                   ${COVERAGE}
                   ${PROFILE}
                   "${ABI}")
    set_target_properties (${NAME} PROPERTIES COMPILE_FLAGS "${_CXX_FLAGS_}")
    set_target_properties (${NAME} PROPERTIES LINK_FLAGS "${_CXX_LINK_FLAGS_}")
endmacro ()


macro (set_target_cxx_flags NAME STRICT_WARNINGS STD11 EXCEPTIONS POSITION_INDEPENDENT_CODE STATIC_CRT RTTI COVERAGE PROFILE)
    set_target_cxx_flags_with_abi (${NAME}
                                   ${STRICT_WARNINGS}
                                   ${STD11}
                                   ${EXCEPTIONS}
                                   ${POSITION_INDEPENDENT_CODE}
                                   ${STATIC_CRT}
                                   ${RTTI}
                                   ${COVERAGE}
                                   ${PROFILE}
                                   "")
endmacro ()
