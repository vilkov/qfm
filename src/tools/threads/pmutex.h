#ifndef PMUTEX_H_
#define PMUTEX_H_

#if defined(TOOLS_THREADS_BACKEND_BOOST)
#	include "backends/boost/pmutex_boost.h"
#elif defined(TOOLS_THREADS_BACKEND_QT)
#	include "backends/qt/pmutex_qt.h"
#else
#	error Tools backend is not defined!
#endif

#endif /* PMUTEX_H_ */
