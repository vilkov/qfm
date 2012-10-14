/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
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
