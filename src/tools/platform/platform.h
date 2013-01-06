/*
 * Copyright (C) 2012-2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
 * Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2007-2009 Torch Mobile, Inc.
 * Copyright (C) 2010, 2011 Research In Motion Limited. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PLATFORM_H
#define PLATFORM_H

/* Include compiler specific macros */
#include "platform_compiler.h"


/* ==== Platform adaptation macros: these describe properties of the target environment. ==== */

/* PLATFORM_CPU() - the target CPU architecture */
#define PLATFORM_CPU(PLATFORM_FEATURE) PLATFORM_IS_SET(PLATFORM_CPU_##PLATFORM_FEATURE)
/* PLATFORM_HAVE() - specific system features (headers, functions or similar) that are present or not */
#define PLATFORM_HAVE(PLATFORM_FEATURE) PLATFORM_IS_SET(HAVE_##PLATFORM_FEATURE)
/* PLATFORM_OS() - underlying operating system */
#define PLATFORM_OS(PLATFORM_FEATURE) PLATFORM_IS_SET(PLATFORM_OS_##PLATFORM_FEATURE)


/* ==== Policy decision macros: these define policy choices for a particular port. ==== */

/* PLATFORM_USE() - use a particular third-party library or optional OS service */
#define PLATFORM_USE(PLATFORM_FEATURE) (defined PLATFORM_USE_##PLATFORM_FEATURE  && PLATFORM_USE_##PLATFORM_FEATURE)
/* PLATFORM_ENABLE() - turn on a specific feature of WebKit */
#define PLATFORM_ENABLE(PLATFORM_FEATURE) (defined PLATFORM_ENABLE_##PLATFORM_FEATURE  && PLATFORM_ENABLE_##PLATFORM_FEATURE)


/* ==== PLATFORM_CPU() - the target CPU architecture ==== */

/* This also defines PLATFORM_CPU(BIG_ENDIAN) or PLATFORM_CPU(MIDDLE_ENDIAN) or neither, as appropriate. */

/* PLATFORM_CPU(ALPHA) - DEC Alpha */
#if defined(__alpha__)
#define PLATFORM_CPU_ALPHA 1
#endif

/* PLATFORM_CPU(IA64) - Itanium / IA-64 */
#if defined(__ia64__)
#define PLATFORM_CPU_IA64 1
/* 32-bit mode on Itanium */
#if !defined(__LP64__)
#define PLATFORM_CPU_IA64_32 1
#endif
#endif

/* PLATFORM_CPU(MIPS) - MIPS 32-bit */
/* Note: Only O32 ABI is tested, so we enable it for O32 ABI for now.  */
#if (defined(mips) || defined(__mips__) || defined(MIPS) || defined(_MIPS_)) \
    && defined(_ABIO32)
#define PLATFORM_CPU_MIPS 1
#if defined(__MIPSEB__)
#define PLATFORM_CPU_BIG_ENDIAN 1
#endif
#define PLATFORM_MIPS_PIC (defined __PIC__)
#define PLATFORM_MIPS_ARCH __mips
#define PLATFORM_MIPS_ISA(v) (defined PLATFORM_MIPS_ARCH && PLATFORM_MIPS_ARCH == v)
#define PLATFORM_MIPS_ISA_AT_LEAST(v) (defined PLATFORM_MIPS_ARCH && PLATFORM_MIPS_ARCH >= v)
#define PLATFORM_MIPS_ARCH_REV __mips_isa_rev
#define PLATFORM_MIPS_ISA_REV(v) (defined PLATFORM_MIPS_ARCH_REV && PLATFORM_MIPS_ARCH_REV == v)
#define PLATFORM_MIPS_DOUBLE_FLOAT (defined __mips_hard_float && !defined __mips_single_float)
#define PLATFORM_MIPS_FP64 (defined __mips_fpr && __mips_fpr == 64)
/* MIPS requires allocators to use aligned memory */
#define PLATFORM_USE_ARENA_ALLOC_ALIGNMENT_INTEGER 1
#endif /* MIPS */

/* PLATFORM_CPU(PPC) - PowerPC 32-bit */
#if   defined(__ppc__)     \
    || defined(__PPC__)     \
    || defined(__powerpc__) \
    || defined(__powerpc)   \
    || defined(__POWERPC__) \
    || defined(_M_PPC)      \
    || defined(__PPC)
#define PLATFORM_CPU_PPC 1
#define PLATFORM_CPU_BIG_ENDIAN 1
#endif

/* PLATFORM_CPU(PPC64) - PowerPC 64-bit */
#if   defined(__ppc64__) \
    || defined(__PPC64__)
