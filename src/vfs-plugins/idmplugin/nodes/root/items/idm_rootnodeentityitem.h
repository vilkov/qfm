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
#ifndef IDM_ROOTNODEENTITYITEM_H_
#define IDM_ROOTNODEENTITYITEM_H_

#include "idm_rootnodeitem.h"
#include "../../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeEntityItem : public RootNodeListItem
{
public:
	RootNodeEntityItem(Entity *entity, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* RootNodeItem */
	virtual bool isEntity();

	Entity *entity() const { return m_entity; }

protected:
	friend class RootNode;
	void add(RootNodeItem *item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }

private:
	Entity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTNODEENTITYITEM_H_ */
