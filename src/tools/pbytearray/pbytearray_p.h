#ifndef PBYTEARRAY_P_H_
#define PBYTEARRAY_P_H_

#include "pbytearray_imp.h"
#include <stdlib.h>
#include <string.h>
#include <new>


class DynamicByteArray : public PByteArrayImplementation
{
private:
	struct Data
	{
		unsigned int ref;
		size_type size;
		size_type capacity;
		value_type string[1];
	};

public:
	enum { DefaultSize = 4 };

public:
	DynamicByteArray(size_type size = DefaultSize) :
		m_data(static_cast<Data*>(malloc(sizeof(Data) + size)))
	{
		m_data->ref = 1;
		m_data->size = 0;
		m_data->capacity = size;
		m_data->string[0] = 0;
	}
	DynamicByteArray(const value_type *string, size_type size) :
		m_data(static_cast<Data*>(malloc(sizeof(Data) + size)))
	{
		m_data->ref = 1;
		m_data->size = size;
		m_data->capacity = 0;
		memcpy(m_data->string, string, size);
		m_data->string[size] = 0;
	}
	DynamicByteArray(const value_type *string1, size_type size1, const value_type *string2, size_type size2) :
		m_data(static_cast<Data*>(malloc(sizeof(Data) + size1 + size2)))
	{
		m_data->ref = 1;
		m_data->capacity = 0;
		memcpy(m_data->string, string1, size1);
		memcpy(m_data->string + size1, string2, size2);
		m_data->string[m_data->size = size1 + size2] = 0;
	}
	virtual ~DynamicByteArray()
	{
		if ((--m_data->ref) == 0)
			free(m_data);
	}

	virtual size_type size() const { return m_data->size; }
	virtual const value_type *data() const { return m_data->string; }
	virtual value_type *data() { return m_data->string; }
	virtual void copyTo(void *dest) const { new (dest) DynamicByteArray(*this); }

	virtual void truncate(size_type pos)
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
	virtual void append(const value_type *string, size_type size)
	{
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

private:
	DynamicByteArray(const DynamicByteArray &other) :
		m_data(other.m_data)
	{
		++m_data->ref;
	}

private:
	Data *m_data;
};


class ReferenceByteArray : public PByteArrayImplementation
{
public:
	ReferenceByteArray(value_type *string, size_type size) :
		m_string(string),
		m_size(size)
	{}

	virtual size_type size() const { return m_size; }
	virtual const value_type *data() const { return m_string; }
	virtual value_type *data() { return m_string; }
	virtual void copyTo(void *dest) const { new (dest) ReferenceByteArray(*this); }

	virtual void truncate(size_type pos)
	{
		if (pos < m_size)
		{
			m_size = pos;
			m_string[m_size] = 0;
		}
	}
	virtual void append(const value_type *string, size_type size)
	{
		this->~ReferenceByteArray();
		new (this) DynamicByteArray(m_string, m_size, string, size);
	}

private:
	value_type *m_string;
	size_type m_size;
};


class ConstReferenceByteArray : public PByteArrayImplementation
{
public:
	ConstReferenceByteArray(const value_type *string, size_type size) :
		m_string(string),
		m_size(size)
	{}

	virtual size_type size() const { return m_size; }
	virtual const value_type *data() const { return m_string; }
	virtual value_type *data()
	{
		this->~ConstReferenceByteArray();
		return (new (this) DynamicByteArray(m_string, m_size))->data();
	}
	virtual void copyTo(void *dest) const { new (dest) ConstReferenceByteArray(*this); }

	virtual void truncate(size_type pos)
	{
		if (pos < m_size)
		{
			this->~ConstReferenceByteArray();
			new (this) DynamicByteArray(m_string, pos);
		}
	}
	virtual void append(const value_type *string, size_type size)
	{
		this->~ConstReferenceByteArray();
		new (this) DynamicByteArray(m_string, m_size, string, size);
	}

private:
	const value_type *m_string;
	size_type m_size;
};


class LiteralByteArray : public PByteArrayImplementation
{
public:
	LiteralByteArray(const value_type *string, size_type size) :
		m_string(string),
		m_size(size)
	{}

	virtual size_type size() const { return m_size; }
	virtual const value_type *data() const { return m_string; }
	virtual value_type *data()
	{
		this->~LiteralByteArray();
		return (new (this) DynamicByteArray(m_string, m_size))->data();
	}
	virtual void copyTo(void *dest) const { new (dest) LiteralByteArray(*this); }

	virtual void truncate(size_type pos)
	{
		if (pos < m_size)
		{
			this->~LiteralByteArray();
			new (this) DynamicByteArray(m_string, pos);
		}
	}
	virtual void append(const value_type *string, size_type size)
	{
		this->~LiteralByteArray();
		new (this) DynamicByteArray(m_string, m_size, string, size);
	}

private:
	const value_type *m_string;
	size_type m_size;
};


class EmptyByteArray : public PByteArrayImplementation
{
public:
	EmptyByteArray()
	{
		m_string[0] = 0;
	}

	virtual size_type size() const { return 0; }
	virtual const value_type *data() const { return m_string; }
	virtual value_type *data()
	{
		this->~EmptyByteArray();
		return (new (this) DynamicByteArray())->data();
	}
	virtual void copyTo(void *dest) const { new (dest) EmptyByteArray(*this); }

	virtual void truncate(size_type count) {}
	virtual void append(const value_type *string, size_type size)
	{
		this->~EmptyByteArray();
		new (this) DynamicByteArray(string, size);
	}

private:
	value_type m_string[1];
};

#endif /* PBYTEARRAY_P_H_ */
