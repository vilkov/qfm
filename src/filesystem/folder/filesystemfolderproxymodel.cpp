#include "filesystemfolderproxymodel.h"
#include "items/filesystementryitem.h"
#include <QtCore/QDateTime>


FILE_SYSTEM_NS_BEGIN

FolderProxyModel::FolderProxyModel(QObject *parent) :
	ProxyModel(parent)
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
					if (leftItem->info().isDir())
						if (rightItem->info().isDir())
							return compareFileNames(leftItem->info().fileName(), rightItem->info().fileName());
						else
							return true;
					else
						if (rightItem->info().isDir())
							return false;
						else
							return compareFileNames(leftItem->info().fileName(), rightItem->info().fileName());
				case 1:
					if (leftItem->info().isDir())
						if (rightItem->info().isDir())
							return leftItem->totalSize().toULongLong() < rightItem->totalSize().toULongLong();
						else
							return true;
					else
						if (rightItem->info().isDir())
							return false;
						else
							return leftItem->info().fileSize() < rightItem->info().fileSize();
				case 2:
					if (leftItem->info().isDir())
						if (rightItem->info().isDir())
							return leftItem->info().lastModified() < rightItem->info().lastModified();
						else
							return true;
					else
						if (rightItem->info().isDir())
							return false;
						else
							return leftItem->info().lastModified() < rightItem->info().lastModified();
			}
		}

	return true;
}

FILE_SYSTEM_NS_END
