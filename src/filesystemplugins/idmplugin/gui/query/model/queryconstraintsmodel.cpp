#include "queryconstraintsmodel.h"
#include "items/groupconstraintsitem.h"


IDM_PLUGIN_NS_BEGIN

QueryConstraintsModel::QueryConstraintsModel(QObject *parent) :
	QAbstractItemModel(parent)
{}

QueryConstraintsModel::~QueryConstraintsModel()
{
	qDeleteAll(m_items);
}

int QueryConstraintsModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<QueryConstraintsModelItem*>(parent.internalPointer())->isGroup())
			return static_cast<GroupConstraintsItem*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int QueryConstraintsModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant QueryConstraintsModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<QueryConstraintsModelItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags QueryConstraintsModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant QueryConstraintsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex QueryConstraintsModel::index(int row, int column, const QModelIndex &parent) const
{
//	if (hasIndex(row, column, parent))
//		if (parent.isValid() && static_cast<QueryConstraintsModelItem*>(parent.internalPointer())->isGroup())
//			return createIndex(row, column, static_cast<GroupConstraintsItem*>(parent.internalPointer())->at(row));
//		else
//			return createIndex(row, column, m_items.at(row));
//    else
        return QModelIndex();
}

QModelIndex QueryConstraintsModel::parent(const QModelIndex &child) const
{
//    if (child.isValid())
//		if (QueryConstraintsModelItem *parent = static_cast<QueryConstraintsModelItem*>(child.internalPointer())->parent())
//			if (parent->parent())
//				return createIndex(static_cast<IdmItemsList*>(parent->parent())->indexOf(parent), 0, parent);
//			else
//				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

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
