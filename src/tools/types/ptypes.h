#ifndef PTYPES_H_
#define PTYPES_H_

#if defined(TOOLS_TYPES_BACKEND_BOOST)
#	include "backends/boost/ptypes_boost.h"
#elif defined(TOOLS_TYPES_BACKEND_QT)
#	include "backends/qt/ptypes_qt.h"
#else
#	error Tools backend is not defined!
#endif

#endif /* PTYPES_H_ */
