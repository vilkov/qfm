#include "filesystemrootnode.h"
#include "filesystempluginsmanager.h"
#include "folder/filesystemfoldernode.h"
#include "folder/items/filesystemfoldernodeentry.h"


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode() :
	Node()
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

Node *RootNode::createNode(const Info &info, PluginsManager *plugins) const
{
	if (Node *res = plugins->node(&info, (RootNode*)this))
		return res;
	else
		if (info.isDir())
			return new FolderNode(info, (RootNode*)this);
		else
			return 0;
}

Values::Value RootNode::createNode(const QString &fileName, PluginsManager *plugins) const
{
	Info info(fileName);
	return Values::Value(new FolderNodeEntry(info), createNode(info, plugins));
}

FILE_SYSTEM_NS_END
