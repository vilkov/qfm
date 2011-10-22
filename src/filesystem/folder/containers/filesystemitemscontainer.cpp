#include "filesystemitemscontainer.h"


FILE_SYSTEM_NS_BEGIN

ItemsContainer::ItemsContainer()
{}

ItemsContainer::size_type ItemsContainer::size() const
{
	return m_container.size();
}

ItemsContainer::Item *ItemsContainer::at(size_type index) const
{
	return m_container.at(index).item;
}

ItemsContainer::size_type ItemsContainer::indexOf(Item *item) const
{
	return m_container.indexOf(static_cast<FileSystemBaseItem*>(item));
}

FILE_SYSTEM_NS_END
