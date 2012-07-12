#include "idmquery.h"


IDM_PLUGIN_NS_BEGIN

Query::Query(IdmEntity *entity) :
	m_entity(entity)
{}

Query::~Query()
{}

IDM_PLUGIN_NS_END
