#ifndef OSPLATFORM_H_
#define OSPLATFORM_H_

#include <platformstl/platformstl.hpp>


#if defined(PLATFORMSTL_OS_IS_WINDOWS)
#	define PLATFORM_OS_IS_WINDOWS
#elif defined(PLATFORMSTL_OS_IS_UNIX)
#	define PLATFORM_OS_IS_UNIX
#else
#	include "errors/osisunknownerror.h"
#endif

#endif /* OSPLATFORM_H_ */
