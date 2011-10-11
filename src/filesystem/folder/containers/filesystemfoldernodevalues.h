#ifndef FILESYSTEMFOLDERNODEVALUES_H_
#define FILESYSTEMFOLDERNODEVALUES_H_

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QList>
#include "../items/filesystemfoldernodeitem.h"
#include "../../filesystemnode.h"
#include "../../../tools/containers/hashedlist.h"


FILE_SYSTEM_NS_BEGIN

class Values
{
public:
	struct Value
	{
		Value() :
			node(0),
			item(0)
		{}
		Value(FolderNodeItem *item) :
			node(0),
			item(item)
		{
			Q_ASSERT(item);
		}
		Value(FolderNodeItem *item, Node *node) :
			node(node),
			item(item)
		{
			Q_ASSERT(item);
		}

		Node *node;
		FolderNodeItem *item;
	};
	typedef HashedList<QString, Value> ValueList;
	typedef ValueList::size_type       size_type;
	enum { InvalidIndex = ValueList::InvalidIndex };

public:
	Values()
	{}
	~Values()
	{
		for (ValueList::size_type i = 0, size = m_items.size(); i < size; ++i)
		{
			delete m_items.at(i).node;
			delete m_items.at(i).item;
		}
	}

	Value &operator[](size_type index) { return m_items[index]; }
	const Value &operator[](size_type index) const { return m_items[index]; }
	const Value &at(size_type index) const { return m_items.at(index); }

	const Value &last() const { return m_items.last(); }
	Value &last() { return m_items.last(); }

	size_type lastIndex() const { return m_items.size() - 1; }
	size_type size() const { return m_items.size(); }
	size_type indexOf(Node *item) const
	{
		for (ValueList::size_type i = 0, size = m_items.size(); i < size; ++i)
			if (m_items.at(i).node == item)
				return i;

		return InvalidIndex;
	}
	size_type indexOf(FolderNodeItem *item) const
	{
		for (ValueList::size_type i = 0, size = m_items.size(); i < size; ++i)
			if (m_items.at(i).item == item)
				return i;

		return InvalidIndex;
	}
	size_type indexOf(const QString &fileName) const { return m_items.indexOf(fileName); }

	void add(const Value &value) { m_items.add(value.item->fileName(), value); }
	void add(FolderNodeItem *item) { m_items.add(item->fileName(), item); }
	void add(FolderNodeItem *item, Node *node) { m_items.add(item->fileName(), Value(item, node)); }
	void remove(size_type index) { m_items.remove(index); }
	Value take(size_type index) { return m_items.take(index); }

private:
	ValueList m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEVALUES_H_ */
