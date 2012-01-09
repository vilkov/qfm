#include "arcnodeproxymodel.h"
#include "items/arcnodedirentryitem.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeProxyModel::ArcNodeProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool ArcNodeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<ArcNodeItem::Base*>(left.internalPointer())->isList())
		if (static_cast<ArcNodeItem::Base*>(right.internalPointer())->isList())
			return static_cast<ArcNodeDirEntryItem*>(left.internalPointer())->fileName() < static_cast<ArcNodeDirEntryItem*>(right.internalPointer())->fileName();
		else
			return static_cast<ArcNodeItem*>(right.internalPointer())->isRoot();
	else
		if (static_cast<ArcNodeItem::Base*>(right.internalPointer())->isList())
			return !static_cast<ArcNodeItem*>(left.internalPointer())->isRoot();
		else
			return static_cast<ArcNodeEntryItem*>(left.internalPointer())->fileName() < static_cast<ArcNodeEntryItem*>(right.internalPointer())->fileName();
}

ARC_PLUGIN_NS_END
