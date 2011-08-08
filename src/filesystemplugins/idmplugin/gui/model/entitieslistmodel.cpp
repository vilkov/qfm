#include "artifactslistmodel.h"
#include "items/artifactslistheaderitem.h"
#include "items/artifactslistitem.h"
#include "../../artifact/artifactvirtual.h"


ArtifactsListModel::ArtifactsListModel(QObject *parent) :
	Model(parent)
{
    QList<QVariant> rootData;
    rootData << tr("Artifact");
    rootItem = new ArtifactsListHeaderItem();
    static_cast<ArtifactsListHeaderItem*>(rootItem)->setHeader(rootData);
}

ArtifactsListModel::~ArtifactsListModel()
{

}

void ArtifactsListModel::appendArtifact(Artifact *artifact)
{
	ModelItem *item = new ArtifactsListItem(artifact);
	m_items[artifact].push_back(item);

	beginInsertRows(QModelIndex(), rootItem->childs().size() - 1, rootItem->childs().size() - 1);
	rootItem->appendChild(item);
	endInsertRows();
}

void ArtifactsListModel::appendArtifact(Artifact *artifact, Artifact *property)
{
	const QList<ModelItem*> items = m_items.value(artifact, QList<ModelItem*>());

	if (property->isVirtual())
		for (QList<ModelItem*>::size_type i = 0; i < items.size(); ++i)
			insertArtifacts(insertArtifact(items[i], property), static_cast<ArtifactVirtual*>(property)->propertys());
	else
		for (QList<ModelItem*>::size_type i = 0; i < items.size(); ++i)
			insertArtifact(items[i], property);
}

void ArtifactsListModel::removeArtifact(Artifact *artifact)
{
	ModelItem *parent;
	ModelItem::size_type childIndex;
	const QList<ModelItem*> items = m_items.value(artifact, QList<ModelItem*>());

	for (QList<ModelItem*>::size_type i = 0; i < items.size(); ++i)
	{
		parent = items[i]->parent();
		childIndex = parent->indexOf(items[i]);
		QModelIndex parentIndex = index(0, parent);

		beginRemoveRows(parentIndex, childIndex, childIndex);
		parent->removeChild(childIndex);
		endRemoveRows();

		delete items[i];
	}

	m_items.remove(artifact);
}

void ArtifactsListModel::removeArtifact(Artifact *artifact, Artifact *property)
{
	const QList<ModelItem*> items = m_items.value(artifact, QList<ModelItem*>());

	for (QList<ModelItem*>::size_type i = 0; i < items.size(); ++i)
	{
		const QList<ModelItem*> &childs = items[i]->childs();

		for (QList<ModelItem*>::size_type q = 0; q < childs.size(); ++q)
			if (static_cast<ArtifactsListItem*>(childs[q])->artifact() == property)
			{
				QModelIndex parentIndex = index(0, items[i]);
				m_items[property].removeAt(m_items[property].indexOf(childs[q]));

				beginRemoveRows(parentIndex, q, q);
				delete childs[q];
				items[i]->removeChild(q);
				endRemoveRows();

				break;
			}
	}
}

void ArtifactsListModel::insertArtifacts(const QList<Artifact*> &artifacts)
{
	QList<ModelItem*> items;
	ModelItem *item;

	for (QList<Artifact*>::size_type i = 0; i < artifacts.size(); ++i)
	{
		item = new ArtifactsListItem(artifacts[i]);
		m_items[artifacts[i]].push_back(item);
		items.push_back(item);

		if (artifacts[i]->isVirtual())
			insertArtifacts(item, static_cast<ArtifactVirtual*>(artifacts[i])->propertys());
	}

	if (!items.isEmpty())
		Model::insertRows(0, items);
}

ModelItem *ArtifactsListModel::insertArtifact(ModelItem *parent, Artifact* artifact)
{
	QModelIndex parentIndex = index(0, parent);
	ModelItem *item = new ArtifactsListItem(artifact);
	m_items[artifact].push_back(item);

	beginInsertRows(parentIndex, parent->childs().size(), parent->childs().size());
	parent->appendChild(item);
	endInsertRows();

	return item;
}

void ArtifactsListModel::insertArtifacts(ModelItem *parent, const QList<Artifact*> &artifacts)
{
	ModelItem *item;

	for (QList<Artifact*>::size_type i = 0; i < artifacts.size(); ++i)
	{
		item = new ArtifactsListItem(artifacts[i]);
		m_items[artifacts[i]].push_back(item);
		parent->appendChild(item);

		if (artifacts[i]->isVirtual())
			insertArtifacts(item, static_cast<ArtifactVirtual*>(artifacts[i])->propertys());
	}
}
