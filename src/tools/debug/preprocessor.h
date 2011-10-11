#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_

#include "../os/osplatform.h"


/*
 * Note:
 *     MSVC: __FUNCTION__, __FUNCDNAME__, __FUNCSIG__
 *     GCC: __func__, __FUNCTION__, __PRETTY_FUNCTION__
 *     MSVC/GCC: __FILE__ __LINE__
 *
 */

#if defined(PLATFORM_OS_IS_WINDOWS)
#	define THIS_FILE               __FILE__
#	define THIS_LINE               __LINE__
#	define THIS_FUNCTIOIN          __FUNCTION__
#	define THIS_PRETTY_FUNCTIOIN   __FUNCDNAME__
#	define THIS_PRETTY_FUNCTIOIN_2 __FUNCSIG__
#elif defined(PLATFORM_OS_IS_UNIX)
#	define THIS_FILE               __FILE__
#	define THIS_LINE               __LINE__
#	define THIS_FUNCTIOIN          __FUNCTION__
#	define THIS_PRETTY_FUNCTIOIN   __PRETTY_FUNCTION__
#endif

#endif /* PREPROCESSOR_H_ */
