#ifndef PBYTEARRAY_H_
#define PBYTEARRAY_H_

#include <iterator>
#include <string.h>
#include <string>
#include "pbytearray_imp.h"

#define IMP(Type, var1, var2) Type var1 = ((Type)var2)
#define IMP_D IMP(PByteArrayImplementation*, d, m_imp)
#define IMP_C IMP(const PByteArrayImplementation*, d, m_imp)
#define IMP_O IMP(const PByteArrayImplementation*, o, other.m_imp)
#define IMP_S(var) IMP(const PByteArrayImplementation*, d, var.m_imp)


class PByteArray
{
public:
	typedef PByteArrayImplementation::value_type value_type;
	typedef PByteArrayImplementation::size_type  size_type;
	struct CopyPolicy { enum Type { CreateCopy, CopyOnWrite }; };
	struct ReferencePolicy { enum Type { CreateCopy, Reference }; };

public:
	/********** Constructors **********/
	PByteArray()
	{
		createEmpty();
	}
	PByteArray(size_type size)
	{
		createDynamic(size);
	}
	PByteArray(const value_type *data, CopyPolicy::Type policy = CopyPolicy::CopyOnWrite)
	{
		if (data)
			if (policy == CopyPolicy::CopyOnWrite)
				createConstReference(data, strlen(data));
			else
				createDynamic(data, strlen(data));
		else
			createEmpty();
	}
	PByteArray(const PByteArray &other)
	{
		IMP_O; o->copyTo(m_imp);
	}
	template <typename T, size_type S>
	PByteArray(const T (&data)[S])
	{
		createLiteral(static_cast<const value_type*>(data), S - 1);
	}
	template <typename T, size_type S>
	PByteArray(const T (&data)[S],
			const value_type *string1 = NULL,
			const value_type *string2 = NULL,
			const value_type *string3 = NULL,
			const value_type *string4 = NULL,
			const value_type *string5 = NULL,
			const value_type *string6 = NULL,
			const value_type *string7 = NULL,
			const value_type *string8 = NULL,
			const value_type *string9 = NULL,
			const value_type *string10 = NULL)
	{
		createDynamic(static_cast<const value_type*>(data), S - 1, string1, string2, string3, string4, string5, string6, string7, string8, string9, string10);
	}
	PByteArray(const std::string &string, CopyPolicy::Type policy = CopyPolicy::CreateCopy)
	{
		if (string.size() > 0)
			if (policy == CopyPolicy::CreateCopy)
				createDynamic(static_cast<const value_type*>(string.data()), string.size());
			else
				createConstReference(static_cast<const value_type*>(string.data()), string.size());
		else
			createEmpty();
	}
	PByteArray(value_type *data, size_type len, ReferencePolicy::Type policy = ReferencePolicy::Reference)
	{
		if (data)
			if (policy == ReferencePolicy::Reference)
				createReference(data, len);
			else
				createDynamic(data, len);
		else
			createEmpty();
	}
	PByteArray(const value_type *data, size_type len, CopyPolicy::Type policy = CopyPolicy::CopyOnWrite)
	{
		if (data)
			if (policy == CopyPolicy::CopyOnWrite)
				createConstReference(data, len);
			else
				createDynamic(data, len);
		else
			createEmpty();
	}
	~PByteArray() { IMP_D; d->~PByteArrayImplementation(); }


	/********** Operators **********/
	value_type at(size_type index) const { IMP_C; return d->data()[index]; }
	value_type operator[](size_type index) const { IMP_C; return d->data()[index]; }
	value_type &operator[](size_type index) { IMP_D; return d->data()[index]; }
	bool operator==(const PByteArray &other) const { return isEqual(other); }
	bool operator!=(const PByteArray &other) const { return !isEqual(other); }
	bool operator<(const PByteArray &other) const { return !isLess(other); }
	void operator=(const PByteArray &other)
	{
		IMP_D; d->~PByteArrayImplementation();
		IMP_O; o->copyTo(m_imp);
	}
	template <typename T, size_type S>
	void operator=(const T (&data)[S])
	{
		IMP_D; d->~PByteArrayImplementation();
		createLiteral(static_cast<const value_type*>(data), S - 1);
	}

	/********** Operations.Data **********/
	size_type size() const { IMP_C; return d->size(); }
	const value_type *c_str() const { IMP_D; return d->c_str(); }
	const value_type *data() const { IMP_C; return d->data(); }
	const value_type *constData() const { IMP_C; return d->data(); }
	value_type *data() { IMP_D; return d->data(); }
	bool isEmpty() const { IMP_C; return d->size() == 0; }

