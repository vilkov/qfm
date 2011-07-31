#include "idmproxymodel.h"


FILE_SYSTEM_NS_BEGIN

IdmProxyModel::IdmProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool IdmProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	return false;
}

FILE_SYSTEM_NS_END
