#ifndef IDMENTITYROOT_H_
#define IDMENTITYROOT_H_

#include "idmentity.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntityRoot : public IdmEntity
{
public:
	IdmEntityRoot(id_type id, const QString &name) :
		IdmEntity(Composite, id, name)
	{}
	virtual ~IdmEntityRoot()
	{
		for (size_type i = 0, sz = size(); i < sz; ++i)
			delete m_items.at(i);
	}
};

FILE_SYSTEM_NS_END

#endif /* IDMENTITYROOT_H_ */
