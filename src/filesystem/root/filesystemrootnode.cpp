#include "filesystemrootnode.h"
#include "../filesystempluginsmanager.h"
#include "../folder/filesystemfoldernode.h"
#include "../folder/items/filesystementryitem.h"


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode() :
	Node(m_items)
{}

Node *RootNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	ItemsContainer::size_type index = m_items.indexOf(fileName);

	if (index == ItemsContainer::InvalidIndex)
	{
		m_items.add(createItem(fileName, plugins));
		return m_items.last()->node();
	}
	else
	{
		FileSystemEntryItem *item = static_cast<FileSystemEntryItem*>(m_items[index]);

		if (item->node())
			return item->node();
		else
			if (Node *node = createNode(*item, plugins))
			{
				item->setNode(node);
				return node;
			}
	}

	return 0;
}

void RootNode::nodeRemoved(Node *node)
{
	delete m_items.take(m_items.indexOf(node));
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

FileSystemBaseItem *RootNode::createItem(const QString &fileName, PluginsManager *plugins)
{
	Info info(fileName);
	return new FileSystemEntryItem(info, createNode(info, plugins));
}

FILE_SYSTEM_NS_END
