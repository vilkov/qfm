#include "idmmodel.h"


IDM_PLUGIN_NS_BEGIN

IdmModel::IdmModel(QObject *parent) :
	Model(m_container, parent),
	m_items(m_container.m_container)
{}

IdmModel::Container::Container()
{}

IdmModel::Container::~Container()
{
	qDeleteAll(m_container);
}

IdmModel::Container::size_type IdmModel::Container::size() const
{
	return m_container.size();
}

IdmModel::Container::Item *IdmModel::Container::at(size_type index) const
{
	return m_container.at(index);
}

IdmModel::Container::size_type IdmModel::Container::indexOf(Item *item) const
{
	return m_container.indexOf(item);
}

IDM_PLUGIN_NS_END
