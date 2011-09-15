#include "valuelistmodel.h"


IDM_PLUGIN_NS_BEGIN

ValueListModel::ValueListModel(const QueryContext &context, QObject *parent) :
	QAbstractItemModel(parent),
	m_context(context)
{

}

int ValueListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<IdmItem*>(parent.internalPointer())->isList())
			return static_cast<IdmItemsList*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int ValueListModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant ValueListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<IdmItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags ValueListModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ValueListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex ValueListModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<IdmItem*>(parent.internalPointer())->isList())
			return createIndex(row, column, static_cast<IdmItemsList*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex ValueListModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (IdmItem *parent = static_cast<IdmItem*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<IdmItemsList*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

void ValueListModel::add(IdmEntity *entity)
{
//	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
//	m_items.add(entity);
//	endInsertRows();
}

void ValueListModel::add(const QModelIndex &index, const QVariant &value)
{
	ValueListItem *item = static_cast<ValueListItem*>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size());
	m_items.add(item, value);
	endInsertRows();
}

void ValueListModel::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

IDM_PLUGIN_NS_END
