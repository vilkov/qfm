#include "valuestreemodel.h"


IDM_PLUGIN_NS_BEGIN

ValuesTreeModel::ValuesTreeModel(IdmEntity *entity, QObject *parent) :
	QAbstractItemModel(parent)
{
	for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
		m_items.add(entity->at(i).entity, entity->at(i).name);
}

ValuesTreeModel::~ValuesTreeModel()
{}

int ValuesTreeModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<IdmItem*>(parent.internalPointer())->isList())
			return static_cast<IdmItemsList*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int ValuesTreeModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant ValuesTreeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<IdmItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags ValuesTreeModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ValuesTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex ValuesTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<IdmItem*>(parent.internalPointer())->isList())
			return createIndex(row, column, static_cast<IdmItemsList*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex ValuesTreeModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (IdmItem *parent = static_cast<IdmItem*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<IdmItemsList*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

void ValuesTreeModel::add(IdmEntity *entity)
{
//	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
//	m_items.add(entity);
//	endInsertRows();
}

void ValuesTreeModel::add(const QModelIndex &index, const QVariant &value)
{
	ValuesTreeItem *item = static_cast<ValuesTreeItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size());
	m_items.add(item, value);
	endInsertRows();
}

void ValuesTreeModel::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

IDM_PLUGIN_NS_END
