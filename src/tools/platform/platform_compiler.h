/*
 * Copyright (C) 2012-2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
 * Copyright (C) 2011, 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef PLATFORM_COMPILER_H
#define PLATFORM_COMPILER_H

#define PLATFORM_IS_SET(macro) PLATFORM_IS_SET_(macro)
#define PLATFORM_MACROTEST_1 ,
#define PLATFORM_IS_SET_(value) PLATFORM_IS_SET__(PLATFORM_MACROTEST_##value)
#define PLATFORM_IS_SET__(comma) PLATFORM_IS_SET___(comma 1, 0, 0)
#define PLATFORM_IS_SET___(_, v, ...) v


/* PLATFORM_COMPILER() - the compiler being used to build the project */
#define PLATFORM_COMPILER(PLATFORM_FEATURE) PLATFORM_IS_SET(PLATFORM_COMPILER_##PLATFORM_FEATURE)

/* PLATFORM_COMPILER_SUPPORTS() - whether the compiler being used to build the project supports the given feature. */
#define PLATFORM_COMPILER_SUPPORTS(PLATFORM_COMPILER_FEATURE) PLATFORM_IS_SET(PLATFORM_COMPILER_SUPPORTS_##PLATFORM_COMPILER_FEATURE)

/* ==== PLATFORM_COMPILER() - the compiler being used to build the project ==== */

/* PLATFORM_COMPILER(CLANG) - Clang  */
#if defined(__clang__)
#define PLATFORM_COMPILER_CLANG 1

#ifndef __has_extension
#define __has_extension __has_feature /* Compatibility with older versions of clang */
#endif

/* Specific compiler features */
#define PLATFORM_COMPILER_SUPPORTS_CXX_VARIADIC_TEMPLATES __has_feature(cxx_variadic_templates)
#define PLATFORM_COMPILER_SUPPORTS_CXX_RVALUE_REFERENCES __has_feature(cxx_rvalue_references)
#define PLATFORM_COMPILER_SUPPORTS_CXX_DELETED_FUNCTIONS __has_feature(cxx_deleted_functions)
#define PLATFORM_COMPILER_SUPPORTS_CXX_NULLPTR __has_feature(cxx_nullptr)
#define PLATFORM_COMPILER_SUPPORTS_BLOCKS __has_feature(blocks)
#define PLATFORM_COMPILER_SUPPORTS_C_STATIC_ASSERT __has_extension(c_static_assert)

#define PLATFORM_COMPILER_SUPPORTS_HAS_TRIVIAL_DESTRUCTOR __has_extension(has_trivial_destructor)

#endif

/* PLATFORM_COMPILER(MSVC) - Microsoft Visual C++ */
/* PLATFORM_COMPILER(MSVC7_OR_LOWER) - Microsoft Visual C++ 2003 or lower*/
/* PLATFORM_COMPILER(MSVC9_OR_LOWER) - Microsoft Visual C++ 2008 or lower*/
#if defined(_MSC_VER)
#define PLATFORM_COMPILER_MSVC 1
#if _MSC_VER < 1400
#define PLATFORM_COMPILER_MSVC7_OR_LOWER 1
#elif _MSC_VER < 1600
#define PLATFORM_COMPILER_MSVC9_OR_LOWER 1
#endif

/* Specific compiler features */
#if !PLATFORM_COMPILER(CLANG) && _MSC_VER >= 1600
#define PLATFORM_COMPILER_SUPPORTS_CXX_NULLPTR 1
#endif

#endif

/* PLATFORM_COMPILER(RVCT) - ARM RealView Compilation Tools */
/* PLATFORM_COMPILER(RVCT4_OR_GREATER) - ARM RealView Compilation Tools 4.0 or greater */
#if defined(__CC_ARM) || defined(__ARMCC__)
#define PLATFORM_COMPILER_RVCT 1
#define RVCT_VERSION_AT_LEAST(major, minor, patch, build) (__ARMCC_VERSION >= (major * 100000 + minor * 10000 + patch * 1000 + build))
#else
/* Define this for !RVCT compilers, just so we can write things like RVCT_VERSION_AT_LEAST(3, 0, 0, 0). */
#define RVCT_VERSION_AT_LEAST(major, minor, patch, build) 0
#endif

/* PLATFORM_COMPILER(GCCE) - GNU Compiler Collection for Embedded */
#if defined(__GCCE__)
#define PLATFORM_COMPILER_GCCE 1
#define GCCE_VERSION (__GCCE__ * 10000 + __GCCE_MINOR__ * 100 + __GCCE_PATCHLEVEL__)
#define GCCE_VERSION_AT_LEAST(major, minor, patch) (GCCE_VERSION >= (major * 10000 + minor * 100 + patch))
#endif

/* PLATFORM_COMPILER(GCC) - GNU Compiler Collection */
/* --gnu option of the RVCT compiler also defines __GNUC__ */
#if defined(__GNUC__) && !PLATFORM_COMPILER(RVCT)
#define PLATFORM_COMPILER_GCC 1
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define GCC_VERSION_AT_LEAST(major, minor, patch) (GCC_VERSION >= (major * 10000 + minor * 100 + patch))

