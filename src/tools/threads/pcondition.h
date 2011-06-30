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
