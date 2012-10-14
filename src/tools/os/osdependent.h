/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
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