#define PLATFORM_CPU_PPC64 1
#define PLATFORM_CPU_BIG_ENDIAN 1
#endif

/* PLATFORM_CPU(SH4) - SuperH SH-4 */
#if defined(__SH4__)
#define PLATFORM_CPU_SH4 1
#endif

/* PLATFORM_CPU(SPARC32) - SPARC 32-bit */
#if defined(__sparc) && !defined(__arch64__) || defined(__sparcv8)
#define PLATFORM_CPU_SPARC32 1
#define PLATFORM_CPU_BIG_ENDIAN 1
#endif

/* PLATFORM_CPU(SPARC64) - SPARC 64-bit */
#if defined(__sparc__) && defined(__arch64__) || defined (__sparcv9)
#define PLATFORM_CPU_SPARC64 1
#define PLATFORM_CPU_BIG_ENDIAN 1
#endif

/* PLATFORM_CPU(SPARC) - any SPARC, true for PLATFORM_CPU(SPARC32) and PLATFORM_CPU(SPARC64) */
#if PLATFORM_CPU(SPARC32) || PLATFORM_CPU(SPARC64)
#define PLATFORM_CPU_SPARC 1
#endif

/* PLATFORM_CPU(S390X) - S390 64-bit */
#if defined(__s390x__)
#define PLATFORM_CPU_S390X 1
#define PLATFORM_CPU_BIG_ENDIAN 1
#endif

/* PLATFORM_CPU(S390) - S390 32-bit */
#if defined(__s390__)
#define PLATFORM_CPU_S390 1
#define PLATFORM_CPU_BIG_ENDIAN 1
#endif

/* PLATFORM_CPU(X86) - i386 / x86 32-bit */
#if   defined(__i386__) \
    || defined(i386)     \
    || defined(_M_IX86)  \
    || defined(_X86_)    \
    || defined(__THW_INTEL)
#define PLATFORM_CPU_X86 1
#endif

/* PLATFORM_CPU(X86_64) - AMD64 / Intel64 / x86_64 64-bit */
#if   defined(__x86_64__) \
    || defined(_M_X64)
#define PLATFORM_CPU_X86_64 1
#endif

/* PLATFORM_CPU(ARM) - ARM, any version*/
#if   defined(arm) \
    || defined(__arm__) \
    || defined(ARM) \
    || defined(_ARM_)
#define PLATFORM_CPU_ARM 1

#if defined(__ARMEB__) || (COMPILER(RVCT) && defined(__BIG_ENDIAN))
#define PLATFORM_CPU_BIG_ENDIAN 1

#elif !defined(__ARM_EABI__) \
    && !defined(__EABI__) \
    && !defined(__VFP_FP__) \
    && !defined(_WIN32_WCE) \
    && !defined(ANDROID)
#define PLATFORM_CPU_MIDDLE_ENDIAN 1

#endif

#define PLATFORM_ARM_ARCH_AT_LEAST(N) (PLATFORM_CPU(ARM) && PLATFORM_ARM_ARCH_VERSION >= N)

/* Set PLATFORM_ARM_ARCH_VERSION */
#if   defined(__ARM_ARCH_4__) \
    || defined(__ARM_ARCH_4T__) \
    || defined(__MARM_ARMV4__) \
    || defined(_ARMV4I_)
#define PLATFORM_ARM_ARCH_VERSION 4

#elif defined(__ARM_ARCH_5__) \
    || defined(__ARM_ARCH_5T__) \
    || defined(__MARM_ARMV5__)
#define PLATFORM_ARM_ARCH_VERSION 5

#elif defined(__ARM_ARCH_5E__) \
    || defined(__ARM_ARCH_5TE__) \
    || defined(__ARM_ARCH_5TEJ__)
#define PLATFORM_ARM_ARCH_VERSION 5
/*ARMv5TE requires allocators to use aligned memory*/
#define PLATFORM_USE_ARENA_ALLOC_ALIGNMENT_INTEGER 1

#elif defined(__ARM_ARCH_6__) \
    || defined(__ARM_ARCH_6J__) \
    || defined(__ARM_ARCH_6K__) \
    || defined(__ARM_ARCH_6Z__) \
    || defined(__ARM_ARCH_6ZK__) \
    || defined(__ARM_ARCH_6T2__) \
    || defined(__ARMV6__)
#define PLATFORM_ARM_ARCH_VERSION 6

#elif defined(__ARM_ARCH_7A__) \
    || defined(__ARM_ARCH_7R__)
