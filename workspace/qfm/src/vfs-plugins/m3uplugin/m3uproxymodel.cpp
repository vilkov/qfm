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
#include "m3uproxymodel.h"
#include "items/m3uentry.h"


M3U_PLUGIN_NS_BEGIN

M3uProxyModel::M3uProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool M3uProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<M3uItem*>(left.internalPointer())->isRoot())
		return true;
	else
		if (static_cast<M3uItem*>(right.internalPointer())->isRoot())
			return false;
		else
		{
			M3uEntry *leftItem = static_cast<M3uEntry*>(left.internalPointer());
			M3uEntry *rightItem = static_cast<M3uEntry*>(right.internalPointer());

			switch (left.column())
			{
				case 0: return leftItem->title() < rightItem->title();
				case 1: return leftItem->length() < rightItem->length();
			}
		}

	return true;
}

M3U_PLUGIN_NS_END
