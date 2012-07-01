#include "defaultnode.h"
#include "../model/items/defaultnoderootitem.h"


DEFAULT_PLUGIN_NS_BEGIN

Node::Node(IFileContainer::Holder &container, FileSystem::Node *parent) :
	BaseNode(container, parent)
{
	DefaultNodeRootItem::Holder item(new DefaultNodeRootItem());
	items().add(item.as<DefaultNodeRootItem>()->label().toString(), item);
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
