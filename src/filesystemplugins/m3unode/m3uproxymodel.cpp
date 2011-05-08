#include "m3uproxymodel.h"


M3uProxyModel::M3uProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool M3uProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	return true;
}
