#include "defaultnode.h"
#include "../model/items/defaultrootnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

Node::Node(IFileContainer::Holder &container, ::VFS::Node *parent) :
	BaseNode(container, parent)
{
	RootNodeItem::Holder item(new RootNodeItem());
	items().add(item.as<RootNodeItem>()->label().toString(), item);
}

QModelIndex Node::rootIndex() const
{
	return proxy().mapFromSource(createIndex(0, 0, items()[0].data()));
}

Snapshot Node::updateFilesList() const
{
	Snapshot::Files files(container().data());

	for (Container::size_type i = 1, size = items().size(); i < size; ++i)
		files.add(items()[i].as<NodeItem>()->info()->fileName(), items()[i]);

	return files;
}

DEFAULT_PLUGIN_NS_END
