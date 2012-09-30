#include "idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

EntityValue::EntityValue(Entity *entity, id_type id) :
	QSharedData(),
	m_entity(entity),
	m_id(id)
{}

EntityValue::~EntityValue()
{}

CompositeEntityValue::CompositeEntityValue(Entity *entity, id_type id) :
	EntityValue(entity, id)
{}

bool CompositeEntityValue::contains(const EntityValue::Holder &value) const
{
	const List list = values(value->entity());

	for (List::size_type i = 0, size = list.size(); i < size; ++i)
		if (list.at(i)->id() == value->id())
			return true;

	return false;
}

bool CompositeEntityValue::contains(const List &values, EntityValue::Holder &propertyValue) const
{
	if (!values.isEmpty())
	{
		const List list = CompositeEntityValue::values(values.at(0)->entity());

		for (List::size_type i = 0, size = list.size(); i < size; ++i)
			for (List::size_type q = 0, size = values.size(); q < size; ++q)
				if (list.at(i)->id() == values.at(q)->id())
				{
					propertyValue = values.at(q);
					return true;
				}
	}

	return false;
}

IDM_PLUGIN_NS_END
