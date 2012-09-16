#include "idm_rootnoderootitem.h"


IDM_PLUGIN_NS_BEGIN

RootNodeRootItem::RootNodeRootItem() :
	RootNodeItem(0),
	m_label(QString::fromLatin1(".."))
{}

QVariant RootNodeRootItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_label;
	else
		return QVariant();
}

bool RootNodeRootItem::isRoot()
{
	return true;
}

IDM_PLUGIN_NS_END
