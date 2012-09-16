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
