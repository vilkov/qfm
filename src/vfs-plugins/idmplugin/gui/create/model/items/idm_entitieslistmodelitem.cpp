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
#include "idm_entitieslistmodelitem.h"


IDM_PLUGIN_NS_BEGIN

IdmEntitiesListItem::IdmEntitiesListItem(const Entity::Property &property, IdmItem *parent) :
	IdmListItem(parent),
	m_property(property)
{}

QVariant IdmEntitiesListItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		switch (column)
		{
			case 0: return m_property.name;
			case 1: return m_property.entity->name();
		}

	return QVariant();
}

IDM_PLUGIN_NS_END
