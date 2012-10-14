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
#ifndef PTYPES_BOOST_H_
#define PTYPES_BOOST_H_

#include <boost/cstdint.hpp>
#include "../ptypes_ns.h"


PTYPES_NAMESPACE_BEGIN

	typedef boost::int8_t    int8_t;
	typedef boost::uint8_t   uint8_t;

    typedef boost::int16_t   int16_t;
    typedef boost::uint16_t  uint16_t;

    typedef boost::int32_t   int32_t;
    typedef boost::uint32_t  uint32_t;

    typedef boost::intmax_t  intmax_t;
    typedef boost::uintmax_t uintmax_t;
    typedef boost::int64_t   int64_t;
    typedef boost::uint64_t  uint64_t;

PTYPES_NAMESPACE_END

#endif /* PTYPES_BOOST_H_ */