	/********** Operations.Compare **********/
	bool endsWith(const PByteArray &other) const
	{
		IMP_C; IMP_O;
		size_type i, q;

		if ((i = d->size()) < (q = o->size()))
			return false;
		else
		{
			const value_type *data1 = d->data() + i - 1;
			const value_type *data2 = o->data() + q - 1;

			for (--q; q > 0; --data1, --data2, --q)
				if ((*data1) != (*data2))
					return false;

			return (*data1) == (*data2);
		}
	}
	bool isEqual(const PByteArray &other) const
	{
		IMP_C; IMP_O;
		size_type q;

		if ((q = d->size()) != o->size())
			return false;
		else
		{
			const value_type *data1 = d->data();
			const value_type *data2 = o->data();

			for (size_type i = 0; i < q; ++data1, ++data2, ++i)
				if ((*data1) != (*data2))
					return false;

			return true;
		}
	}
	bool isLess(const PByteArray &other) const
	{
		IMP_C; IMP_O;
		size_type q;
		size_type e;

		if ((q = d->size()) == 0)
			return true;
		else
			if ((e = o->size()) == 0)
				return false;
			else
			{
				if (q > e)
					q = e;

				const value_type *data1 = d->data();
				const value_type *data2 = o->data();

				for (size_type i = 0; i < q; ++data1, ++data2, ++i)
					if ((*data1) > (*data2))
						return false;

				return true;
			}
	}

	/********** Operations.Modification **********/
	PByteArray &truncate(size_type pos) { IMP_D; d->truncate(pos); return *this; }
	PByteArray &append(const value_type *string) { IMP_D; d->append(string, strlen(string)); return *this; }
	PByteArray &append(const PByteArray &string) { IMP_D; d->append(string.data(), string.size()); return *this; }
	PByteArray &append(const value_type *string, size_type size) { IMP_D; d->append(string, size); return *this; }

	/********** Operations.Conversion **********/
	int toInt(bool *ok = 0) const
	{
		IMP_C;

		if (ok)
			*ok = true;

		if (size_type size = d->size())
		{
			value_type c;
			int mul = 1;
			int res = 0;

			for (const value_type *end = d->data(), *data = end + size - 1; data >= end; --data, mul *= 10)
				if ((c = *data) >= '0' && c <= '9')
					res += (c - '0') * mul;
				else
					if (c == '-')
						return -res;
					else
						if (c == '+')
							return res;
						else
						{
							if (ok)
								*ok = false;

							return 0;
						}

			return res;
		}
		else
			return 0;
	}
	static PByteArray fromInt(int value)
	{
		return PByteArray();
	}

	/********** Iterators **********/
	class iterator
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef PByteArray::value_type          value_type;
		typedef PByteArray::size_type           difference_type;
		typedef value_type *                    pointer;
		typedef value_type &                    reference;

	public:
		bool operator==(const iterator &other) const { return m_data == other.m_data; }
		bool operator!=(const iterator &other) const { return m_data != other.m_data; }

		const value_type &operator*() const { return *m_data; }
		value_type &operator*() { return *m_data; }

		iterator &operator++() { ++m_data; return *this; }
		iterator &operator++(int) { m_data++; return *this; }

		iterator &operator--() { --m_data; return *this; }
		iterator &operator--(int) { m_data--; return *this; }

	protected:
		friend class PByteArray;
		iterator(value_type *data) :
			m_data(data)
		{}

	private:
		/* Disable default constructor */
		iterator();

	private:
		value_type *m_data;
	};

	iterator begin() { IMP_D; return iterator(d->data()); }
	iterator end() { IMP_D; return iterator(d->data() + d->size()); }

	class const_iterator
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef PByteArray::value_type          value_type;
		typedef PByteArray::size_type           difference_type;
		typedef value_type *                    pointer;
		typedef value_type &                    reference;

	public:
		bool operator==(const const_iterator &other) const { return m_data == other.m_data; }
		bool operator!=(const const_iterator &other) const { return m_data != other.m_data; }

		const value_type &operator*() const { return *m_data; }

		const_iterator &operator++() { ++m_data; return *this; }
		const_iterator &operator++(int) { m_data++; return *this; }

		const_iterator &operator--() { --m_data; return *this; }
		const_iterator &operator--(int) { m_data--; return *this; }

	protected:
		friend class PByteArray;
		const_iterator(const value_type *data) :
			m_data(data)
		{}

	private:
		/* Disable default constructor */
		const_iterator();

	private:
		const value_type *m_data;
	};

	const_iterator constBegin() const { IMP_C; return const_iterator(d->data()); }
	const_iterator constEnd() const { IMP_C; return const_iterator(d->data() + d->size()); }

private:
	void createEmpty();
	void createDynamic(size_type size);
	void createLiteral(const value_type *data, size_type size);
	void createDynamic(const value_type *data, size_type size,
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
	void createReference(value_type *data, size_type size);
	void createConstReference(const value_type *data, size_type size);

private:
	enum { Size = 16 };
	unsigned char m_imp[Size];
};


#undef IMP
#undef IMP_D
#undef IMP_C
#undef IMP_O
#undef IMP_S

#endif /* PBYTEARRAY_H_ */
