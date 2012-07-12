#include "idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

IdmEntityValue::IdmEntityValue(IdmEntity *entity, id_type id) :
	QSharedData(),
	m_entity(entity),
	m_id(id)
{}

IdmEntityValue::~IdmEntityValue()
{}

IdmCompositeEntityValue::IdmCompositeEntityValue(IdmEntity *entity, id_type id) :
	IdmEntityValue(entity, id)
{}

bool IdmCompositeEntityValue::contains(const IdmEntityValue::Holder &value) const
{
	const List list = values(value->entity());

	for (List::size_type i = 0, size = list.size(); i < size; ++i)
		if (list.at(i)->id() == value->id())
			return true;

	return false;
}

bool IdmCompositeEntityValue::contains(const List &values, IdmEntityValue::Holder &propertyValue) const
{
	if (!values.isEmpty())
	{
		const List list = IdmCompositeEntityValue::values(values.at(0)->entity());

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
