#ifndef IDM_QUERY_H_
#define IDM_QUERY_H_

#include "../entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class Query
{
public:
	Query(Entity *entity);
	virtual ~Query();

	Entity *entity() const { return m_entity; }
	virtual QByteArray compile() const = 0;

private:
	Entity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERY_H_ */
