#ifndef FILESYSTEMFOLDERNODE_H_
#define FILESYSTEMFOLDERNODE_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "filesystemdelegate.h"
#include "filesystemproxymodel.h"
#include "filesystemchangeslist.h"
#include "items/filesystemfoldernodeitem.h"
#include "events/filesystemmodelevent.h"
#include "../filesystem_ns.h"
#include "../filesystemnode.h"
#include "../info/filesysteminfo.h"
#include "../../tools/metatemplates.h"


FILE_SYSTEM_NS_BEGIN

class FolderNode : public Node
{
	Q_DISABLE_COPY(FolderNode)

public:
	FolderNode(const Info &info, Node *parent = 0);

    virtual bool event(QEvent *e);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* IFileInfo */
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QDateTime lastModified() const;

	virtual void refresh();

	/* Node */
	virtual void update();
	virtual Node *subnode(const QModelIndex &idx, PluginsManager *plugins);
	virtual void remove(Node *subnode);
	virtual void remove(const QModelIndex &index);
	virtual void copy(const QModelIndex &index, Node *destination);
	virtual void move(const QModelIndex &index, Node *destination);
	virtual void createFolder(const QString &name);
	virtual void createFile(const QString &name);
	virtual void view(QAbstractItemView *itemView);

	virtual QModelIndex parentEntryIndex() const { return m_parentEntryIndex; }
	virtual void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }

	virtual QModelIndex rootIndex() const;
	virtual bool isRootIndex(const QModelIndex &index) const;

protected:
	virtual bool isRoot() const { return m_items.size() == 0 || !m_items.at(0).item->isRoot(); }
	virtual Node *node(const QString &fileName, PluginsManager *plugins);

protected:
	FolderNodeItem *rootItem() const { return m_items.at(0).item; }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

protected:
	void list(FolderNodeItem *fileSystemTree);
	void listEvent(const ModelEvent::Params *p);

	void updateFiles();
	void updateFilesEvent(const ModelEvent::Params *p);

	void removeEntry(FolderNodeItem *fileSystemTree, FolderNodeItem *entry);
	void scanForRemove(FolderNodeItem *fileSystemTree, FolderNodeItem *entry);
	void scanForRemoveEvent(const ModelEvent::Params *p);
	void removeCompleteEvent(const ModelEvent::Params *p);
	void removeCanceledEvent(const ModelEvent::Params *p);

	void scanForSize(FolderNodeItem *fileSystemTree, FolderNodeItem *entry);
	void scanForSizeEvent(const ModelEvent::Params *p);

	void copyEntry(FolderNodeItem *fileSystemTree, FolderNodeItem *entry, Node *destination);
	void scanForCopy(FolderNodeItem *fileSystemTree, FolderNodeItem *entry, Node *destination);
	void moveEntry(FolderNodeItem *fileSystemTree, FolderNodeItem *entry, Node *destination);
	void scanForMove(FolderNodeItem *fileSystemTree, FolderNodeItem *entry, Node *destination);
	void scanForCopyEvent(const ModelEvent::Params *p);
	void scanForMoveEvent(const ModelEvent::Params *p);
	void copyCompleteEvent(const ModelEvent::Params *p);
	void copyCanceledEvent(const ModelEvent::Params *p);

	void questionAnswerEvent(const ModelEvent::Params *p);
	void updateProgressEvent(const ModelEvent::Params *p);

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

private:
	ChangesList makeChangeSet() const;
	QModelIndex index(int column, FolderNodeItem *item) const;
	Node *createNode(const Info &info, PluginsManager *plugins) const;
	Values::Value createNode(const QString &fileName, PluginsManager *plugins, Node *&node) const;

	void updateFirstColumn(FolderNodeItem *fileSystemTree, FolderNodeItem *entry);
	void updateSecondColumn(FolderNodeItem *fileSystemTree, FolderNodeItem *entry);
	void updateBothColumns(FolderNodeItem *fileSystemTree, FolderNodeItem *entry);
	void removeEntry(Values::size_type index);
	void removeEntry(const QModelIndex &index);
	void refresh(FolderNodeItem *fileSystemTree);
	void doRefresh();

private:
	bool m_updating;
	Values m_items;
	ProxyModel m_proxy;
	Delegate m_delegate;
	QModelIndex m_parentEntryIndex;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODE_H_ */
