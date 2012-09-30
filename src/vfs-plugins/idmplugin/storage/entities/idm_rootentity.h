#ifndef IDM_ROOTENTITY_H_
#define IDM_ROOTENTITY_H_

#include "idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class RootEntity : public Entity
{
public:
	RootEntity() :
		Entity(Database::Composite, 0, QString(), QString(), QByteArray(), QByteArray())
	{}
	virtual ~RootEntity()
	{
		for (size_type i = 0, sz = size(); i < sz; ++i)
			delete m_items.at(i).entity;
	}
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTENTITY_H_ */
