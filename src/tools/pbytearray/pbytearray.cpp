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
