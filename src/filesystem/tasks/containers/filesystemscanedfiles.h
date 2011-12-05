#ifndef FILESYSTEMSCANEDFILES_H_
#define FILESYSTEMSCANEDFILES_H_

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include "filesysteminfoitem.h"
#include "../items/filesystemtasknodeitem.h"


FILE_SYSTEM_NS_BEGIN

class ScanedFiles
{
public:
	class List;
	class Files;
	typedef QList<QPair<TaskNodeItem::Base*, InfoItem*> > Container;

public:
	ScanedFiles();
	ScanedFiles(Container::size_type reserver);

	IFile::size_type totalSize() const { return m_data->totalSize; }
	void push_back(TaskNodeItem::Base *item, InfoItem *info) { m_data->list.push_back(Container::value_type(item, info)); m_data->totalSize += info->totalSize(); }
	void remove(Container::size_type index) { InfoItem *info = m_data->list.takeAt(index).second; m_data->totalSize -= info->totalSize(); delete info; }

private:
	struct Data : public QSharedData
	{
		Data();
		~Data();

		Container list;
		IFile::size_type totalSize;
	};

private:
	friend class List;
	friend class Files;
	QExplicitlySharedDataPointer<Data> m_data;
};


class ScanedFiles::List
{
	Q_DISABLE_COPY(List);

public:
	typedef Container::size_type  size_type;
	typedef Container::value_type value_type;

public:
	List(const ScanedFiles &file) :
		m_data(file.m_data),
		m_list(m_data->list)
	{}

	size_type size() const { return m_list.size(); }
	const value_type &at(size_type index) const { return m_list.at(index); }

private:
	QExplicitlySharedDataPointer<Data> m_data;
	const Container &m_list;
};


class ScanedFiles::Files
{
	Q_DISABLE_COPY(Files);

public:
	typedef Container::size_type  size_type;
	typedef Container::value_type value_type;

public:
	Files(const ScanedFiles &file) :
		m_data(file.m_data),
		m_list(m_data->list)
	{}

	IFile::size_type totalSize() const { return m_data->totalSize; }

	size_type size() const { return m_list.size(); }
	const InfoItem *at(size_type index) const { return m_list.at(index).second; }

private:
	QExplicitlySharedDataPointer<Data> m_data;
	const Container &m_list;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMSCANEDFILES_H_ */
