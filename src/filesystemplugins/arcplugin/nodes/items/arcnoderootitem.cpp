#include "arcnoderootitem.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeRootItem::ArcNodeRootItem(Base *parent) :
	ArcNodeItem(parent),
	m_label(QString::fromLatin1(".."))
{}

QVariant ArcNodeRootItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_label;
	else
		return QVariant();
}

ARC_PLUGIN_NS_END
