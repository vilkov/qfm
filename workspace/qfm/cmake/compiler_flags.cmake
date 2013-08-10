# Copyright (C) 2013  Dmitriy Vilkov <dav.daemon@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file LICENSE for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


macro (set_compiler_and_linker_flags)
    # Base flags.
    set (C_BASE_FLAGS)
    set (CXX_BASE_FLAGS)
    # Exceptions.
    set (CXX_EXCEPTION_FLAGS)
    set (CXX_NO_EXCEPTION_FLAGS)
    # RTTI.
    set (CXX_RTTI_FLAGS)
    set (CXX_NO_RTTI_FLAGS)
    # Strict warnings.
    set (C_STRICT_WARNINGS_FLAGS)
    set (CXX_STRICT_WARNINGS_FLAGS)
    # Static/Dynamic CRT.
    set (C_STATIC_CRT_FLAGS)
    set (CXX_STATIC_CRT_FLAGS)
    set (C_DYNAMIC_CRT_FLAGS)
    set (CXX_DYNAMIC_CRT_FLAGS)
    # C\C++ 11 standard.
    set (C_STD11_FLAGS)
    set (CXX_STD11_FLAGS)
    # Position Independent Code.
    set (C_POSITION_INDEPENDENT_CODE_FLAGS)
    set (CXX_POSITION_INDEPENDENT_CODE_FLAGS)
    # Coverage.
    set (C_COVERAGE_FLAGS)
    set (C_LINK_COVERAGE_FLAGS)
    set (CXX_COVERAGE_FLAGS)
    set (CXX_LINK_COVERAGE_FLAGS)
    # Profile.
    set (C_PROFILE_FLAGS)
    set (C_LINK_PROFILE_FLAGS)
    set (CXX_PROFILE_FLAGS)
    set (CXX_LINK_PROFILE_FLAGS)
    # ABI.
    set (C_ABI_X32_FLAGS)
    set (CXX_ABI_X32_FLAGS)
    set (C_ABI_X32_64_FLAGS)
    set (CXX_ABI_X32_64_FLAGS)
	
	if (WIN32)
        # We're require at least Windows 2000 (http://msdn.microsoft.com/en-us/library/Aa383745).
        set (CXX_BASE_FLAGS "${CXX_BASE_FLAGS} -D_UNICODE -DUNICODE -DSTRICT -DWIN32_LEAN_AND_MEAN")
	endif()

    if (MSVC)

        # Base flags.
        set (CXX_BASE_FLAGS "${CXX_BASE_FLAGS} -D_WIN32_WINNT=0x0500 -DWINVER=0x0500 -GS -W4 -WX -wd4127 -wd4251 -wd4275 -wd4290 -wd4512 -wd4355   -wd4530 -wd4244 -wd4800 -wd4245 -wd4100   -nologo -J -Zi")
        # Exceptions.
        set (CXX_EXCEPTION_FLAGS "-EHsc")
        set (CXX_NO_EXCEPTION_FLAGS)
        # RTTI.
        set (CXX_RTTI_FLAGS)
        set (CXX_NO_RTTI_FLAGS "-GR-")
        # Strict warnings.
        set (C_STRICT_WARNINGS_FLAGS)
        set (CXX_STRICT_WARNINGS_FLAGS)
        # Static/Dynamic CRT.
        set (C_STATIC_CRT_FLAGS "-MT")
        set (CXX_STATIC_CRT_FLAGS "${C_STATIC_CRT_FLAGS}")
        set (C_DYNAMIC_CRT_FLAGS "-MD")
        set (CXX_DYNAMIC_CRT_FLAGS "${C_DYNAMIC_CRT_FLAGS}")
        # C\C++ 11 standard.
        set (C_STD11_FLAGS)
        set (CXX_STD11_FLAGS)
        # Position Independent Code.
        set (C_POSITION_INDEPENDENT_CODE_FLAGS)
        set (CXX_POSITION_INDEPENDENT_CODE_FLAGS)
        # Coverage.
        set (C_COVERAGE_FLAGS)
        set (C_LINK_COVERAGE_FLAGS)
        set (CXX_COVERAGE_FLAGS)
        set (CXX_LINK_COVERAGE_FLAGS)
        # Profile.
        set (C_PROFILE_FLAGS)
        set (C_LINK_PROFILE_FLAGS)
        set (CXX_PROFILE_FLAGS)
        set (CXX_LINK_PROFILE_FLAGS)
        # ABI.
        set (C_ABI_X32_FLAGS)
        set (CXX_ABI_X32_FLAGS)
        set (C_ABI_X32_64_FLAGS)
        set (CXX_ABI_X32_64_FLAGS)

    elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUC)
        # Useful link: "http://gcc.gnu.org/onlinedocs/gcc/Invoking-GCC.html".

        if (WIN32)
            # We're require at least Windows 2000 (MinGW needs at least 0x0501 version; http://msdn.microsoft.com/en-us/library/Aa383745).
            set (CXX_BASE_FLAGS "${CXX_BASE_FLAGS} -D_WIN32_WINNT=0x0501 -DWINVER=0x0501")
        endif()
        # Turn on WARNINGS.
        set (C_BASE_FLAGS   "${C_BASE_FLAGS}   -Wall -Wundef -Wcast-align -Wpointer-arith -Wformat")
        # Turn off WARNINGS.
        set (C_BASE_FLAGS   "${C_BASE_FLAGS}   -Wno-unused-parameter -Wno-parentheses -Wno-shadow -Wno-ignored-qualifiers")
        set (CXX_BASE_FLAGS "${CXX_BASE_FLAGS} -Wno-overloaded-virtual")
        # WARNINGS to ERRORS.
        set (C_BASE_FLAGS   "${C_BASE_FLAGS}   -Werror=return-type")
        set (CXX_BASE_FLAGS "${CXX_BASE_FLAGS} -Werror=non-virtual-dtor")
        # Turn on CODE GEN.
        set (C_BASE_FLAGS   "${C_BASE_FLAGS}   -fvisibility=hidden -fvisibility-inlines-hidden")
        # Turn off CODE GEN.
        set (C_BASE_FLAGS   "${C_BASE_FLAGS}   -fno-common -fno-threadsafe-statics")
        set (CXX_BASE_FLAGS "${CXX_BASE_FLAGS} -fno-check-new")
        # Prepend C_BASE_FLAGS to CXX_BASE_FLAGS
        set (CXX_BASE_FLAGS "${C_BASE_FLAGS} ${CXX_BASE_FLAGS}")
        # Exceptions.
        set (CXX_EXCEPTION_FLAGS    "-fexceptions")
        set (CXX_NO_EXCEPTION_FLAGS "-fno-exceptions")
        # RTTI. Until version 4.3.2, GCC doesn't define a macro to indicate whether RTTI is enabled.
        set (CXX_RTTI_FLAGS    "-frtti")
        set (CXX_NO_RTTI_FLAGS "-fno-rtti")
        # Strict warnings.
        set (C_STRICT_WARNINGS_FLAGS   "-Wextra -pedantic")
        set (CXX_STRICT_WARNINGS_FLAGS "${C_STRICT_WARNINGS_FLAGS}")
        # Static/Dynamic CRT.
        set (C_STATIC_CRT_FLAGS "-static-libgcc")
        set (CXX_STATIC_CRT_FLAGS "${C_STATIC_CRT_FLAGS} -static-libstdc++")
        set (C_DYNAMIC_CRT_FLAGS "-shared-libgcc")
        set (CXX_DYNAMIC_CRT_FLAGS "${C_DYNAMIC_CRT_FLAGS} -shared-libstdc++")
        # C\C++ 11 standard.
        if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.7.1")
            set (C_STD11_FLAGS "-std=gnu11")
            set (CXX_STD11_FLAGS "-std=gnu++11")
        else ()
            set (C_STD11_FLAGS "-std=gnu0x")
            set (CXX_STD11_FLAGS "-std=gnu++0x")
        endif ()
        # Position Independent Code.
        set (C_POSITION_INDEPENDENT_CODE_FLAGS   "-fPIC")
        set (CXX_POSITION_INDEPENDENT_CODE_FLAGS "${C_POSITION_INDEPENDENT_CODE_FLAGS}")
        # Coverage. GCov.
        set (C_COVERAGE_FLAGS        "-fprofile-arcs -ftest-coverage")
        set (C_LINK_COVERAGE_FLAGS   "${C_COVERAGE_FLAGS}")
        set (CXX_COVERAGE_FLAGS      "${C_COVERAGE_FLAGS}")
        set (CXX_LINK_COVERAGE_FLAGS "${C_COVERAGE_FLAGS}")
        # Profile. GProf.
        set (C_PROFILE_FLAGS         "-pg")
        set (C_LINK_PROFILE_FLAGS    "-pg")
        set (CXX_PROFILE_FLAGS       "${C_PROFILE_FLAGS}")
        set (CXX_LINK_PROFILE_FLAGS  "${C_LINK_PROFILE_FLAGS}")
        # ABI.
        set (C_ABI_X32_FLAGS "-m32")
        set (CXX_ABI_X32_FLAGS "${C_ABI_X32_FLAGS}")
        set (C_ABI_X32_64_FLAGS "-m64")
        set (CXX_ABI_X32_64_FLAGS "${C_ABI_X32_64_FLAGS}")

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "SunPro")
  
        # Base flags.
        set (CXX_BASE_FLAGS "${CMAKE_CXX_FLAGS}")
        # Exceptions. Sun Pro doesn't provide macros to indicate whether exceptions are enabled.
        set (CXX_EXCEPTION_FLAGS    "-features=except -DPLATFORM_COMPILER_SUPPORTS_EXCEPTIONS=1")
        set (CXX_NO_EXCEPTION_FLAGS "-features=no%except -DPLATFORM_COMPILER_SUPPORTS_EXCEPTIONS=0")
        # RTTI. Sun Pro doesn't provide macros to indicate whether RTTI is enabled.
        set (CXX_RTTI_FLAGS    "-features=rtti")
        set (CXX_NO_RTTI_FLAGS "-features=no%rtti")
        # Strict warnings.
        set (C_STRICT_WARNINGS_FLAGS)
        set (CXX_STRICT_WARNINGS_FLAGS)
        # Static/Dynamic CRT.
        set (C_STATIC_CRT_FLAGS)
        set (CXX_STATIC_CRT_FLAGS)
        set (C_DYNAMIC_CRT_FLAGS)
        set (CXX_DYNAMIC_CRT_FLAGS)
        # C\C++ 11 standard.
        set (C_STD11_FLAGS)
        set (CXX_STD11_FLAGS)
        # Position Independent Code.
        set (C_POSITION_INDEPENDENT_CODE_FLAGS)
        set (CXX_POSITION_INDEPENDENT_CODE_FLAGS)
        # Coverage.
        set (C_COVERAGE_FLAGS)
        set (C_LINK_COVERAGE_FLAGS)
        set (CXX_COVERAGE_FLAGS)
        set (CXX_LINK_COVERAGE_FLAGS)
        # Profile.
        set (C_PROFILE_FLAGS)
        set (C_LINK_PROFILE_FLAGS)
        set (CXX_PROFILE_FLAGS)
        set (CXX_LINK_PROFILE_FLAGS)
        # ABI.
        set (C_ABI_X32_FLAGS)
        set (CXX_ABI_X32_FLAGS)
        set (C_ABI_X32_64_FLAGS)
        set (CXX_ABI_X32_64_FLAGS)

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "VisualAge" OR CMAKE_CXX_COMPILER_ID STREQUAL "XL")
        # CMake 2.8 changes Visual Age's compiler ID to "XL".

        # Base flags.
        set (CXX_BASE_FLAGS "${CMAKE_CXX_FLAGS}")
        # Exceptions.
        set (CXX_EXCEPTION_FLAGS    "-qeh")
        set (CXX_NO_EXCEPTION_FLAGS "-qnoeh")
        # RTTI. Until version 9.0, Visual Age doesn't define a macro to indicate whether RTTI is enabled.
        set (CXX_RTTI_FLAGS    "-qrtti")
        set (CXX_NO_RTTI_FLAGS "-qnortti")
        # Strict warnings.
        set (C_STRICT_WARNINGS_FLAGS)
        set (CXX_STRICT_WARNINGS_FLAGS)
        # Static/Dynamic CRT.
        set (C_STATIC_CRT_FLAGS)
        set (CXX_STATIC_CRT_FLAGS)
        set (C_DYNAMIC_CRT_FLAGS)
        set (CXX_DYNAMIC_CRT_FLAGS)
        # C\C++ 11 standard.
        set (C_STD11_FLAGS)
        set (CXX_STD11_FLAGS)
        # Position Independent Code.
        set (C_POSITION_INDEPENDENT_CODE_FLAGS)
        set (CXX_POSITION_INDEPENDENT_CODE_FLAGS)
        # Coverage.
        set (C_COVERAGE_FLAGS)
        set (C_LINK_COVERAGE_FLAGS)
        set (CXX_COVERAGE_FLAGS)
        set (CXX_LINK_COVERAGE_FLAGS)
        # Profile.
        set (C_PROFILE_FLAGS)
        set (C_LINK_PROFILE_FLAGS)
        set (CXX_PROFILE_FLAGS)
        set (CXX_LINK_PROFILE_FLAGS)
        # ABI.
        set (C_ABI_X32_FLAGS)
        set (CXX_ABI_X32_FLAGS)
        set (C_ABI_X32_64_FLAGS)
        set (CXX_ABI_X32_64_FLAGS)

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "HP")

        # Base flags.
        set (CXX_BASE_FLAGS "-AA -mt")
        # Exceptions.
        set (CXX_EXCEPTION_FLAGS    "-DPLATFORM_COMPILER_SUPPORTS_EXCEPTIONS=1")
        set (CXX_NO_EXCEPTION_FLAGS "+noeh -DPLATFORM_COMPILER_SUPPORTS_EXCEPTIONS=0")
        # RTTI. Can not be disabled in HP aCC compiler.
        set (CXX_RTTI_FLAGS)
        set (CXX_NO_RTTI_FLAGS)
        # Strict warnings.
        set (C_STRICT_WARNINGS_FLAGS)
        set (CXX_STRICT_WARNINGS_FLAGS)
        # Static/Dynamic CRT.
        set (C_STATIC_CRT_FLAGS)
        set (CXX_STATIC_CRT_FLAGS)
        set (C_DYNAMIC_CRT_FLAGS)
        set (CXX_DYNAMIC_CRT_FLAGS)
        # C\C++ 11 standard.
        set (C_STD11_FLAGS)
        set (CXX_STD11_FLAGS)
        # Position Independent Code.
        set (C_POSITION_INDEPENDENT_CODE_FLAGS)
        set (CXX_POSITION_INDEPENDENT_CODE_FLAGS)
        # Coverage.
        set (C_COVERAGE_FLAGS)
        set (C_LINK_COVERAGE_FLAGS)
        set (CXX_COVERAGE_FLAGS)
        set (CXX_LINK_COVERAGE_FLAGS)
        # Profile.
        set (C_PROFILE_FLAGS)
        set (C_LINK_PROFILE_FLAGS)
        set (CXX_PROFILE_FLAGS)
        set (CXX_LINK_PROFILE_FLAGS)
        # ABI.
        set (C_ABI_X32_FLAGS)
        set (CXX_ABI_X32_FLAGS)
        set (C_ABI_X32_64_FLAGS)
        set (CXX_ABI_X32_64_FLAGS)

    endif ()
endmacro ()
