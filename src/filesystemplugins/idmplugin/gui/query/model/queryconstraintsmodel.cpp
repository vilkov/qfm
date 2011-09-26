#include "queryconstraintsmodel.h"
#include "items/querygroupconstraintsitem.h"


IDM_PLUGIN_NS_BEGIN

QueryConstraintsModel::QueryConstraintsModel(QObject *parent) :
	IdmModel(parent)
{}

void QueryConstraintsModel::add()
{
//	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
//	m_items.add(name, entity);
//	endInsertRows();
}

void QueryConstraintsModel::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

IDM_PLUGIN_NS_END
