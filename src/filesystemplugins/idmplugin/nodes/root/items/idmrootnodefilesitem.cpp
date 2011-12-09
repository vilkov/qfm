#include "idmrootnodefilesitem.h"


IDM_PLUGIN_NS_BEGIN

RootNodeFilesItem::RootNodeFilesItem() :
	RootNodeItem(0),
	m_label(tr("File system...")),
	m_node(0)
{}

QVariant RootNodeFilesItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_label;
	else
		return QVariant();
}

bool RootNodeFilesItem::isRoot()
{
	return false;
}

bool RootNodeFilesItem::isFiles()
{
	return true;
}

bool RootNodeFilesItem::isEntity()
{
	return false;
}

bool RootNodeFilesItem::isProperty()
{
	return false;
}

IDM_PLUGIN_NS_END
