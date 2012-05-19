#include "filesystemsnapshot.h"


FILE_SYSTEM_NS_BEGIN

Snapshot::Snapshot()
{}

Snapshot::Snapshot(const Files &files) :
	m_data(files.m_data)
{}

Snapshot::Snapshot(const Updates &updates) :
	m_data(updates.m_data)
{}

Snapshot::Snapshot(const IFileContainer *container) :
	m_data(new Data(container))
{}

Snapshot::Data::Data(const IFileContainer *container) :
	container(container)
{}

Snapshot::Data::~Data()
{
	for (Container::iterator it = map.begin(), end = map.end(); it != end; it = map.erase(it))
		delete (*it).second;
}

FILE_SYSTEM_NS_END
