#include "filesystemitemscontainer.h"


FILE_SYSTEM_NS_BEGIN

ItemsContainer::ItemsContainer()
{}

ItemsContainer::~ItemsContainer()
{
	for (List::size_type i = 0, size = m_container.size(); i < size; ++i)
		delete m_container.at(i);
}

ItemsContainer::size_type ItemsContainer::size() const
{
	return m_container.size();
}

ItemsContainer::Item *ItemsContainer::at(size_type index) const
{
	return m_container.at(index);
}

ItemsContainer::size_type ItemsContainer::indexOf(Item *item) const
{
	for (List::size_type i = 0, size = m_container.size(); i < size; ++i)
		if (static_cast<Item*>(m_container.at(i)) == item)
			return i;

	return InvalidIndex;
}

FILE_SYSTEM_NS_END
