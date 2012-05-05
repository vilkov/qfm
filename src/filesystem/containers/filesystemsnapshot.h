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
	class Files;
	class Updates;

	typedef QPair<NodeItem::Holder, WrappedNodeItem *> Pair;
	typedef QMap<QString, Pair>                        Container;
	typedef Container::iterator                        iterator;
	typedef Container::const_iterator                  const_iterator;
	typedef Container::size_type                       size_type;

public:
	Snapshot();
	Snapshot(const Files &files);
	Snapshot(const Updates &updates);

	bool isEmpty() const { return m_data->map.isEmpty(); }
	size_type size() const { return m_data->map.size(); }

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

	bool isAdded(const_iterator i) const { return (*i).first.data() == NULL; }
	bool isUpdated(const_iterator i) const { return (*i).second != NULL && (*i).second->isValid(); }
	bool isRemoved(const_iterator i) const { return (*i).second == NULL; }

	NodeItem::Holder exists(const QString &fileName) const
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

	iterator erase(iterator i)
	{
		delete (*i).second;
		return m_data->map.erase(i);
	}

	iterator remove(iterator i)
	{
		return m_data->map.erase(i);
	}

private:
	friend class Files;
	friend class Updates;

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


class Snapshot::Files
{
public:
	Files(const IFileContainer *container) :
		m_data(new Data(container))
	{}

	bool isEmpty() const { return m_data->map.isEmpty(); }
	void add(const QString &fileName, const NodeItem::Holder &item) { m_data->map[fileName].first = item; }

private:
	friend class Snapshot;
	QExplicitlySharedDataPointer<Data> m_data;
};


class Snapshot::Updates
{
public:
	Updates(const Snapshot &snapshot) :
		m_data(new Data(snapshot.m_data->container))
	{}

	void add(const QString &fileName, const Pair &pair) { m_data->map[fileName] = pair; }

private:
	friend class Snapshot;
	QExplicitlySharedDataPointer<Data> m_data;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMSNAPSHOT_H_ */
