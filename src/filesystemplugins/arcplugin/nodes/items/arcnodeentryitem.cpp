#include "arcnodeentryitem.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeEntryItem::ArcNodeEntryItem(const QString &fileName, Base *parent) :
	ArcNodeItem(parent),
	m_fileName(fileName)
{}

QVariant ArcNodeEntryItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_fileName;
	else
		return QVariant();
}

bool ArcNodeEntryItem::isRoot()
{
	return false;
}

bool ArcNodeEntryItem::isDir()
{
	return false;
}

ARC_PLUGIN_NS_END
