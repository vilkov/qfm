#include "arcnodeentryitem.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeEntryItem::ArcNodeEntryItem(const QString &fileName, Base *parent) :
	ArcNodeListItem(parent),
	m_fileName(fileName)
{}

QVariant ArcNodeEntryItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_fileName;
	else
		return QVariant();
}

ARC_PLUGIN_NS_END
