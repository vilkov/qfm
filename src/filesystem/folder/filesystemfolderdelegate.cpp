#include "filesystemfolderdelegate.h"
#include "items/filesystementryitem.h"
#include "../tools/filesystemcommontools.h"
#include <QtGui/QApplication>


FILE_SYSTEM_NS_BEGIN

FolderDelegate::FolderDelegate(QAbstractProxyModel *proxy, QObject *parent) :
	TasksNodeDelegate(parent),
    m_proxy(proxy)
{
	Q_ASSERT(m_proxy != 0);
}

void FolderDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		FileSystemBaseItem *entry = static_cast<FileSystemBaseItem*>(m_proxy->mapToSource(index).internalPointer());

		if (!entry->isRootItem() && entry->isInProgress())
		{
			paintProgressInMb(entry, painter, option);
			return;
		}
	}

	Delegate::paintBackgroundLines(painter, option, index);
}

FILE_SYSTEM_NS_END
