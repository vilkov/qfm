#include "m3ucontainer.h"


M3U_PLUGIN_NS_BEGIN

M3uContainer::M3uContainer()
{}

M3uContainer::~M3uContainer()
{
	qDeleteAll(m_container);
}

M3uContainer::size_type M3uContainer::size() const
{
	return m_container.size();
}

M3uContainer::Item *M3uContainer::at(size_type index) const
{
	return m_container.at(index);
}

M3uContainer::size_type M3uContainer::indexOf(Item *item) const
{
	return -1;//m_items.indexOf(item);
}

M3U_PLUGIN_NS_END
