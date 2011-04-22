#include "filesystemproxymodel.h"
#include "items/filesystementry.h"
#include <QtCore/QDateTime>


FileSystemProxyModel::FileSystemProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool FileSystemProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<FileSystemItem*>(left.internalPointer())->isRoot())
		return true;
	else
		if (static_cast<FileSystemItem*>(right.internalPointer())->isRoot())
			return false;
		else
		{
			FileSystemEntry *leftItem = static_cast<FileSystemEntry*>(left.internalPointer());
			FileSystemEntry *rightItem = static_cast<FileSystemEntry*>(right.internalPointer());

			switch (left.column())
			{
				case 0:
				{
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

					break;
				}
				case 1:
				{
					if (leftItem->fileInfo().isDir())
						if (rightItem->fileInfo().isDir())
							return leftItem->totalSize().toULongLong() < rightItem->totalSize().toULongLong();
						else
							return true;
					else
						if (rightItem->fileInfo().isDir())
							return false;
						else
							return leftItem->fileInfo().size() < rightItem->fileInfo().size();

					break;
				}
				case 2:
				{
					if (leftItem->fileInfo().isDir())
						if (rightItem->fileInfo().isDir())
							return leftItem->fileInfo().lastModified() < rightItem->fileInfo().lastModified();
						else
							return true;
					else
						if (rightItem->fileInfo().isDir())
							return false;
						else
							return leftItem->fileInfo().lastModified() < rightItem->fileInfo().lastModified();

					break;
				}
			}
		}

	return true;
}
