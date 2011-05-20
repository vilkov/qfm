#ifndef FILESYSTEMFOLDERNODEBASE_H_
#define FILESYSTEMFOLDERNODEBASE_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "items/filesystemfoldernodeitem.h"
#include "../filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeBase : public Node
{
	Q_DISABLE_COPY(FolderNodeBase)

public:
	FolderNodeBase(Node *parent = 0) :
		Node(parent)
	{}

protected:
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

			bool operator==(const Value &other)
			{
				return item == other.item;
			}

			Node *node;
			FolderNodeItem *item;
		};
		typedef QList<Value> 			 ValueList;
		typedef ValueList::size_type     size_type;
		typedef QMap<QString, size_type> ValueMap;
		enum { InvalidIndex = (size_type)-1 };

	public:
		Values()
		{}
		~Values()
		{
			for (ValueList::size_type i = 0, size = m_list.size(); i < size; ++i)
			{
				delete m_list.at(i).node;
				delete m_list.at(i).item;
			}
		}

		Value &operator[](size_type index) { return m_list[index]; }
		const Value &operator[](size_type index) const { return m_list[index]; }
		const Value &at(size_type index) const { return m_list.at(index); }
		const Value &last() const { return m_list.last(); }
		Value &last() { return m_list.last(); }

		size_type size() const { return m_list.size(); }
		size_type indexOf(FolderNodeItem *item) const { return m_list.indexOf(item); }
		size_type indexOf(const QString &fileName) const { return m_map.value(fileName, InvalidIndex); }

		void add(const Value &value)
		{
			m_list.push_back(value);
			m_map[value.item->fileName()] = m_list.size() - 1;
		}
		void add(FolderNodeItem *item)
		{
			m_list.push_back(item);
			m_map[item->fileName()] = m_list.size() - 1;
		}
		void add(FolderNodeItem *item, Node *node)
		{
			m_list.push_back(Value(item, node));
			m_map[item->fileName()] = m_list.size() - 1;
		}
		void remove(size_type index)
		{
			Q_ASSERT(index != InvalidIndex);
			const Value &value = m_list.at(index);
			m_map.remove(value.item->fileName());
			delete value.item;
			delete value.node;
			m_list.removeAt(index);
		}

	private:
		ValueList m_list;
		ValueMap m_map;
	};

protected:
	const Values &items() const { return m_items; }
	Values &items() { return m_items; }

private:
	Values m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEBASE_H_ */
