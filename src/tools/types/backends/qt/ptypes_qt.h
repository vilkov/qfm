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
