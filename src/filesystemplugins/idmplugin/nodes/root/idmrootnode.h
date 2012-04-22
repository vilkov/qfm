#ifndef IDMROOTNODE_H_
#define IDMROOTNODE_H_

#include "idmrootnodedelegate.h"
#include "../../containeres/idmcontainer.h"
#include "../../../../filesystem/tasks/filesystemtasksnode.h"
#include "../../../../filesystem/model/filesystemnodemodelcontainer.h"
#include "../../../../filesystem/interfaces/filesysteminodeview.h"


IDM_PLUGIN_NS_BEGIN

class IdmRootNode : public TasksNode
{
public:
	IdmRootNode(IFileContainer::Holder &container, Node *parent = 0);
	virtual ~IdmRootNode();

    /* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view);
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
	virtual void removeToTrash(const QModelIndexList &list, INodeView *view);

	/* INode */
    virtual void refresh();
	virtual QString title() const;
	virtual QString location() const;
	virtual QString location(const QString &fileName) const;
	virtual QString location(const QModelIndex &index) const;

	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual QAbstractItemView::SelectionMode selectionMode() const;
	virtual ::History::Entry *menuAction(QAction *action, INodeView *view);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);
	virtual void nodeRemoved(Node *node);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(const Item *item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const Item *item, quint64 timeElapsed);
	virtual void performActionEvent(const AsyncFileAction::FilesList &files);

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
		RootItemIndex = 0,
		FilesItemIndex = 1
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
	void createEntity();
	void removeEntity(const QModelIndex &index);
	void addProperty(const QModelIndex &index);
	void removeProperty(const QModelIndex &index);

private:
	void doAdd(IdmEntity *entity);
	void doRemove(IdmEntity *entity);
	void doAdd(const QModelIndex &index, ItemsContainer::Item *item, IdmEntity *property, const QString &propertyName);
	void doRemove(const QModelIndex &index, ItemsContainer::Item *item, ItemsContainer::Item *property);
	void expand(ItemsContainer::Item *parent);

private:
	typedef QMap<IdmEntity*, ItemsContainer::List> EntitiesMap;

private:
	INodeView::MenuActionList m_actions;
	ItemsContainer m_itemsContainer;
	ItemsContainer::List &m_items;
	EntitiesMap m_entities;
	IdmContainer m_container;
	IdmRootNodeDelegate m_delegate;
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODE_H_ */
