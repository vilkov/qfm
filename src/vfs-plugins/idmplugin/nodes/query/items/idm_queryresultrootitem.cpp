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
#include "idm_queryresultrootitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultRootItem::QueryResultRootItem(const EntityValue::Holder &value, Base *parent) :
	QueryResultItem(parent),
	m_value(value)
{}

QVariant QueryResultRootItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

bool QueryResultRootItem::isRoot()
{
	return true;
}

bool QueryResultRootItem::isProperty()
{
	return false;
}

bool QueryResultRootItem::isValue()
{
	return false;
}

bool QueryResultRootItem::isPath()
{
	return false;
}

bool QueryResultRootItem::isRootPathValue()
{
	return false;
}

IDM_PLUGIN_NS_END
