#include "filesystemrootnode.h"
#include "../filesystempluginsmanager.h"
#include "../folder/filesystemfoldernode.h"
#include "../folder/items/filesystementryitem.h"


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode() :
	Node(m_container),
	m_items(m_container.m_container)
{}

Node *RootNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	Values::size_type index = m_items.indexOf(fileName);

	if (index == Values::InvalidIndex)
	{
		m_items.add(createNode(fileName, plugins));
		return m_items.last().node;
	}
	else
	{
		Values::Value &value = m_items[index];

		if (value.node || (value.node = createNode(*(value.item), plugins)))
			return value.node;
	}

	return 0;
}

void RootNode::nodeRemoved(Node *node)
{
	delete m_items.take(m_items.indexOf(node)).item;
}

Node *RootNode::createNode(const Info &info, PluginsManager *plugins)
{
	if (Node *res = plugins->node(&info, this))
		return res;
	else
		if (info.isDir())
			return new FolderNode(info, this);
		else
			return 0;
}

Values::Value RootNode::createNode(const QString &fileName, PluginsManager *plugins)
{
	Info info(fileName);
	return Values::Value(new FileSystemEntryItem(info), createNode(info, plugins));
}

FILE_SYSTEM_NS_END
