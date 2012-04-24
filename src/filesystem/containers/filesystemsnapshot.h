#ifndef FILESYSTEMSNAPSHOT_H_
#define FILESYSTEMSNAPSHOT_H_

#include <QtCore/QMap>
#include <QtCore/QSharedData>
#include "filesystemwrappednodeitem.h"
#include "../model/items/filesystemnodeitem.h"


FILE_SYSTEM_NS_BEGIN

class Snapshot
{
public:
	class BaseList;
	class List;
	class Files;
	class Updates;

	typedef QPair<NodeItem *, WrappedNodeItem *> Pair;
	typedef QMap<QString, Pair>                  Container;

public:
	Snapshot();
	Snapshot(const Files &files);

	List list() const;
	Updates takeUpdates();

	const IFileContainer *container() const { return m_data->container; }

	NodeItem *exists(const QString &fileName) const
	{
		return m_data->map.value(fileName).first;
	}

	void insert(const QString &fileName, WrappedNodeItem *item) const
	{
		Pair &pair = m_data->map[fileName];
		delete pair.second;
		pair.second = item;
		m_data->totalSize += item->totalSize();
	}

	void remove(const QString &fileName)
	{
		Q_ASSERT(m_data->map.value(fileName).second);
		Pair pair = m_data->map.take(fileName);
		m_data->totalSize -= pair.second->totalSize();
		delete pair.second;
	}

private:
	friend class Files;
	friend class Update;

	struct Data : public QSharedData
	{
		Data(const IFileContainer *container);
		~Data();

		Container map;
		IFileInfo::size_type totalSize;
		const IFileContainer *container;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
};


class Snapshot::BaseList
{
public:
	typedef QList<Pair>               Container;
	typedef Container::iterator       iterator;
	typedef Container::const_iterator const_iterator;

public:
	bool isEmpty() const { return m_list.isEmpty(); }

	bool isAdded(const_iterator i) const { return (*i).first == NULL; }
	bool isUpdated(const_iterator i) const { return (*i).second != NULL && (*i).second->isValid(); }
	bool isRemoved(const_iterator i) const { return (*i).second == NULL; }

	const_iterator begin() const { return m_list.begin(); }
	iterator begin() { return m_list.begin(); }

	const_iterator end() const { return m_list.end(); }
	iterator end() { return m_list.end(); }

	iterator erase(iterator i) { return m_list.erase(i); }

protected:
	friend class Snapshot;

	BaseList(const Container &list) :
		m_list(list)
	{}

protected:
	Container m_list;
};

class Snapshot::List : public Snapshot::BaseList
{
public:
	IFileInfo::size_type totalSize() const { return m_data->totalSize; }
	const IFileContainer *container() const { return m_data->container; }

protected:
	friend class Snapshot;

	List(const QExplicitlySharedDataPointer<Data> &data) :
		Snapshot::BaseList(data->map.values()),
		m_data(data)
	{}

private:
	QExplicitlySharedDataPointer<Data> m_data;
};


class Snapshot::Files
{
public:
	Files(const IFileContainer *container) :
		m_data(new Data(container))
	{}

	void add(const QString &fileName, NodeItem *item) { m_data->map[fileName].first = item; }

private:
	friend class Snapshot;
	QExplicitlySharedDataPointer<Data> m_data;
};


class Snapshot::Updates : public Snapshot::BaseList
{
protected:
	friend class Snapshot;

	Updates(const Container &list) :
		Snapshot::BaseList(list)
	{}
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMSNAPSHOT_H_ */