#define PLATFORM_ARM_ARCH_VERSION 7

/* RVCT sets _TARGET_ARCH_ARM */
#elif defined(__TARGET_ARCH_ARM)
#define PLATFORM_ARM_ARCH_VERSION __TARGET_ARCH_ARM

#if defined(__TARGET_ARCH_5E) \
    || defined(__TARGET_ARCH_5TE) \
    || defined(__TARGET_ARCH_5TEJ)
/*ARMv5TE requires allocators to use aligned memory*/
#define PLATFORM_USE_ARENA_ALLOC_ALIGNMENT_INTEGER 1
#endif

#else
#define PLATFORM_ARM_ARCH_VERSION 0

#endif

/* Set PLATFORM_THUMB_ARCH_VERSION */
#if   defined(__ARM_ARCH_4T__)
#define PLATFORM_THUMB_ARCH_VERSION 1

#elif defined(__ARM_ARCH_5T__) \
    || defined(__ARM_ARCH_5TE__) \
    || defined(__ARM_ARCH_5TEJ__)
#define PLATFORM_THUMB_ARCH_VERSION 2

#elif defined(__ARM_ARCH_6J__) \
    || defined(__ARM_ARCH_6K__) \
    || defined(__ARM_ARCH_6Z__) \
    || defined(__ARM_ARCH_6ZK__) \
    || defined(__ARM_ARCH_6M__)
#define PLATFORM_THUMB_ARCH_VERSION 3

#elif defined(__ARM_ARCH_6T2__) \
    || defined(__ARM_ARCH_7__) \
    || defined(__ARM_ARCH_7A__) \
    || defined(__ARM_ARCH_7R__) \
    || defined(__ARM_ARCH_7M__)
#define PLATFORM_THUMB_ARCH_VERSION 4

/* RVCT sets __TARGET_ARCH_THUMB */
#elif defined(__TARGET_ARCH_THUMB)
#define PLATFORM_THUMB_ARCH_VERSION __TARGET_ARCH_THUMB

#else
#define PLATFORM_THUMB_ARCH_VERSION 0
#endif


/* PLATFORM_CPU(ARMV5_OR_LOWER) - ARM instruction set v5 or earlier */
/* On ARMv5 and below the natural alignment is required. 
   And there are some other differences for v5 or earlier. */
#if !defined(ARMV5_OR_LOWER) && !PLATFORM_ARM_ARCH_AT_LEAST(6)
#define PLATFORM_CPU_ARMV5_OR_LOWER 1
#endif


/* PLATFORM_CPU(ARM_TRADITIONAL) - Thumb2 is not available, only traditional ARM (v4 or greater) */
/* PLATFORM_CPU(ARM_THUMB2) - Thumb2 instruction set is available */
/* Only one of these will be defined. */
#if !defined(PLATFORM_CPU_ARM_TRADITIONAL) && !defined(PLATFORM_CPU_ARM_THUMB2)
#  if defined(thumb2) || defined(__thumb2__) \
    || ((defined(__thumb) || defined(__thumb__)) && PLATFORM_THUMB_ARCH_VERSION == 4)
#    define PLATFORM_CPU_ARM_TRADITIONAL 0
#    define PLATFORM_CPU_ARM_THUMB2 1
#  elif PLATFORM_ARM_ARCH_AT_LEAST(4)
#    define PLATFORM_CPU_ARM_TRADITIONAL 1
#    define PLATFORM_CPU_ARM_THUMB2 0
#  else
#    error "Not supported ARM architecture"
#  endif
#elif PLATFORM_CPU(ARM_TRADITIONAL) && PLATFORM_CPU(ARM_THUMB2) /* Sanity Check */
#  error "Cannot use both of PLATFORM_CPU_ARM_TRADITIONAL and PLATFORM_CPU_ARM_THUMB2 platforms"
#endif /* !defined(PLATFORM_CPU_ARM_TRADITIONAL) && !defined(PLATFORM_CPU_ARM_THUMB2) */

#if defined(__ARM_NEON__) && !defined(PLATFORM_CPU_ARM_NEON)
#define PLATFORM_CPU_ARM_NEON 1
#endif

#endif /* ARM */

#if PLATFORM_CPU(ARM) || PLATFORM_CPU(MIPS) || PLATFORM_CPU(SH4) || PLATFORM_CPU(SPARC)
#define PLATFORM_CPU_NEEDS_ALIGNED_ACCESS 1
#endif

/* ==== PLATFORM_OS() - underlying operating system; only to be used for mandated low-level services like
   virtual memory, not to choose a GUI toolkit ==== */

