#include "arcnodedirentryitem.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeDirEntryItem::ArcNodeDirEntryItem(const QString &fileName, Base *parent) :
	ArcNodeListItem(parent),
	m_fileName(fileName)
{}

QVariant ArcNodeDirEntryItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_fileName;
	else
		return QVariant();
}

bool ArcNodeDirEntryItem::isRoot()
{
	return false;
}

bool ArcNodeDirEntryItem::isDir()
{
	return true;
}

ARC_PLUGIN_NS_END
