#include "filesystemfolderproxymodel.h"
#include "items/filesystemfoldernodeentry.h"
#include <QtCore/QDateTime>


FILE_SYSTEM_NS_BEGIN

FolderProxyModel::FolderProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool FolderProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<FolderNodeItem*>(left.internalPointer())->isRoot())
		return true;
	else
		if (static_cast<FolderNodeItem*>(right.internalPointer())->isRoot())
			return false;
		else
		{
			FolderNodeEntry *leftItem = static_cast<FolderNodeEntry*>(left.internalPointer());
			FolderNodeEntry *rightItem = static_cast<FolderNodeEntry*>(right.internalPointer());

			switch (left.column())
			{
				case 0:
				{
					if (leftItem->isDir())
						if (rightItem->isDir())
							return leftItem->fileName() < rightItem->fileName();
						else
							return true;
					else
						if (rightItem->isDir())
							return false;
						else
							return leftItem->fileName() < rightItem->fileName();

					break;
				}
				case 1:
				{
					if (leftItem->isDir())
						if (rightItem->isDir())
							return leftItem->totalSize().toULongLong() < rightItem->totalSize().toULongLong();
						else
							return true;
					else
						if (rightItem->isDir())
							return false;
						else
							return leftItem->size() < rightItem->size();

					break;
				}
				case 2:
				{
					if (leftItem->isDir())
						if (rightItem->isDir())
							return leftItem->lastModified() < rightItem->lastModified();
						else
							return true;
					else
						if (rightItem->isDir())
							return false;
						else
							return leftItem->lastModified() < rightItem->lastModified();

					break;
				}
			}
		}

	return true;
}

FILE_SYSTEM_NS_END
