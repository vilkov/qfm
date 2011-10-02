#include "queryconstraintsmodel.h"
#include "items/querygroupconstraintsitem.h"


IDM_PLUGIN_NS_BEGIN

QueryConstraintsModel::QueryConstraintsModel(QObject *parent) :
	IdmModel(parent)
{}

void QueryConstraintsModel::add(const QModelIndex &index)
{
	if (index.isValid() && static_cast<IdmItem*>(index.internalPointer())->isList())
	{
		beginInsertRows(index, static_cast<IdmListItem*>(index.internalPointer())->size(), static_cast<IdmListItem*>(index.internalPointer())->size());
		m_items.push_back(new QueryGroupConstraintsItem());
		endInsertRows();
	}
}

void QueryConstraintsModel::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

IDM_PLUGIN_NS_END
