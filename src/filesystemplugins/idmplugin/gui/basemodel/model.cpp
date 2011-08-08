#include "model.h"


Model::Model(QObject *parent) :
	QAbstractItemModel(parent),
	rootItem(0)
{}

Model::~Model()
{
	delete rootItem;
}

int Model::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<ModelItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    	return QVariant();

    return static_cast<ModelItem*>(index.internalPointer())->data(index.column(), role);
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
		return rootItem->data(section, role);

	return QVariant();
}

QModelIndex Model::index(int row, int column, ModelItem *parentItem) const
{
    ModelItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ModelItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelItem*>(parent.internalPointer());

    return index(row, column, parentItem);
}

QModelIndex Model::index(int column, ModelItem *item) const
{
    if (item && item != rootItem)
        return createIndex(item->row(), column, item);
    else
        return QModelIndex();
}

QModelIndex Model::parent(const QModelIndex &index) const
{
    if (index.isValid())
    {
		ModelItem *childItem = static_cast<ModelItem*>(index.internalPointer());
		ModelItem *parentItem = childItem->parent();

		if (parentItem && parentItem != rootItem)
			return createIndex(parentItem->row(), 0, parentItem);
    }

    return QModelIndex();
}

int Model::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

	ModelItem *parentItem;

	if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelItem*>(parent.internalPointer());

    return parentItem->size();
}

void Model::insertRows(ModelItem *parent, const QList<ModelItem*> &data)
{
	QModelIndex parentIndex = index(0, parent);

	beginInsertRows(parentIndex, 0, data.size() - 1);

	if (parent)
		for (int i = 0; i < data.size(); ++i)
			parent->appendChild(data[i]);
	else
		for (int i = 0; i < data.size(); ++i)
			rootItem->appendChild(data[i]);

	endInsertRows();
}

void Model::appendRows(ModelItem *parent, const QList<ModelItem*> &data)
{
	QModelIndex parentIndex = index(0, parent);
	qint32 i = rowCount(parentIndex);

	beginInsertRows(parentIndex, i, i + data.size() - 1);
	for (QList<ModelItem*>::size_type i = 0; i < data.size(); ++i)
		rootItem->appendChild(data[i]);
	endInsertRows();
}

void Model::removeRows(ModelItem *item, const bool isParent)
{
	if (isParent)
	{
		if (!item->isEmpty())
		{
			QModelIndex parentIndex = index(0, item);

			beginRemoveRows(parentIndex, 0, item->size() - 1);
			item->clear();
			endRemoveRows();
		}
	}
	else
	{
		ModelItem *parent = item->parent();
		QModelIndex parentIndex = index(0, parent);

		beginRemoveRows(parentIndex, parent->indexOf(item), parent->indexOf(item));
		parent->removeChild(item);
		endRemoveRows();
	}
}

void Model::update(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	emit dataChanged(topLeft, bottomRight);
}
