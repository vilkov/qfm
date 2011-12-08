#ifndef IDMROOTNODE_H_
#define IDMROOTNODE_H_

#include "idmrootnodedelegate.h"
#include "../../containeres/idmcontainer.h"
#include "../../../../filesystem/tasks/filesystemtasksnode.h"
#include "../../../../filesystem/model/filesystemmodelcontainer.h"
#include "../../../../filesystem/interfaces/filesysteminodeview.h"


IDM_PLUGIN_NS_BEGIN

class IdmRootNode : public TasksNode
{
public:
	IdmRootNode(const Info &storage, Node *parent = 0);
	virtual ~IdmRootNode();

    /* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual void refresh();

	/* IFileOperations */
	virtual IFileInfo *info(const QModelIndex &idx) const;
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void menuAction(QAction *action, INodeView *view);
	virtual void createFile(const QModelIndex &index, INodeView *view);
	virtual void createDirectory(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndexList &list, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);
	virtual void cancel(const QModelIndexList &list, INodeView *view);
	virtual void calculateSize(const QModelIndexList &list, INodeView *view);
	virtual void pathToClipboard(const QModelIndexList &list, INodeView *view);
	virtual void copy(const INodeView *source, INodeView *destination);
	virtual void move(const INodeView *source, INodeView *destination);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual QAbstractItemModel *proxyModel() const;
	virtual QAbstractItemDelegate *itemDelegate() const;
	virtual const INodeView::MenuActionList &menuActions() const;

	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);
	virtual void nodeRemoved(Node *node);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(TaskNodeItem::Base *item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(TaskNodeItem::Base *item, quint64 timeElapsed);

private:
	enum MenuId
	{
		Create,
		Remove,
		AddProperty,
		RemoveProperty,
		Find
	};

	enum Items
	{
		RootItem = 0,
		FilesItem = 1
	};

private:
	class ItemsContainer : public ModelContainer
	{
	public:
		typedef QList<Item*> List;

	public:
		ItemsContainer();
		virtual ~ItemsContainer();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

	private:
		friend class IdmRootNode;
		List m_container;
	};

private:
	void add(IdmEntity *entity);
	void remove(const QModelIndex &index);
	void doAdd(IdmEntity *entity);
	void doAdd(ItemsContainer::Item *item, IdmEntity *property);
	void doRemove(ItemsContainer::Item *item, ItemsContainer::size_type index);
	void expand(ItemsContainer::Item *parent);

private:
	typedef QMap<IdmEntity*, ItemsContainer::List> EntitiesMap;

private:
	INodeView::MenuActionList m_menuActions;
	ItemsContainer m_itemsContainer;
	ItemsContainer::List &m_items;
	EntitiesMap m_entities;
	IdmContainer m_container;
	IdmRootNodeDelegate m_delegate;
	Info m_info;
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODE_H_ */
