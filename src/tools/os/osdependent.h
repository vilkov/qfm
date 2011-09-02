#ifndef OSDEPENDENT_H_
#define OSDEPENDENT_H_

#include <platformstl/platformstl.hpp>


/* OS_DEPENDENT_CODE */
#if defined(PLATFORMSTL_OS_IS_WINDOWS)
#	define OS_DEPENDENT_CODE(UNIX, WIN) WIN
#elif defined(PLATFORMSTL_OS_IS_UNIX)
#	define OS_DEPENDENT_CODE(UNIX, WIN) UNIX
#else
#	error OS is unknown!
#endif

#endif /* OSDEPENDENT_H_ */
