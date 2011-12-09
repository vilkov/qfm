#include "idmrootnoderootitem.h"


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

bool RootNodeRootItem::isFiles()
{
	return false;
}

bool RootNodeRootItem::isEntity()
{
	return false;
}

bool RootNodeRootItem::isProperty()
{
	return false;
}

IDM_PLUGIN_NS_END
