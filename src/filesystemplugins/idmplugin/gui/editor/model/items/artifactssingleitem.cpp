#include "artifactssingleitem.h"


ArtifactsSingleItem::ArtifactsSingleItem(const qint32 &id, QVariant &data, ModelItem *parent) :
	ArtifactsItem(id, parent),
	m_data(data)
{}

QVariant ArtifactsSingleItem::data(int column, int role) const
{
	Q_UNUSED(column)

	if (role == Qt::DisplayRole || role == Qt::EditRole)
		return m_data;

	return QVariant();
}

Qt::ItemFlags ArtifactsSingleItem::flags(int column) const
{
	Q_UNUSED(column)

	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
