#ifndef IDMENTITYROOT_H_
#define IDMENTITYROOT_H_

#include <QtCore/QList>
#include "idmentitylist.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntityRoot : public IdmEntityList
{
public:
	IdmEntityRoot(id_type id, const QString &name) :
		IdmEntityList(id, name)
	{}
	virtual ~IdmEntityRoot()
	{
		for (size_type i = 0, size = m_items.size(); i < size; ++i)
			delete m_items.at(i);
	}
};

FILE_SYSTEM_NS_END

#endif /* IDMENTITYROOT_H_ */
