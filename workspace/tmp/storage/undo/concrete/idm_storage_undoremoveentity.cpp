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
#include "idm_storage_undoremoveentity.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoRemoveEntity::StorageUndoRemoveEntity(Entity *entity) :
	m_entity(entity)
{}

StorageUndoRemoveEntity::~StorageUndoRemoveEntity()
{
	delete m_entity;
}

void StorageUndoRemoveEntity::undo(RootEntity &root)
{
	root.add(m_entity, m_entity->name());

	for (Parents::size_type i = 0, size = m_parents.size(); i < size; ++i)
		m_parents.at(i).entity->add(m_entity, m_parents.at(i).name);

	for (Entity::size_type i = 0, size = m_entity->size(); i < size; ++i)
		m_entity->at(i).entity->addParent(m_entity);

	m_entity = 0;
}

IDM_PLUGIN_NS_END
