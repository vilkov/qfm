#include "pbytearray.h"
#include "pbytearray_p.h"


void PByteArray::createEmpty()
{
	new (m_imp) EmptyByteArray();
}

void PByteArray::createDynamic(size_type size)
{
	new (m_imp) DynamicByteArray(size);
}

void PByteArray::createLiteral(const value_type *data, size_type size)
{
	new (m_imp) LiteralByteArray(data, size);
}

void PByteArray::createDynamic(const value_type *data, size_type size)
{
	new (m_imp) DynamicByteArray(data, size);
}

void PByteArray::createReference(value_type *data, size_type size)
{
	new (m_imp) ReferenceByteArray(data, size);
}

void PByteArray::createConstReference(const value_type *data, size_type size)
{
	new (m_imp) ConstReferenceByteArray(data, size);
}
