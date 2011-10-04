#include "idmnodeproxymodel.h"


IDM_PLUGIN_NS_BEGIN

IdmNodeProxyModel::IdmNodeProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool IdmNodeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	return false;
}

IDM_PLUGIN_NS_END
