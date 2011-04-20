#include "filesystemproxymodel.h"
#include "items/filesystementry.h"


FileSystemProxyModel::FileSystemProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool FileSystemProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (left.column() == 0)
		if (static_cast<FileSystemItem*>(left.internalPointer())->isRoot())
			return true;
		else
			if (static_cast<FileSystemItem*>(right.internalPointer())->isRoot())
				return false;
			else
			{
				FileSystemEntry *leftItem = static_cast<FileSystemEntry*>(left.internalPointer());
				FileSystemEntry *rightItem = static_cast<FileSystemEntry*>(right.internalPointer());

				if (leftItem->fileInfo().isDir())
					if (rightItem->fileInfo().isDir())
						return leftItem->fileInfo().fileName() < rightItem->fileInfo().fileName();
					else
						return true;
				else
					if (rightItem->fileInfo().isDir())
						return false;
					else
						return leftItem->fileInfo().fileName() < rightItem->fileInfo().fileName();
			}

	return true;
}
