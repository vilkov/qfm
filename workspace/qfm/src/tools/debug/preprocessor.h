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
