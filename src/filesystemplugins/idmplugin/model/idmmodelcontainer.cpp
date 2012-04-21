#include "idmmodelcontainer.h"


IDM_PLUGIN_NS_BEGIN

IdmModelContainer::IdmModelContainer()
{}

IdmModelContainer::~IdmModelContainer()
{
	qDeleteAll(m_container);
}

IdmModelContainer::size_type IdmModelContainer::size() const
{
	return m_container.size();
}

IdmModelContainer::Item *IdmModelContainer::at(size_type index) const
{
	return m_container.at(index);
}

IdmModelContainer::size_type IdmModelContainer::indexOf(Item *item) const
{
	return m_container.indexOf(item);
}

IDM_PLUGIN_NS_END
