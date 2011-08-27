#include "m3uproxymodel.h"
#include "items/m3uentry.h"


M3U_PLUGIN_NS_BEGIN

M3uProxyModel::M3uProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool M3uProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	if (static_cast<M3uItem*>(left.internalPointer())->isRoot())
		return true;
	else
		if (static_cast<M3uItem*>(right.internalPointer())->isRoot())
			return false;
		else
		{
			M3uEntry *leftItem = static_cast<M3uEntry*>(left.internalPointer());
			M3uEntry *rightItem = static_cast<M3uEntry*>(right.internalPointer());

			switch (left.column())
			{
				case 0: return leftItem->title() < rightItem->title();
				case 1: return leftItem->length() < rightItem->length();
			}
		}

	return true;
}

M3U_PLUGIN_NS_END
