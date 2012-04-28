#include "filesystemsnapshot.h"


FILE_SYSTEM_NS_BEGIN

Snapshot::Snapshot()
{}

Snapshot::Snapshot(const Files &files) :
	m_data(files.m_data)
{}

Snapshot::List Snapshot::list() const
{
	return List(m_data);
}

Snapshot::Updates Snapshot::takeUpdates()
{
	BaseList::Container res;

	for (Container::iterator it = m_data->map.begin(), end = m_data->map.end(); it != end;)
		if ((*it).second)
		{
			if ((*it).second->isValid())
				res.push_back((*it));
			else
				delete (*it).second;

			it = m_data->map.erase(it);
		}
		else
			++it;

	return res;
}

Snapshot::Data::Data(const IFileContainer *container) :
	totalSize(0),
	container(container)
{}

Snapshot::Data::~Data()
{
	for (Container::iterator it = map.begin(), end = map.end(); it != end; it = map.erase(it))
		delete (*it).second;
}

FILE_SYSTEM_NS_END
