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
#include "idm_entitiestreemodel.h"
#include "items/idm_entitiestreeitem.h"


IDM_PLUGIN_NS_BEGIN

EntitiesTreeModel::EntitiesTreeModel(QObject *parent) :
	Model(parent)
{}

void EntitiesTreeModel::add(Entity *entity)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	doAdd(entity);
	endInsertRows();
}

void EntitiesTreeModel::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

void EntitiesTreeModel::doAdd(Entity *entity)
{
	IdmEntitiesTreeItem *item;

	m_items.push_back(item = new IdmEntitiesTreeItem(entity));
	m_entities[entity].push_back(item);
	expand(item);
}

void EntitiesTreeModel::doAdd(IdmItem *item, Entity *property)
{
	IdmEntitiesTreeItem *child;

	static_cast<IdmEntitiesTreeItem*>(item)->add(child = new IdmEntitiesTreeItem(property, item));
	m_entities[property].push_back(child);
	expand(child);
}

void EntitiesTreeModel::doRemove(IdmItem *item, Container::size_type index)
{
	Container::List &items = m_entities[static_cast<IdmEntitiesTreeItem *>(item)->entity()];
	items.removeAt(items.indexOf(item));
	static_cast<IdmEntitiesTreeItem*>(item->parent())->remove(index);
}

void EntitiesTreeModel::expand(IdmItem *p)
{
	Entity *entity;
	IdmEntitiesTreeItem *item;
	IdmEntitiesTreeItem *parent = static_cast<IdmEntitiesTreeItem *>(p);

	for (Entity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
	{
		parent->add(item = new IdmEntitiesTreeItem(entity = parent->entity()->at(i).entity, parent));
		m_entities[entity].push_back(item);
		expand(item);
	}
}

IDM_PLUGIN_NS_END
