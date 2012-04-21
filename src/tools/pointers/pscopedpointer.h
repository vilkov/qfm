#ifndef PSCOPEDPOINTER_H_
#define PSCOPEDPOINTER_H_

#if defined(TOOLS_POINTERS_BACKEND_BOOST)
#	include "backends/boost/pscopedpointer_boost.h"
#elif defined(TOOLS_POINTERS_BACKEND_QT)
#	include "backends/qt/pscopedpointer_qt.h"
#else
#	include "errors/toolspointersbackendisnotdefined.h"
#endif

#endif /* PSCOPEDPOINTER_H_ */
