#include "treemodel.h"
#include "items/treelistitem.h"


MODELS_NS_BEGIN

TreeModel::TreeModel(QObject *parent) :
	QAbstractItemModel(parent)
{}

TreeModel::~TreeModel()
{
	qDeleteAll(m_items);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<TreeItem*>(parent.internalPointer())->isList())
			return static_cast<TreeListItem*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<TreeItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<TreeItem*>(parent.internalPointer())->isList())
			return createIndex(row, column, static_cast<TreeListItem*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (TreeItem *parent = static_cast<TreeItem*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<TreeListItem*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

MODELS_NS_END
