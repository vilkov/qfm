#include "defaultfoldernode.h"
#include "model/items/defaultnoderootitem.h"


DEFAULT_PLUGIN_NS_BEGIN

FolderNode::FolderNode(IFileContainer::Holder &container, Node *parent) :
	FolderNodeBase(container, parent)
{
	DefaultNodeRootItem *item = new DefaultNodeRootItem();
	items().add(item->label().toString(), item);
}

Snapshot FolderNode::updateFilesList() const
{
	Snapshot::Files files(container().data());

	for (Container::size_type i = 1, size = items().size(); i < size; ++i)
		files.add(items()[i]->info()->fileName(), items()[i]);

	return files;
}

DEFAULT_PLUGIN_NS_END
