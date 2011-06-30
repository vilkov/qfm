#ifndef PTHREAD_H_
#define PTHREAD_H_

#if defined(TOOLS_THREADS_BACKEND_BOOST)
#	include "backends/boost/pthread_boost.h"
#elif defined(TOOLS_THREADS_BACKEND_QT)
#	include "backends/qt/pthread_qt.h"
#else
#	error Tools backend is not defined!
#endif

#endif /* PTHREAD_H_ */
