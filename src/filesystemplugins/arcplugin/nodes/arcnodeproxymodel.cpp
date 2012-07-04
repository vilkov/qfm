#include "arcnodeproxymodel.h"
#include "items/arcentrynodeitem.h"
#include "../../../filesystem/filesystemproxymodel.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeProxyModel::ArcNodeProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool ArcNodeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<NodeItem *>(left.internalPointer())->isRoot())
		return sortOrder() == Qt::AscendingOrder;
	else
		if (static_cast<NodeItem *>(right.internalPointer())->isRoot())
			return sortOrder() == Qt::DescendingOrder;
		else
			return ProxyModel::compareByFileNames(static_cast<EntryNodeItem *>(left.internalPointer())->info().data(), static_cast<EntryNodeItem *>(right.internalPointer())->info().data());
}

ARC_PLUGIN_NS_END
