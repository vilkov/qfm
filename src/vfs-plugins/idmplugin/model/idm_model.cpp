#include "idm_model.h"


IDM_PLUGIN_NS_BEGIN

Model::Model(QObject *parent) :
	::Tools::Models::Tree::Model(m_container, parent),
	m_items(m_container.m_container)
{}

Model::Container::Container()
{}

Model::Container::~Container()
{
	qDeleteAll(m_container);
}

Model::Container::size_type Model::Container::size() const
{
	return m_container.size();
}

Model::Container::Item *Model::Container::at(size_type index) const
{
	return m_container.at(index);
}

Model::Container::size_type Model::Container::indexOf(Item *item) const
{
	return m_container.indexOf(item);
}

IDM_PLUGIN_NS_END
