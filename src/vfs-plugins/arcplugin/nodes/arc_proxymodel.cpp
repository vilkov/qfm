#include "arc_proxymodel.h"
#include "items/arc_entrynodeitem.h"

#include <vfs/model/vfs_proxymodel.h>


ARC_PLUGIN_NS_BEGIN

ProxyModel::ProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool ProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<NodeItem *>(left.internalPointer())->isRoot())
		return sortOrder() == Qt::AscendingOrder;
	else
		if (static_cast<NodeItem *>(right.internalPointer())->isRoot())
			return sortOrder() == Qt::DescendingOrder;
		else
			return VFS::ProxyModel::compareByFileNames(static_cast<EntryNodeItem *>(left.internalPointer())->info(), static_cast<EntryNodeItem *>(right.internalPointer())->info());
}

ARC_PLUGIN_NS_END
