#include "idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

IdmEntityValue::IdmEntityValue(IdmEntity *entity, id_type id) :
	m_entity(entity),
	m_id(id)
{}

IdmEntityValue::~IdmEntityValue()
{}

IdmCompositeEntityValue::IdmCompositeEntityValue(IdmEntity *entity, id_type id) :
	IdmEntityValue(entity, id)
{}

IdmCompositeEntityValue::~IdmCompositeEntityValue()
{
	for (Map::iterator it = m_items.begin(), end = m_items.end(); it != end; ++it)
		qDeleteAll(it.value());
}

IDM_PLUGIN_NS_END
