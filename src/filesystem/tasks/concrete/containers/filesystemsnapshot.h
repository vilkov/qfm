#ifndef FILESYSTEMSNAPSHOT_H_
#define FILESYSTEMSNAPSHOT_H_

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include "filesysteminfoitem.h"
#include "../../filesystembasetask.h"
#include "../../../model/items/filesystemnodeitem.h"
#include "../../../interfaces/filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class Snapshot
{
public:
	class List;
	class Files;
	typedef BaseTask::Flags                       Flags;
	typedef QList<QPair<NodeItem *, InfoItem *> > Container;

public:
	IFileInfo::size_type totalSize() const { return m_data->totalSize; }
	const IFileContainer *container() const { return m_data->m_container.data(); }

protected:
	friend class ScanFilesBaseTask;

	Snapshot();
	Snapshot(IFileContainer::Holder &container);
	Snapshot(IFileContainer::Holder &container, Container::size_type reserver);

	void push_back(NodeItem *item, InfoItem *info) { m_data->list.push_back(Container::value_type(item, info)); m_data->totalSize += info->totalSize(); }
	void remove(Container::size_type index) { InfoItem *info = m_data->list.takeAt(index).second; m_data->totalSize -= info->totalSize(); delete info; }

private:
	struct Data : public QSharedData
	{
		Data(IFileContainer::Holder &container);
		Data(IFileContainer::Holder &container, Container::size_type reserver);
		~Data();

		Container list;
		IFileInfo::size_type totalSize;
		IFileContainer::Holder m_container;
	};

private:
	friend class List;
	friend class Files;
	QExplicitlySharedDataPointer<Data> m_data;
};


class Snapshot::List
{
	Q_DISABLE_COPY(List)

public:
	typedef Container::size_type  size_type;
	typedef Container::value_type value_type;

public:
	List(const Snapshot &snapshot) :
		m_data(snapshot.m_data),
		m_list(m_data->list)
	{}

	size_type size() const { return m_list.size(); }
	const value_type &at(size_type index) const { return m_list.at(index); }

private:
	QExplicitlySharedDataPointer<Data> m_data;
	const Container &m_list;
};


class Snapshot::Files
{
	Q_DISABLE_COPY(Files)

public:
	typedef Container::size_type  size_type;
	typedef Container::value_type value_type;

public:
	Files(const Snapshot &snapshot) :
		m_data(snapshot.m_data),
		m_list(m_data->list)
	{}

	IFileInfo::size_type totalSize() const { return m_data->totalSize; }
	const IFileContainer *container() const { return m_data->m_container.data(); }

	size_type size() const { return m_list.size(); }
	const InfoItem *at(size_type index) const { return m_list.at(index).second; }

private:
	QExplicitlySharedDataPointer<Data> m_data;
	const Container &m_list;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMSNAPSHOT_H_ */
