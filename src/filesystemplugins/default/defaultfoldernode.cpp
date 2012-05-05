#include "defaultfoldernode.h"
#include "model/items/defaultnoderootitem.h"


DEFAULT_PLUGIN_NS_BEGIN

FolderNode::FolderNode(IFileContainer::Holder &container, Node *parent) :
	FolderNodeBase(container, parent)
{
	DefaultNodeRootItem::Holder item(new DefaultNodeRootItem());
	items().add(item.as<DefaultNodeRootItem>()->label().toString(), item);
}

QModelIndex FolderNode::rootIndex() const
{
	return proxy().mapFromSource(createIndex(0, 0, items()[0].data()));
}

Snapshot FolderNode::updateFilesList() const
{
	Snapshot::Files files(container().data());

	for (Container::size_type i = 1, size = items().size(); i < size; ++i)
		files.add(items()[i].as<DefaultNodeItem>()->info()->fileName(), items()[i]);

	return files;
}

DEFAULT_PLUGIN_NS_END
