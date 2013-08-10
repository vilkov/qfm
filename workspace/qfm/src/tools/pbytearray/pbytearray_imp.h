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
#ifndef PBYTEARRAY_IMP_H_
#define PBYTEARRAY_IMP_H_

#include <stddef.h>


class PByteArrayImplementation
{
public:
	typedef char   value_type;
	typedef size_t size_type;

public:
	virtual ~PByteArrayImplementation() {}

	virtual size_type size() const = 0;
	virtual const value_type *c_str() const = 0;
	virtual const value_type *data() const = 0;
	virtual value_type *data() = 0;
	virtual void copyTo(void *dest) const = 0;

	virtual void truncate(size_type pos) = 0;
	virtual void append(const value_type *string, size_type size) = 0;
	virtual void prepend(const value_type *string, size_type size) = 0;
};

#endif /* PBYTEARRAY_IMP_H_ */
