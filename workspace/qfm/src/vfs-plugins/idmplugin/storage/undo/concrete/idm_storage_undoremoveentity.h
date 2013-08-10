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
#ifndef IDM_STORAGE_UNDOREMOVEENTITY_H_
#define IDM_STORAGE_UNDOREMOVEENTITY_H_

#include <QtCore/QList>
#include "../idm_storage_undocommand.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoRemoveEntity : public StorageUndoCommand
{
public:
	typedef QList<Entity::Property> Parents;

public:
	StorageUndoRemoveEntity(Entity *entity);
	virtual ~StorageUndoRemoveEntity();

	virtual void undo(RootEntity &root);

	void addParent(Entity *entity, const QString &name) { m_parents.push_back(Entity::Property(entity, name)); }

private:
	Entity *m_entity;
	Parents m_parents;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOREMOVEENTITY_H_ */