/* PLATFORM_OS(ANDROID) - Android */
#ifdef ANDROID
#define PLATFORM_OS_ANDROID 1
#endif

/* PLATFORM_OS(AIX) - AIX */
#ifdef _AIX
#define PLATFORM_OS_AIX 1
#endif

/* PLATFORM_OS(DARWIN) - Any Darwin-based OS, including Mac OS X and iPhone OS */
#ifdef __APPLE__
#define PLATFORM_OS_DARWIN 1

#include <Availability.h>
#include <AvailabilityMacros.h>
#include <TargetConditionals.h>
#endif

/* PLATFORM_OS(IOS) - iOS */
/* PLATFORM_OS(MAC_OS_X) - Mac OS X (not including iOS) */
#if PLATFORM_OS(DARWIN) && ((defined(TARGET_OS_EMBEDDED) && TARGET_OS_EMBEDDED) \
    || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)                 \
    || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR))
#define PLATFORM_OS_IOS 1
#elif PLATFORM_OS(DARWIN) && defined(TARGET_OS_MAC) && TARGET_OS_MAC
#define PLATFORM_OS_MAC_OS_X 1
/* FIXME: BUILDING_ON_.., and TARGETING... macros should be folded into the PLATFORM_OS() system */
#if !defined(MAC_OS_X_VERSION_10_6) || MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_6
#define BUILDING_ON_LEOPARD 1
#elif !defined(MAC_OS_X_VERSION_10_7) || MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_7
#define BUILDING_ON_SNOW_LEOPARD 1
#elif !defined(MAC_OS_X_VERSION_10_8) || MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_8
#define BUILDING_ON_LION 1
#endif
#if !defined(MAC_OS_X_VERSION_10_6) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
#define TARGETING_LEOPARD 1
#elif !defined(MAC_OS_X_VERSION_10_7) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_7
#define TARGETING_SNOW_LEOPARD 1
#elif !defined(MAC_OS_X_VERSION_10_8) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_8
#define TARGETING_LION 1
#endif
#endif

/* PLATFORM_OS(FREEBSD) - FreeBSD */
#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#define PLATFORM_OS_FREEBSD 1
#endif

/* PLATFORM_OS(HURD) - GNU/Hurd */
#ifdef __GNU__
#define PLATFORM_OS_HURD 1
#endif

/* PLATFORM_OS(LINUX) - Linux */
#ifdef __linux__
#define PLATFORM_OS_LINUX 1
#endif

/* PLATFORM_OS(NETBSD) - NetBSD */
#if defined(__NetBSD__)
#define PLATFORM_OS_NETBSD 1
#endif

/* PLATFORM_OS(OPENBSD) - OpenBSD */
#ifdef __OpenBSD__
#define PLATFORM_OS_OPENBSD 1
#endif

/* PLATFORM_OS(QNX) - QNX */
#if defined(__QNXNTO__)
#define PLATFORM_OS_QNX 1
#endif

/* PLATFORM_OS(SOLARIS) - Solaris */
#if defined(sun) || defined(__sun)
#define PLATFORM_OS_SOLARIS 1
#endif

/* PLATFORM_OS(WINCE) - Windows CE; note that for this platform PLATFORM_OS(WINDOWS) is also defined */
#if defined(_WIN32_WCE)
#define PLATFORM_OS_WINCE 1
#endif

/* PLATFORM_OS(WINDOWS) - Any version of Windows */
#if defined(WIN32) || defined(_WIN32)
#define PLATFORM_OS_WINDOWS 1
#endif

/* PLATFORM_OS(UNIX) - Any Unix-like system */
#if    PLATFORM_OS(AIX)       \
    || PLATFORM_OS(ANDROID)   \
    || PLATFORM_OS(DARWIN)    \
    || PLATFORM_OS(FREEBSD)   \
    || PLATFORM_OS(HURD)      \
    || PLATFORM_OS(LINUX)     \
    || PLATFORM_OS(NETBSD)    \
    || PLATFORM_OS(OPENBSD)   \
    || PLATFORM_OS(QNX)       \
    || PLATFORM_OS(SOLARIS)   \
    || defined(unix)     \
    || defined(__unix)   \
    || defined(__unix__)
#define PLATFORM_OS_UNIX 1
#endif


/* PLATFORM_ENABLE(EXCEPTIONS) - Enable exceptions by default */
#if !defined(PLATFORM_ENABLE_EXCEPTIONS)
#define PLATFORM_ENABLE_EXCEPTIONS 1
#endif

#endif /* PLATFORM_H */
