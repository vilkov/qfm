#include "filesystemitemscontainer.h"


FILE_SYSTEM_NS_BEGIN

ItemsContainer::ItemsContainer()
{}

ItemsContainer::~ItemsContainer()
{
	for (Container::size_type i = 0, size = m_items.size(); i < size; ++i)
		delete m_items.at(i);
}

ItemsContainer::size_type ItemsContainer::size() const
{
	return m_items.size();
}

ItemsContainer::Item *ItemsContainer::at(size_type index) const
{
	return m_items.at(index);
}

ItemsContainer::size_type ItemsContainer::indexOf(Item *item) const
{
	for (Container::size_type i = 0, size = m_items.size(); i < size; ++i)
		if (m_items.at(i) == item)
			return i;

	return Container::InvalidIndex;
}

FILE_SYSTEM_NS_END
