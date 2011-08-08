#include "artifactslistitem.h"


ArtifactsListItem::ArtifactsListItem(Artifact *artifact, ModelItem *parent) :
	ModelItem(parent),
	m_artifact(artifact)
{}

ArtifactsListItem::~ArtifactsListItem()
{
	clear();
}

QVariant ArtifactsListItem::data(int column, int role) const
{
	if (role == Qt::DisplayRole)
		return m_artifact->name();

	return QVariant();
}
