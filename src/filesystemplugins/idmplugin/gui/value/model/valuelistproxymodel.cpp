#include "valuelistproxymodel.h"
#include "valuelistmodel.h"


IDM_PLUGIN_NS_BEGIN

ValueListProxyModel::ValueListProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

void ValueListProxyModel::setFilter(const QString &filter)
{
	m_filter = filter;
	invalidateFilter();
}

bool ValueListProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	if (m_filter.isEmpty() || sourceParent.isValid())
		return true;
	else
		return static_cast<ValueListModel *>(sourceModel())->at(sourceRow)->value().toString().contains(m_filter, Qt::CaseInsensitive);
}

bool ValueListProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	return
		static_cast<ValueListModel *>(sourceModel())->at(left.row())->value().toString() <
		static_cast<ValueListModel *>(sourceModel())->at(right.row())->value().toString();
}

IDM_PLUGIN_NS_END
