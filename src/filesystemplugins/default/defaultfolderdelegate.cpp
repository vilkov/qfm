#include "defaultfolderdelegate.h"
#include "items/filesystemfolderitem.h"
#include "../tools/filesystemcommontools.h"


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
		FolderBaseItem *entry = static_cast<FolderBaseItem*>(m_proxy->mapToSource(index).internalPointer());

		if (!entry->isRootItem() && entry->isInProgress())
		{
			paintProgressInMb(entry, painter, option);
			return;
		}
	}

	Delegate::paintBackgroundLines(painter, option, index);
}

FILE_SYSTEM_NS_END
