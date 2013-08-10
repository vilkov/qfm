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
#ifndef IDM_ROOTNODEPROPERTYITEM_H_
#define IDM_ROOTNODEPROPERTYITEM_H_

#include "idm_rootnodeentityitem.h"


IDM_PLUGIN_NS_BEGIN

class RootNodePropertyItem : public RootNodeEntityItem
{
public:
	RootNodePropertyItem(const Entity::Property &property, Base *parent);
	RootNodePropertyItem(Entity *property, const QString &name, Base *parent);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* RootNodeItem */
	virtual bool isProperty();

	const QString &name() const { return m_property.name; }

private:
	Entity::Property m_property;
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTNODEPROPERTYITEM_H_ */
