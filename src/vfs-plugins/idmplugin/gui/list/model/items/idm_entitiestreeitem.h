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
#ifndef IDM_MENUENTITIESTREEITEM_H_
#define IDM_MENUENTITIESTREEITEM_H_

#include "../../../../model/items/idm_listitem.h"
#include "../../../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntitiesTreeItem : public IdmListItem
{
public:
	IdmEntitiesTreeItem(Entity *entity, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	Entity *entity() const { return m_entity; }

protected:
	friend class EntitiesTreeModel;
	void add(IdmListItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }

private:
	Entity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDM_MENUENTITIESTREEITEM_H_ */
