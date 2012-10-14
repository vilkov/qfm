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
#ifndef PCONDITION_H_
#define PCONDITION_H_

#if defined(TOOLS_THREADS_BACKEND_BOOST)
#	include "backends/boost/pcondition_boost.h"
#elif defined(TOOLS_THREADS_BACKEND_QT)
#	include "backends/qt/pcondition_qt.h"
#else
#	error Tools backend is not defined!
#endif

#endif /* PCONDITION_H_ */
