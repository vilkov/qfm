#ifndef OSDEPENDENT_H_
#define OSDEPENDENT_H_

#include "osplatform.h"


/* OS_DEPENDENT_CODE */
#if defined(PLATFORM_OS_IS_WINDOWS)
#	define OS_DEPENDENT_CODE(UNIX, WIN) WIN
#elif defined(PLATFORM_OS_IS_UNIX)
#	define OS_DEPENDENT_CODE(UNIX, WIN) UNIX
#endif


/* Usage:
 *
 *	OS_DEPENDENT_CODE
 *	(
 *		UNIX
 *		{
 *
 *		},
 *
 *		WIN
 *		{
 *
 *		}
 *	)
 *
 * */

#endif /* OSDEPENDENT_H_ */
