#include "filesystemrootnode.h"
#include "filesystempluginsmanager.h"
#include "folder/filesystemfoldernode.h"
#include "folder/items/filesystemfoldernodeentry.h"


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode(PluginsManager *plugins) :
	FolderNodeBase(),
	m_plugins(plugins)
{
	Q_ASSERT(m_plugins != 0);
}

void RootNode::view(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins)
{
	Node *node;
	Values::Value *value;
	Values::size_type index = items().indexOf(*path);

	if (index == Values::InvalidIndex)
	{
		items().add(createNode(*path, plugins, node));
		value = &items().last();
	}
	else
	{
		value = &items()[index];

		if (value->node)
			node = value->node;
		else
			value->node = node = createNode(*(value->item), plugins);
	}

	if (node)
		if ((++path).atEnd())
			node->view(nodeView, QModelIndex());
		else
			node->view(nodeView, path, plugins);
}

void RootNode::view(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins)
{
	Path path(absoluteFilePath);

	if (path.isAbsolute())
		view(nodeView, path.begin(), plugins);
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

RootNode::Values::Value RootNode::createNode(const QString &fileName, PluginsManager *plugins, Node *&node) const
{
	Info info(fileName);
	return Values::Value(new FolderNodeEntry(info), node = createNode(info, plugins));
}

FILE_SYSTEM_NS_END
