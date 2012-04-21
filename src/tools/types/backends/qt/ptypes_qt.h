#ifndef PTYPES_QT_H_
#define PTYPES_QT_H_

#include <QtCore/QGlobalStatic>
#include "../ptypes_ns.h"


PTYPES_NAMESPACE_BEGIN

	typedef qint8   int8_t;
	typedef quint8  uint8_t;

    typedef qint16  int16_t;
    typedef quint8  uint16_t;

    typedef qint32  int32_t;
    typedef quint32 uint32_t;

    typedef qint32  intmax_t;
    typedef quint32 uintmax_t;
    typedef qint64  int64_t;
    typedef quint64 uint64_t;

PTYPES_NAMESPACE_END

#endif /* PTYPES_QT_H_ */
