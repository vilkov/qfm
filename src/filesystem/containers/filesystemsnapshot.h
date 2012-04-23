#ifndef FILESYSTEMSNAPSHOT_H_
#define FILESYSTEMSNAPSHOT_H_

#include <QtCore/QMap>
#include <QtCore/QSharedData>
#include "filesystemwrappednodeitem.h"


FILE_SYSTEM_NS_BEGIN

class Snapshot
{
public:
	class List;
	class Files;

	typedef QPair<NodeItem *, WrappedNodeItem *> Pair;
	typedef QMap<QString, Pair>                  Container;

public:
	Snapshot(const Files &files);

	List list() const;

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
		const IFileContainer *m_container;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
};


class Snapshot::List
{
public:
	typedef QList<Pair>               Container;
	typedef Container::iterator       iterator;
	typedef Container::const_iterator const_iterator;

public:
	IFileInfo::size_type totalSize() const { return m_data->totalSize; }
	const IFileContainer *container() const { return m_data->m_container; }

	bool isAdded(iterator i) const { return (*i).first == NULL; }
	bool isUpdated(iterator i) const { return (*i).second != NULL && (*i).second->isValid(); }
	bool isRemoved(iterator i) const { return (*i).second == NULL; }

	iterator begin() { return m_list.begin(); }
	const_iterator begin() const { return m_list.begin(); }

	iterator end() { return m_list.end(); }
	const_iterator end() const { return m_list.end(); }

	iterator erase(iterator i) { return m_list.erase(i); }

protected:
	friend class Snapshot;

	List(const QExplicitlySharedDataPointer<Data> &data) :
		m_data(data),
		m_list(data->map.values())
	{}

private:
	QExplicitlySharedDataPointer<Data> m_data;
	Container m_list;
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

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMSNAPSHOT_H_ */
