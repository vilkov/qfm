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
#include "idm_valuelistproxymodel.h"
#include "idm_valuelistmodel.h"

#include <cstring>


IDM_PLUGIN_NS_BEGIN

ValueListProxyModel::ValueListProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{}

void ValueListProxyModel::setFilter(const QString &filter)
{
    m_filter = filter;
    invalidateFilter();
}

bool ValueListProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_filter.isEmpty() || sourceParent.isValid())
        return true;
    else
        return strstr(static_cast<ValueListModel *>(sourceModel())->at(sourceRow).value().asString(), fromUnicode(m_filter).data()) != NULL;
}

bool ValueListProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return strcmp(static_cast<ValueListModel *>(sourceModel())->at(left.row()).value().asString(), static_cast<ValueListModel *>(sourceModel())->at(right.row()).value().asString()) < 0;
}

IDM_PLUGIN_NS_END
