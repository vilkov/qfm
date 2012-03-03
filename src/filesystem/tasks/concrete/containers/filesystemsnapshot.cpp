#include "filesystemsnapshot.h"


FILE_SYSTEM_NS_BEGIN

Snapshot::Snapshot() :
	m_data()
{}

Snapshot::Snapshot(IFileContainer::Holder &container) :
	m_data(new Data(container))
{}

Snapshot::Snapshot(IFileContainer::Holder &container, Container::size_type reserver) :
	m_data(new Data(container, reserver))
{}

Snapshot::Data::Data(IFileContainer::Holder &container) :
	totalSize(0),
	m_container(container.take())
{}

Snapshot::Data::Data(IFileContainer::Holder &container, Container::size_type reserver) :
	totalSize(0),
	m_container(container.take())
{
	list.reserve(reserver);
}

Snapshot::Data::~Data()
{
	for (List::size_type i = 0, size = list.size(); i < size; ++i)
		delete list.at(i).second;
}

FILE_SYSTEM_NS_END
