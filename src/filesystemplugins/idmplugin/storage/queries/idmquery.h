#ifndef IDMQUERY_H_
#define IDMQUERY_H_

#include "../entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class Query
{
public:
	Query(IdmEntity *entity);
	virtual ~Query();

	IdmEntity *entity() const { return m_entity; }
	virtual QByteArray compile() const = 0;

private:
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERY_H_ */
