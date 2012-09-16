#ifndef IDM_ENTITYROOT_H_
#define IDM_ENTITYROOT_H_

#include "idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityRoot : public IdmEntity
{
public:
	IdmEntityRoot() :
		IdmEntity(Database::Composite, 0, QString(), QString(), QByteArray(), QByteArray())
	{}
	virtual ~IdmEntityRoot()
	{
		for (size_type i = 0, sz = size(); i < sz; ++i)
			delete m_items.at(i).entity;
	}
};

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITYROOT_H_ */
