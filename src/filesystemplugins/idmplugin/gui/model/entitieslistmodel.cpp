#include "entitieslistmodel.h"
#include "../../containeres/menu/idmmenu.h"


FILE_SYSTEM_NS_BEGIN

EntitiesListModel::EntitiesListModel(const IdmItemsList *exsisting, QObject *parent) :
	m_exsisting(exsisting),
	m_items()
{}

int EntitiesListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<IdmItem*>(parent.internalPointer())->isList())
			return static_cast<IdmItemsList*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items->size();
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
    	return m_items->at(index.row())->data(index.column(), role);
}

Qt::ItemFlags EntitiesListModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
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
			return createIndex(row, column, m_items->at(row));
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
				return createIndex(m_items->indexOf(parent), 0, parent);

    return QModelIndex();
}

FILE_SYSTEM_NS_END
