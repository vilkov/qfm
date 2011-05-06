#ifndef FILESYSTEMFOLDERNODE_H_
#define FILESYSTEMFOLDERNODE_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "items/filesystemfoldernodeitem.h"
#include "../filesystem_ns.h"
#include "../filesystemnode.h"
#include "../info/filesysteminfo.h"


FILE_SYSTEM_NS_BEGIN

class FolderNode : public Node
{
public:
	FolderNode(const Info &info, Node *parent = 0);

	/* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* Node */
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;

	virtual void refresh();
	virtual void activated(const QModelIndex &index);
	virtual void remove(const QModelIndex &index);
	virtual void copy(const QModelIndex &index, Node *destination);
	virtual void move(const QModelIndex &index, Node *destination);
	virtual void createFolder(const QString &name);
	virtual void createFile(const QString &name);

protected:
	virtual bool isRoot() const
	{
		if (m_items.size() > 0 && m_items.at(0).item->isRoot())
			return false;
		else
			return true;
	}
	virtual void view(QAbstractItemView *itemView);
	virtual Node *node(const QString &fileName, PluginsManager *plugins);

protected:
	QModelIndex index(int column, FolderNodeItem *item) const;

private:
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
				Q_ASSERT(node);
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

		size_type size() const { return m_list.size(); }
		size_type indexOf(FolderNodeItem *item) const { return m_list.indexOf(item); }
		size_type indexOf(const QString &fileName) const { return m_map.value(fileName, InvalidIndex); }

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

private:
	Values m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODE_H_ */
