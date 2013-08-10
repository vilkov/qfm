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
#include "idm_queryentitiesmodel.h"
#include "items/idm_queryentitiesmodelitem.h"


IDM_PLUGIN_NS_BEGIN

QueryEntitiesModel::QueryEntitiesModel(QObject *parent) :
	Model(parent)
{}

void QueryEntitiesModel::add(const Entity::Property &property)
{
	QueryEntitiesModelItem *item;

	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(item = new QueryEntitiesModelItem(property));
	expand(item);
	endInsertRows();
}

void QueryEntitiesModel::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

void QueryEntitiesModel::expand(IdmItem *p)
{
	QueryEntitiesModelItem *item;
	QueryEntitiesModelItem *parent = static_cast<QueryEntitiesModelItem*>(p);

	for (Entity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
	{
		parent->add(item = new QueryEntitiesModelItem(parent->entity()->at(i), parent));
		expand(item);
	}
}

IDM_PLUGIN_NS_END
