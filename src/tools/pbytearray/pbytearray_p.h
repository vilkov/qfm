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
	enum { MaxNumberOfArgs = 10 };

public:
	enum { DefaultSize = 4 };

public:
	DynamicByteArray(size_type size = DefaultSize);
	DynamicByteArray(const value_type *string1, size_type size1, const value_type *string2, size_type size2);
	DynamicByteArray(const value_type *string, size_type size,
			const value_type *string1 = NULL,
			const value_type *string2 = NULL,
			const value_type *string3 = NULL,
			const value_type *string4 = NULL,
			const value_type *string5 = NULL,
			const value_type *string6 = NULL,
			const value_type *string7 = NULL,
			const value_type *string8 = NULL,
			const value_type *string9 = NULL,
			const value_type *string10 = NULL);
	virtual ~DynamicByteArray();

	virtual size_type size() const { return m_data->size; }
	virtual const value_type *c_str() const { return m_data->string; }
	virtual const value_type *data() const { return m_data->string; }
	virtual value_type *data();
	virtual void copyTo(void *dest) const { new (dest) DynamicByteArray(*this); }

	virtual void truncate(size_type pos);
	virtual void append(const value_type *string, size_type size);
	virtual void prepend(const value_type *string, size_type size);

private:
	DynamicByteArray(const DynamicByteArray &other);

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
	virtual const value_type *c_str() const { return m_string; }
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
	virtual void prepend(const value_type *string, size_type size)
	{
		this->~ReferenceByteArray();
		new (this) DynamicByteArray(string, size, m_string, m_size);
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
	virtual const value_type *c_str() const
	{
		this->~ConstReferenceByteArray();
		return (new (const_cast<ConstReferenceByteArray*>(this)) DynamicByteArray(m_string, m_size))->c_str();
	}
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
	virtual void prepend(const value_type *string, size_type size)
	{
		this->~ConstReferenceByteArray();
		new (this) DynamicByteArray(string, size, m_string, m_size);
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
	virtual const value_type *c_str() const { return m_string; }
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
	virtual void prepend(const value_type *string, size_type size)
	{
		this->~LiteralByteArray();
		new (this) DynamicByteArray(string, size, m_string, m_size);
	}

private:
	const value_type *m_string;
	size_type m_size;
};


class EmptyByteArray : public PByteArrayImplementation
{
public:
	EmptyByteArray();

	virtual size_type size() const { return 0; }
	virtual const value_type *c_str() const { return m_string; }
	virtual const value_type *data() const { return m_string; }
	virtual value_type *data();
	virtual void copyTo(void *dest) const { new (dest) EmptyByteArray(*this); }

	virtual void truncate(size_type count) {}
	virtual void append(const value_type *string, size_type size);
	virtual void prepend(const value_type *string, size_type size);

private:
	value_type m_string[1];
};

#endif /* PBYTEARRAY_P_H_ */
