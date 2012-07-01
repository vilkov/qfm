#include "defaultdelegate.h"
#include "../model/items/defaultnodeitem.h"
#include "../../../filesystem/tools/filesystemcommontools.h"


DEFAULT_PLUGIN_NS_BEGIN

Delegate::Delegate(QAbstractProxyModel *proxy, QObject *parent) :
	TasksNodeDelegate(parent),
    m_proxy(proxy)
{
	Q_ASSERT(m_proxy != 0);
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		NodeItem *entry = static_cast<NodeItem*>(m_proxy->mapToSource(index).internalPointer());

		if (!entry->isRootItem() && entry->isInProgress())
		{
			paintProgressInMb(entry, painter, option);
			return;
		}
	}

	paintBackgroundLines(painter, option, index);
}

DEFAULT_PLUGIN_NS_END
