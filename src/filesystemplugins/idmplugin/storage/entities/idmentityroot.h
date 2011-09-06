#ifndef IDMENTITYROOT_H_
#define IDMENTITYROOT_H_

#include "idmentity.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityRoot : public IdmEntity
{
public:
	IdmEntityRoot() :
		IdmEntity(Database::Composite, 0, QString(), QString())
	{}
	virtual ~IdmEntityRoot()
	{
		for (size_type i = 0, sz = size(); i < sz; ++i)
			delete m_items.at(i).entity;
	}
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITYROOT_H_ */
