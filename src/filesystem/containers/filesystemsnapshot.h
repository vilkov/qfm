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
	typedef Container::iterator                  iterator;
	typedef Container::const_iterator            const_iterator;

public:
	Snapshot();
	Snapshot(const Files &files);

	List list() const;
	Updates takeUpdates();

	const_iterator begin() const { return m_data->map.begin(); }
	iterator begin() { return m_data->map.begin(); }

	const_iterator end() const { return m_data->map.end(); }
	iterator end() { return m_data->map.end(); }

	const IFileContainer *container() const { return m_data->container; }

	IFileInfo::size_type totalSize() const
	{
		IFileInfo::size_type res = 0;

		for (const_iterator it = begin(), localEnd = end(); it != localEnd; ++it)
			if ((*it).second)
				res += (*it).second->totalSize();

		return res;
	}

	NodeItem *exists(const QString &fileName) const
	{
		return m_data->map.value(fileName).first;
	}

	void insert(iterator &i, WrappedNodeItem *item)
	{
		(*i).second = item;
	}

	void insert(const QString &fileName, WrappedNodeItem *item)
	{
		Pair &pair = m_data->map[fileName];
		delete pair.second;
		pair.second = item;
	}

	void remove(iterator &i)
	{
		delete (*i).second;
		i = m_data->map.erase(i);
	}

private:
	friend class Files;
	friend class Update;

	struct Data : public QSharedData
	{
		Data(const IFileContainer *container);
		~Data();

		Container map;
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
	typedef Container::size_type      size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	bool isEmpty() const { return m_list.isEmpty(); }
	size_type size() const { return m_list.size(); }
	const Container::value_type &at(size_type index) const { return m_list.at(index); }

	size_type indexOf(NodeItem *item) const
	{
		for (size_type i = 0; i < size(); ++i)
			if (at(i).first == item)
				return i;

		return InvalidIndex;
	}

	void removeAt(size_type index) { m_list.removeAt(index); }

	bool isAdded(const_iterator i) const { return (*i).first == NULL; }
	bool isUpdated(const_iterator i) const { return (*i).second != NULL && (*i).second->isValid(); }
	bool isRemoved(const_iterator i) const { return (*i).second == NULL; }

	const_iterator begin() const { return m_list.begin(); }
	iterator begin() { return m_list.begin(); }

	const_iterator end() const { return m_list.end(); }
	iterator end() { return m_list.end(); }

	iterator erase(iterator i) { delete (*i).second; return m_list.erase(i); }

protected:
	BaseList()
	{}

	BaseList(const Container &list) :
		m_list(list)
	{}

protected:
	Container m_list;
};

class Snapshot::List : public Snapshot::BaseList
{
public:
	List()
	{}

	const IFileContainer *container() const { return m_data->container; }

	IFileInfo::size_type totalSize() const
	{
		IFileInfo::size_type res = 0;

		for (const_iterator it = begin(), localEnd = end(); it != localEnd; ++it)
			if ((*it).second)
				res += (*it).second->totalSize();

		return res;
	}

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

	bool isEmpty() const { return m_data->map.isEmpty(); }
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
