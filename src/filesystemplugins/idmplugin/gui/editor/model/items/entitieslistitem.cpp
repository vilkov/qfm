#include "artifactsvirtualitem.h"


ArtifactsVirtualItem::ArtifactsVirtualItem(const qint32 &id, const QList<ArtifactsColumn> &columns, ModelItem *parent) :
	ArtifactsItem(id, parent),
	m_columns(columns)
{}

QVariant ArtifactsVirtualItem::data(int column, int role) const
{
	if (role == Qt::DisplayRole)
		return m_columns[column].data();

	return QVariant();
}

Qt::ItemFlags ArtifactsVirtualItem::flags(int column) const
{
	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
