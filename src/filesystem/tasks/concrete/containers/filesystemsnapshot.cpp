#include "filesystemsnapshot.h"


FILE_SYSTEM_NS_BEGIN

Snapshot::Snapshot() :
	m_data()
{}

Snapshot::Snapshot(const IFileContainer *container) :
	m_data(new Data(container))
{}

Snapshot::Snapshot(const IFileContainer *container, Container::size_type reserver) :
	m_data(new Data(container, reserver))
{}

Snapshot::Data::Data(const IFileContainer *container) :
	totalSize(0),
	m_container(container)
{}

Snapshot::Data::Data(const IFileContainer *container, Container::size_type reserver) :
	totalSize(0),
	m_container(container)
{
	list.reserve(reserver);
}

Snapshot::Data::~Data()
{
	for (List::size_type i = 0, size = list.size(); i < size; ++i)
		delete list.at(i).second;
}

FILE_SYSTEM_NS_END
