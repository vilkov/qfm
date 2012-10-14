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
#include "idm_storage_undoaddentity.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoAddEntity::StorageUndoAddEntity(Entity *entity) :
	m_entity(entity)
{}

void StorageUndoAddEntity::undo(RootEntity &root)
{
	root.remove(m_entity);

	for (Entity::size_type i = 0, size = m_entity->size(); i < size; ++i)
		m_entity->at(i).entity->removeParent(m_entity);

	delete m_entity;
}

IDM_PLUGIN_NS_END
