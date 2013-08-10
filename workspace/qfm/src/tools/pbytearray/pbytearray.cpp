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
#include "pbytearray.h"
#include "pbytearray_p.h"
#include <cassert>


#define ASSERTS                                      \
	assert(Size >= sizeof(DynamicByteArray));        \
	assert(Size >= sizeof(ReferenceByteArray));      \
	assert(Size >= sizeof(ConstReferenceByteArray)); \
	assert(Size >= sizeof(LiteralByteArray));        \
	assert(Size >= sizeof(EmptyByteArray))


void PByteArray::createEmpty()
{
	ASSERTS;
	new (m_imp) EmptyByteArray();
}

void PByteArray::createDynamic(size_type size)
{
	ASSERTS;
	new (m_imp) DynamicByteArray(size);
}

void PByteArray::createLiteral(const value_type *data, size_type size)
{
	ASSERTS;
	new (m_imp) LiteralByteArray(data, size);
}

void PByteArray::createDynamic(const value_type *data, size_type size,
		const value_type *string1,
		const value_type *string2,
		const value_type *string3,
		const value_type *string4,
		const value_type *string5,
		const value_type *string6,
		const value_type *string7,
		const value_type *string8,
		const value_type *string9,
		const value_type *string10)
{
	ASSERTS;
	new (m_imp) DynamicByteArray(data, size, string1, string2, string3, string4, string5, string6, string7, string8, string9, string10);
}

void PByteArray::createReference(value_type *data, size_type size)
{
	ASSERTS;
	new (m_imp) ReferenceByteArray(data, size);
}

void PByteArray::createConstReference(const value_type *data, size_type size)
{
	ASSERTS;
	new (m_imp) ConstReferenceByteArray(data, size);
}
