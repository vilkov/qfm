#include "entitieslistmodel.h"
#include "items/idmentitieslistmodelitem.h"
#include "../../items/idmentityitem.h"


IDM_PLUGIN_NS_BEGIN

EntitiesListModel::EntitiesListModel(QObject *parent) :
	QAbstractItemModel(parent)
{}

EntitiesListModel::~EntitiesListModel()
{
	qDeleteAll(m_items);
}

int EntitiesListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<IdmItem*>(parent.internalPointer())->isList())
			return static_cast<IdmItemsList*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int EntitiesListModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant EntitiesListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<IdmItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags EntitiesListModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant EntitiesListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}
QModelIndex EntitiesListModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<IdmItem*>(parent.internalPointer())->isList())
			return createIndex(row, column, static_cast<IdmItemsList*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex EntitiesListModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (IdmItem *parent = static_cast<IdmItem*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<IdmItemsList*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

IdmEntity *EntitiesListModel::at(size_type index) const
{
	return static_cast<IdmEntitiesListItem*>(m_items.at(index))->entity();
}

void EntitiesListModel::add(IdmEntity *entity)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(new IdmEntitiesListItem(entity));
	endInsertRows();
}

void EntitiesListModel::remove(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	delete m_items.at(index.row());
	m_items.removeAt(index.row());
	endRemoveRows();
}

IDM_PLUGIN_NS_END
