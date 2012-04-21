#include "filesystemfoldernode.h"
#include "items/filesystemrootfolderitem.h"


FILE_SYSTEM_NS_BEGIN

FolderNode::FolderNode(IFileContainer::Holder &container, Node *parent) :
	RootFolderNode(container, parent)
{
	m_items.add(new RootFolderItem());
}

QModelIndex FolderNode::rootIndex() const
{
	return m_proxy.mapFromSource(createIndex(0, 0, m_items[0]));
}

UpdatesList::Changes FolderNode::updateFilesMap() const
{
	UpdatesList::Changes changes;

	for (ItemsContainer::size_type i = 1, size = m_items.size(); i < size; ++i)
		changes.insert(m_items[i]->info().fileName(), m_items[i]->info());

	return changes;
}

FILE_SYSTEM_NS_END
