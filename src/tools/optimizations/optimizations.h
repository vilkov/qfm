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
#ifndef OPTIMIZATIONS_H_
#define OPTIMIZATIONS_H_

#include <QtCore/qglobal.h>


#ifdef Q_CC_GNU
	/**
	 * Since you are limited to integral expressions for Expression,
	 * you should use constructions such as
	 *
	 *     if (LIKELY(ptr != NULL))
	 *         foo(*ptr);
	 *
	 * when testing pointer or floating-point values.
	 */
	#define LIKELY(Expression) __builtin_expect(Expression, 1)
	#define UNLIKELY(Expression) __builtin_expect(Expression, 0)
#else
	#define LIKELY(Expression) Expression
	#define UNLIKELY(Expression) Expression
#endif


/**
 * Useful links:
 *   http://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */

#endif /* OPTIMIZATIONS_H_ */
