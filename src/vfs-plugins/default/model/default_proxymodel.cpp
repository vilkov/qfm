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
#include "default_proxymodel.h"
#include "../model/items/default_nodeitem.h"

#include <QtCore/QDateTime>


DEFAULT_PLUGIN_NS_BEGIN

ProxyModel::ProxyModel(QObject *parent) :
	::VFS::ProxyModel(parent)
{}

bool ProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<NodeItem *>(left.internalPointer())->isRootItem())
		return sortOrder() == Qt::AscendingOrder;
	else
		if (static_cast<NodeItem*>(right.internalPointer())->isRootItem())
			return sortOrder() == Qt::DescendingOrder;
		else
		{
			NodeItem *leftItem = static_cast<NodeItem *>(left.internalPointer());
			NodeItem *rightItem = static_cast<NodeItem *>(right.internalPointer());

			switch (left.column())
			{
				case 0:
					if (leftItem->info()->isDir())
						if (rightItem->info()->isDir())
							return compareFileNames(leftItem->info()->fileName(), rightItem->info()->fileName());
						else
							return true;
					else
						if (rightItem->info()->isDir())
							return false;
						else
							return compareFileNames(leftItem->info()->fileName(), rightItem->info()->fileName());
				case 1:
					if (leftItem->info()->isDir())
						if (rightItem->info()->isDir())
							return leftItem->totalSize().toULongLong() < rightItem->totalSize().toULongLong();
						else
							return true;
					else
						if (rightItem->info()->isDir())
							return false;
						else
							return leftItem->info()->fileSize() < rightItem->info()->fileSize();
				case 2:
					if (leftItem->info()->isDir())
						if (rightItem->info()->isDir())
							return leftItem->info()->lastModified() < rightItem->info()->lastModified();
						else
							return true;
					else
						if (rightItem->info()->isDir())
							return false;
						else
							return leftItem->info()->lastModified() < rightItem->info()->lastModified();
			}
		}

	return true;
}

DEFAULT_PLUGIN_NS_END
