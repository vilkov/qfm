#include "idmproxymodel.h"


IDM_PLUGIN_NS_BEGIN

IdmProxyModel::IdmProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool IdmProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	return false;
}

IDM_PLUGIN_NS_END
