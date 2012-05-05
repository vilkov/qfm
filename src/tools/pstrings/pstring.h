#ifndef PSTRING_H_
#define PSTRING_H_

#include <string>
#include "../templates/metatemplates.h"
#include "../pbytearray/pbytearray.h"


class PString
{
public:
	typedef PByteArray::value_type      value_type;
	typedef PByteArray::size_type       size_type;
	typedef PByteArray::iterator 		iterator;
	typedef PByteArray::const_iterator  const_iterator;
	typedef PByteArray::CopyPolicy      CopyPolicy;
	typedef PByteArray::ReferencePolicy ReferencePolicy;

public:
	/********** Constructors **********/
	PString() :
		m_string()
	{}
	PString(const PString &other) :
		m_string(other.m_string)
	{}
	template <typename T, size_type S>
	PString(const T (&string)[S]) :
		m_string(PByteArray::literal(static_cast<const value_type *>(string), S - 1))
	{}


	/********** Operators **********/
	value_type at(size_type index) const { return m_string.at(index); }
	value_type operator[](size_type index) const { return m_string[index]; }
	bool operator==(const PString &other) const { return m_string == other.m_string; }
	bool operator!=(const PString &other) const { return m_string != other.m_string; }
	bool operator<(const PString &other) const { return m_string < other.m_string; }
	PString & operator=(const PString &other) { m_string = other.m_string; return *this; }
	template <typename T, size_type S>
	PString &operator=(const T (&data)[S]) { m_string = data; return *this; }

	/********** Operations.Data **********/
	size_type size() const { return m_string.size(); }
	const value_type *c_str() const { return m_string.c_str(); }
	const value_type *data() const { return m_string.data(); }
	bool isEmpty() const { return m_string.isEmpty(); }

	/********** Operations.Compare **********/
	bool endsWith(const PString &other) const { return m_string.endsWith(other.m_string); }
	bool isEqual(const PString &other) const { return m_string.isEqual(other.m_string); }

	/********** Operations.Modification **********/
	PString &append(value_type *string) { m_string.append(string); return *this; }
	PString &append(value_type *string, size_type size) { m_string.append(string, size); return *this; }
	PString &append(const value_type *string) { m_string.append(string); return *this; }
	PString &append(const value_type *string, size_type size) { m_string.append(string, size); return *this; }
	PString &append(const PString &string) { m_string.append(string.m_string); return *this; }
	PString &prepend(value_type *string) { m_string.prepend(string); return *this; }
	PString &prepend(value_type *string, size_type size) { m_string.prepend(string, size); return *this; }
	PString &prepend(const value_type *string) { m_string.prepend(string); return *this; }
	PString &prepend(const value_type *string, size_type size) { m_string.prepend(string, size); return *this; }
	PString &prepend(const PString &string) { m_string.prepend(string.m_string); return *this; }

	/********** Operations.Conversion **********/
	int toInt(bool *ok = 0) const { return m_string.toInt(ok); }
	static PString fromInt(int value) { return PString::fromByteArray(PByteArray::fromInt(value)); }

	template <typename T, size_type S>
	static PString format(const T (&string)[S],
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
		return PString(static_cast<const value_type *>(string), S - 1, string1, string2, string3, string4, string5, string6, string7, string8, string9, string10);
	}
	static PString reference(value_type *string, size_type len, ReferencePolicy::Type policy = ReferencePolicy::Reference)
	{
		return PString(string, len, policy);
	}
	static PString reference(const value_type *string, size_type len, CopyPolicy::Type policy = CopyPolicy::CopyOnWrite)
	{
		return PString(string, len, policy);
	}
	static PString reference(const value_type *string, CopyPolicy::Type policy = CopyPolicy::CopyOnWrite)
	{
		return PString(string, policy);
	}
	static PString copy(value_type *string, size_type len, ReferencePolicy::Type policy = ReferencePolicy::CreateCopy)
	{
		return PString(string, len, policy);
	}
	static PString copy(const value_type *string, size_type len, CopyPolicy::Type policy = CopyPolicy::CreateCopy)
	{
		return PString(string, len, policy);
	}
	static PString copy(const value_type *string, CopyPolicy::Type policy = CopyPolicy::CreateCopy)
	{
		return PString(string, policy);
	}
	static PString fromStdString(const std::string &string, CopyPolicy::Type policy = CopyPolicy::CreateCopy)
	{
		return PString(string, policy);
	}
	static PString fromByteArray(const PByteArray &buffer)
	{
		return PString(buffer);
	}


	/********** Iterators **********/
	iterator begin() { return m_string.begin(); }
	iterator end() { return m_string.end(); }
	const_iterator constBegin() const { return m_string.constBegin(); }
	const_iterator constEnd() const { return m_string.constEnd(); }

protected:
	explicit PString(const value_type *data, size_type len,
			const value_type *string1 = NULL,
			const value_type *string2 = NULL,
			const value_type *string3 = NULL,
			const value_type *string4 = NULL,
			const value_type *string5 = NULL,
			const value_type *string6 = NULL,
			const value_type *string7 = NULL,
			const value_type *string8 = NULL,
			const value_type *string9 = NULL,
			const value_type *string10 = NULL) :
		m_string(PByteArray::format(data, len, string1, string2, string3, string4, string5, string6, string7, string8, string9, string10))
	{}
	explicit PString(const std::string &string, CopyPolicy::Type policy) :
		m_string(PByteArray::reference(string.data(), string.size(), policy))
	{}
	explicit PString(value_type *string, size_type len, ReferencePolicy::Type policy) :
		m_string(PByteArray::reference(string, len, policy))
	{}
	explicit PString(const value_type *string, size_type len, CopyPolicy::Type policy) :
		m_string(PByteArray::reference(string, len, policy))
	{}
	explicit PString(const value_type *string, CopyPolicy::Type policy) :
		m_string(PByteArray::reference(string, policy))
	{}
	explicit PString(const PByteArray &buffer) :
		m_string(buffer)
	{}

private:
	PByteArray m_string;
};

#endif /* PSTRING_H_ */
