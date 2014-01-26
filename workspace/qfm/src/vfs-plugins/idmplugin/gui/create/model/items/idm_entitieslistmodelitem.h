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
#ifndef IDM_ENTITIESLISTMODELITEM_H_
#define IDM_ENTITIESLISTMODELITEM_H_

#include <liquiddb/Entity>
#include "../../../../model/items/idm_listitem.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


class IdmEntitiesListItem : public IdmListItem
{
public:
    IdmEntitiesListItem(const Entity::Property &property, IdmItem *parent = 0);

    /* IdmItem */
    virtual QVariant data(qint32 column, qint32 role) const;

    const Entity &entity() const { return m_property.entity; }
    const Entity::Property &property() const { return m_property; }

    void setEntity(const Entity &value) { m_property.entity = value; }
    void setName(const ::EFC::String &name) { m_property.name = name; }

private:
    Entity::Property m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITIESLISTMODELITEM_H_ */