/* Specific compiler features */
#if !PLATFORM_COMPILER(CLANG) && GCC_VERSION_AT_LEAST(4, 6, 0) && defined(__GXX_EXPERIMENTAL_CXX0X__)
#define PLATFORM_COMPILER_SUPPORTS_CXX_NULLPTR 1
#define PLATFORM_COMPILER_SUPPORTS_CXX_STATIC_ASSERT 1
#define PLATFORM_COMPILER_SUPPORTS_MOVE_SEMANTIC 1
#endif

#else
/* Define this for !GCC compilers, just so we can write things like GCC_VERSION_AT_LEAST(4, 1, 0). */
#define GCC_VERSION_AT_LEAST(major, minor, patch) 0
#endif

/* PLATFORM_COMPILER(MINGW) - MinGW GCC */
/* PLATFORM_COMPILER(MINGW64) - mingw-w64 GCC - only used as additional check to exclude mingw.org specific functions */
#if defined(__MINGW32__)
#define PLATFORM_COMPILER_MINGW 1
#include <_mingw.h> /* private MinGW header */
    #if defined(__MINGW64_VERSION_MAJOR) /* best way to check for mingw-w64 vs mingw.org */
        #define PLATFORM_COMPILER_MINGW64 1
    #endif /* __MINGW64_VERSION_MAJOR */
#endif /* __MINGW32__ */

/* PLATFORM_COMPILER(INTEL) - Intel C++ Compiler */
#if defined(__INTEL_COMPILER)
#define PLATFORM_COMPILER_INTEL 1
#endif

/* PLATFORM_COMPILER(SUNCC) */
#if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#define PLATFORM_COMPILER_SUNCC 1
#endif

/* ==== Compiler features ==== */


/* ALWAYS_INLINE */

#ifndef ALWAYS_INLINE
#if PLATFORM_COMPILER(GCC) && defined(NDEBUG) && !PLATFORM_COMPILER(MINGW)
#define ALWAYS_INLINE inline __attribute__((__always_inline__))
#elif (PLATFORM_COMPILER(MSVC) || PLATFORM_COMPILER(RVCT)) && defined(NDEBUG)
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE inline
#endif
#endif


/* NEVER_INLINE */

#ifndef NEVER_INLINE
#if PLATFORM_COMPILER(GCC)
#define NEVER_INLINE __attribute__((__noinline__))
#elif PLATFORM_COMPILER(RVCT)
#define NEVER_INLINE __declspec(noinline)
#else
#define NEVER_INLINE
#endif
#endif


/* UNLIKELY */

#ifndef UNLIKELY
#if PLATFORM_COMPILER(GCC) || (RVCT_VERSION_AT_LEAST(3, 0, 0, 0) && defined(__GNUC__))
	/**
	 * Since you are limited to integral expressions for Expression,
	 * you should use constructions such as
	 *
	 *     if (LIKELY(ptr != NULL))
	 *         foo(*ptr);
	 *
	 * when testing pointer or floating-point values.
	 */
#define UNLIKELY(Expression) __builtin_expect((Expression), 0)
#else
#define UNLIKELY(Expression) (Expression)
#endif
#endif


/* LIKELY */

#ifndef LIKELY
#if PLATFORM_COMPILER(GCC) || (RVCT_VERSION_AT_LEAST(3, 0, 0, 0) && defined(__GNUC__))
	/**
	 * Since you are limited to integral expressions for Expression,
	 * you should use constructions such as
	 *
	 *     if (LIKELY(ptr != NULL))
	 *         foo(*ptr);
	 *
	 * when testing pointer or floating-point values.
	 */
#define LIKELY(Expression) __builtin_expect((Expression), 1)
#else
#define LIKELY(Expression) (Expression)
#endif
#endif


/* NO_RETURN */


#ifndef NO_RETURN
#if PLATFORM_COMPILER(GCC)
#define NO_RETURN __attribute((__noreturn__))
#elif PLATFORM_COMPILER(MSVC) || PLATFORM_COMPILER(RVCT)
#define NO_RETURN __declspec(noreturn)
#else
#define NO_RETURN
#endif
#endif


/* NO_RETURN_WITH_VALUE */

#ifndef NO_RETURN_WITH_VALUE
#if !PLATFORM_COMPILER(MSVC)
#define NO_RETURN_WITH_VALUE NO_RETURN
#else
#define NO_RETURN_WITH_VALUE
#endif
#endif


/* WARN_UNUSED_RETURN */

#if PLATFORM_COMPILER(GCC)
#define WARN_UNUSED_RETURN __attribute__ ((warn_unused_result))
#else
#define WARN_UNUSED_RETURN
#endif


/* OVERRIDE */

#ifndef OVERRIDE
#if PLATFORM_COMPILER(CLANG)
#if __has_extension(cxx_override_control)
#define OVERRIDE override
#endif
#elif PLATFORM_COMPILER(MSVC)
#define OVERRIDE override
#endif
#endif

#ifndef OVERRIDE
#define OVERRIDE
#endif


/* FINAL */

#ifndef FINAL
#if PLATFORM_COMPILER(CLANG)
#if __has_extension(cxx_override_control)
#define FINAL final
#endif
#elif PLATFORM_COMPILER(MSVC)
#define FINAL sealed
#endif
#endif

#ifndef FINAL
#define FINAL
#endif


/* OBJC_CLASS */

#ifndef OBJC_CLASS
#ifdef __OBJC__
#define OBJC_CLASS @class
#else
#define OBJC_CLASS class
#endif
#endif

#endif /* PLATFORM_COMPILER_H */
