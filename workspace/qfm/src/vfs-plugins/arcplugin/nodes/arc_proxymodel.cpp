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
#include "arc_proxymodel.h"
#include "items/arc_entrynodeitem.h"

#include <vfs/model/vfs_proxymodel.h>


ARC_PLUGIN_NS_BEGIN

ProxyModel::ProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool ProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<NodeItem *>(left.internalPointer())->isRoot())
		return sortOrder() == Qt::AscendingOrder;
	else
		if (static_cast<NodeItem *>(right.internalPointer())->isRoot())
			return sortOrder() == Qt::DescendingOrder;
		else
			return VFS::ProxyModel::compareByFileNames(static_cast<EntryNodeItem *>(left.internalPointer())->info(), static_cast<EntryNodeItem *>(right.internalPointer())->info());
}

ARC_PLUGIN_NS_END
