#include "arcnodeproxymodel.h"
#include "items/arcnodeentryitem.h"
#include "../../../filesystem/filesystemproxymodel.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeProxyModel::ArcNodeProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool ArcNodeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<ArcNodeItem *>(left.internalPointer())->isRoot())
		return sortOrder() == Qt::AscendingOrder;
	else
		if (static_cast<ArcNodeItem *>(right.internalPointer())->isRoot())
			return sortOrder() == Qt::DescendingOrder;
		else
			return ProxyModel::compareByFileNames(static_cast<ArcNodeEntryItem *>(left.internalPointer())->info().data(), static_cast<ArcNodeEntryItem *>(right.internalPointer())->info().data());
}

ARC_PLUGIN_NS_END
