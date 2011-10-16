#include "filesystemfolderproxymodel.h"
#include "items/filesystementryitem.h"
#include <QtCore/QDateTime>


FILE_SYSTEM_NS_BEGIN

FolderProxyModel::FolderProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool FolderProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<FileSystemBaseItem*>(left.internalPointer())->isRootItem())
		return true;
	else
		if (static_cast<FileSystemBaseItem*>(right.internalPointer())->isRootItem())
			return false;
		else
		{
			FileSystemEntryItem *leftItem = static_cast<FileSystemEntryItem*>(left.internalPointer());
			FileSystemEntryItem *rightItem = static_cast<FileSystemEntryItem*>(right.internalPointer());

			switch (left.column())
			{
				case 0:
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
				case 1:
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
				case 2:
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
			}
		}

	return true;
}

FILE_SYSTEM_NS_END
