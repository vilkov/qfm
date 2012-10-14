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
#ifndef VERSION_H_
#define VERSION_H_

#define MAJOR_VERSION_NUMBER    0
#define MINOR_VERSION_NUMBER    0
#define RELEASE_VERSION_NUMBER  0
#define BUILD_VERSION_NUMBER    1
#ifdef RC_INVOKED
#	define VERSION_STRING       "0.0.0.1\0"
#else
#	define VERSION_STRING       "0.0.0.1"
#endif

#endif /* VERSION_H_ */
