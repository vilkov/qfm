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
