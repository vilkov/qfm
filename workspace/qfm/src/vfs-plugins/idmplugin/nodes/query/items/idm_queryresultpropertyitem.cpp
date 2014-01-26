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
#include "idm_queryresultpropertyitem.h"
#include "idm_queryresultvalueitem.h"
#include "../../../containeres/idm_container.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPropertyItem::QueryResultPropertyItem(const Entity::Property &property, Base *parent) :
    QueryResultItem(parent),
    m_property(property)
{}

QueryResultPropertyItem::Base *QueryResultPropertyItem::at(size_type index) const
{
    return m_items.at(index).data();
}

QueryResultPropertyItem::size_type QueryResultPropertyItem::size() const
{
    return m_items.size();
}

QueryResultPropertyItem::size_type QueryResultPropertyItem::indexOf(Base *item) const
{
    QueryResultItem::Holder holder(static_cast<QueryResultItem *>(item));
    return m_items.indexOf(holder);
}

QVariant QueryResultPropertyItem::data(qint32 column, qint32 role) const
{
    if (role == Qt::DisplayRole)
        return toUnicode(m_property.name);
    else
        return QVariant();
}

bool QueryResultPropertyItem::isRoot()
{
    return false;
}

bool QueryResultPropertyItem::isProperty()
{
    return true;
}

bool QueryResultPropertyItem::isValue()
{
    return false;
}

bool QueryResultPropertyItem::isPath()
{
    return false;
}

bool QueryResultPropertyItem::isRootPathValue()
{
    return false;
}

void QueryResultPropertyItem::add(const EntityValue &value)
{
    m_items.push_back(QueryResultItem::Holder(new QueryResultValueItem(value, this)));
}

void QueryResultPropertyItem::add(const EntityValue::Values &values)
{
    for (auto i : values)
        m_items.push_back(QueryResultItem::Holder(new QueryResultValueItem(i.second, this)));
}

void QueryResultPropertyItem::remove(size_type index)
{
    m_items.removeAt(index);
}

IDM_PLUGIN_NS_END
