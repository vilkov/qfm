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
#include "pbytearray_p.h"


DynamicByteArray::DynamicByteArray(size_type size) :
	m_data(static_cast<Data*>(malloc(sizeof(Data) + size)))
{
	if (m_data)
	{
		m_data->ref = 1;
		m_data->size = 0;
		m_data->capacity = size;
		m_data->string[0] = 0;
	}
	else
		new (this) EmptyByteArray();
}

DynamicByteArray::DynamicByteArray(const value_type *string1, size_type size1, const value_type *string2, size_type size2) :
	m_data(static_cast<Data*>(malloc(sizeof(Data) + size1 + size2)))
{
	if (m_data)
	{
		m_data->ref = 1;
		m_data->capacity = 0;
		memcpy(m_data->string, string1, size1);
		memcpy(m_data->string + size1, string2, size2);
		m_data->string[m_data->size = size1 + size2] = 0;
	}
	else
		new (this) EmptyByteArray();
}

DynamicByteArray::DynamicByteArray(const value_type *string, size_type size,
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
	struct { const value_type *string; size_type size; } strings[MaxNumberOfArgs] =
	{
			{ string1,  string1  ? strlen(string1)  : 0 },
			{ string2,  string2  ? strlen(string2)  : 0 },
			{ string3,  string3  ? strlen(string3)  : 0 },
			{ string4,  string4  ? strlen(string4)  : 0 },
			{ string5,  string5  ? strlen(string5)  : 0 },
			{ string6,  string6  ? strlen(string6)  : 0 },
			{ string7,  string7  ? strlen(string7)  : 0 },
			{ string8,  string8  ? strlen(string8)  : 0 },
			{ string9,  string9  ? strlen(string9)  : 0 },
			{ string10, string10 ? strlen(string10) : 0 },
	};
	size_type totalSize = size;

	for (unsigned char i = 0; i < MaxNumberOfArgs; ++i)
		totalSize += strings[i].size;

	if (m_data = static_cast<Data*>(malloc(sizeof(Data) + totalSize)))
	{
		m_data->ref = 1;
		m_data->size = size;
		m_data->capacity = 0;
		memcpy(m_data->string, string, size);

		for (unsigned char i = 0; i < MaxNumberOfArgs; ++i)
			if (strings[i].size)
			{
				memcpy(m_data->string + m_data->size, strings[i].string, strings[i].size);
				m_data->size += strings[i].size;
			}

		m_data->string[m_data->size] = 0;
	}
	else
		new (this) EmptyByteArray();
}

DynamicByteArray::~DynamicByteArray()
{
	if ((--m_data->ref) == 0)
		free(m_data);
}

DynamicByteArray::value_type *DynamicByteArray::data()
{
	if (m_data->ref > 1)
	{
		this->~DynamicByteArray();
		return (new (this) DynamicByteArray(m_data->string, m_data->size))->data();
	}
	else
		return m_data->string;
}

void DynamicByteArray::truncate(size_type pos)
{
	if (m_data->size == 0)
		if (pos < m_data->capacity)
		{
			m_data->size = pos;
			m_data->capacity -= pos;
			m_data->string[m_data->size] = 0;
		}
		else
		{
			m_data->size = m_data->capacity;
			m_data->capacity = 0;
			m_data->string[m_data->size] = 0;
		}
	else
		if (pos < m_data->size)
		{
			m_data->capacity += m_data->size - pos;
			m_data->size = pos;
			m_data->string[m_data->size] = 0;
		}
		else
			if (pos < m_data->size + m_data->capacity)
			{
				m_data->capacity = m_data->size + m_data->capacity - pos;
				m_data->size = pos;
				m_data->string[m_data->size] = 0;
			}
}

void DynamicByteArray::append(const value_type *string, size_type size)
{
	if (m_data->ref > 1)
	{
		this->~DynamicByteArray();
		new (this) DynamicByteArray(m_data->string, m_data->size, string, size);
	}
	else
		if (m_data->capacity > size)
		{
			m_data->capacity -= size;
			memcpy(m_data->string + m_data->size, string, size);
			m_data->string[m_data->size += size] = 0;
		}
		else
		{
			Data *newData = static_cast<Data*>(realloc(m_data, sizeof(Data) + m_data->size + size));

			if (newData != NULL)
			{
				m_data = newData;
				m_data->capacity = 0;
				memcpy(m_data->string + m_data->size, string, size);
				m_data->string[m_data->size += size] = 0;
			}
		}
}

void DynamicByteArray::prepend(const value_type *string, size_type size)
{
	if (m_data->ref > 1)
	{
		this->~DynamicByteArray();
		new (this) DynamicByteArray(string, size, m_data->string, m_data->size);
	}
	else
		if (m_data->capacity > size)
		{
			m_data->capacity -= size;
			memmove(m_data->string + size, m_data->string, m_data->size);
			memcpy(m_data->string, string, size);
			m_data->string[m_data->size += size] = 0;
		}
		else
		{
			Data *newData = static_cast<Data*>(realloc(m_data, sizeof(Data) + m_data->size + size));

			if (newData != NULL)
			{
				m_data = newData;
				m_data->capacity = 0;
				memmove(m_data->string + size, m_data->string, m_data->size);
				memcpy(m_data->string, string, size);
				m_data->string[m_data->size += size] = 0;
			}
		}
}

DynamicByteArray::DynamicByteArray(const DynamicByteArray &other) :
	m_data(other.m_data)
{
	++m_data->ref;
}


EmptyByteArray::EmptyByteArray()
{
	m_string[0] = 0;
}

EmptyByteArray::value_type *EmptyByteArray::data()
{
	this->~EmptyByteArray();
	return (new (this) DynamicByteArray())->data();
}

void EmptyByteArray::append(const value_type *string, size_type size)
{
	this->~EmptyByteArray();
	new (this) DynamicByteArray(string, size);
}

void EmptyByteArray::prepend(const value_type *string, size_type size)
{
	this->~EmptyByteArray();
	new (this) DynamicByteArray(string, size);
}
