#include "artifactsmodel.h"
#include "items/artifactsheaderitem.h"
#include "items/artifactsitem.h"


ArtifactsModel::ArtifactsModel(const QList<QVariant> &header, QObject *parent) :
	Model(parent)
{
    rootItem = new ArtifactsHeaderItem();
    static_cast<ArtifactsHeaderItem*>(rootItem)->setHeader(header);
}

Qt::ItemFlags ArtifactsModel::flags(const QModelIndex &index) const
{
	return static_cast<ArtifactsItem*>(index.internalPointer())->flags(index.column());
}

void ArtifactsModel::remove(ModelItem *item)
{
	ModelItem *parent = item->parent();
	QModelIndex parentIndex = index(0, parent);
	const ModelItem::size_type index = parent->indexOf(item);

	beginRemoveRows(parentIndex, index, index);
	parent->removeChild(index);
	delete item;
	endRemoveRows();
}

void ArtifactsModel::clear()
{
	beginRemoveRows(QModelIndex(), 0, rootItem->size() - 1);
	rootItem->clear();
	endRemoveRows();
}
