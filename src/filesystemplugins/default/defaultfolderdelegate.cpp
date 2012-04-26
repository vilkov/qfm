#include "defaultfolderdelegate.h"
#include "model/items/defaultnodeitem.h"
#include "../../filesystem/tools/filesystemcommontools.h"


DEFAULT_PLUGIN_NS_BEGIN

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
		DefaultNodeItem *entry = static_cast<DefaultNodeItem*>(m_proxy->mapToSource(index).internalPointer());

		if (!entry->isRootItem() && entry->isInProgress())
		{
			paintProgressInMb(entry, painter, option);
			return;
		}
	}

	Delegate::paintBackgroundLines(painter, option, index);
}

DEFAULT_PLUGIN_NS_END
