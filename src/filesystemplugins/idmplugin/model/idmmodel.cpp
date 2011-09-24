#include "idmmodel.h"
#include "items/idmlistitem.h"


IDM_PLUGIN_NS_BEGIN

IdmModel::IdmModel(QObject *parent) :
	QAbstractItemModel(parent)
{}

IdmModel::~IdmModel()
{
	qDeleteAll(m_items);
}

int IdmModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<IdmItem*>(parent.internalPointer())->isList())
			return static_cast<IdmListItem*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int IdmModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant IdmModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<IdmItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags IdmModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant IdmModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex IdmModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<IdmItem*>(parent.internalPointer())->isList())
			return createIndex(row, column, static_cast<IdmListItem*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex IdmModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (IdmItem *parent = static_cast<IdmItem*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<IdmListItem*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

IDM_PLUGIN_NS_END
