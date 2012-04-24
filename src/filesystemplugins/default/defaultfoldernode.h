#ifndef DEFAULTFOLDERNODE_H_
#define DEFAULTFOLDERNODE_H_

#include "defaultfolderdelegate.h"
#include "defaultfolderproxymodel.h"
#include "model/items/defaultnodeitem.h"
#include "../../filesystem/tasks/filesystemtasksnode.h"
#include "../../filesystem/interfaces/filesystemifilecontainer.h"
#include "../../tools/containers/hashedlist.h"


//#include "containers/filesystemupdateslist.h"
//#include "../../interfaces/filesystemifilecontainer.h"
//#include "../../tasks/filesystemtasksnode.h"
//#include "../../tasks/concrete/containers/filesystemsnapshot.h"


DEFAULT_PLUGIN_NS_BEGIN

class DefaultFolderNode : public TasksNode
{
	Q_DISABLE_COPY(DefaultFolderNode)

public:
	DefaultFolderNode(IFileContainer::Holder &container, Node *parent = 0);
	virtual ~DefaultFolderNode();

    /* Model */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* TasksNode */
    virtual bool event(QEvent *event);

    /* INode */
    virtual void refresh();
	virtual QString title() const;
	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual ::History::Entry *menuAction(QAction *action, INodeView *view);

	/* IFileLocation */
	virtual QString location() const;
	virtual QString location(const QString &fileName) const;

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

protected:
	/* Task event handlers */
	virtual UpdatesList::Changes updateFilesMap() const = 0;
	virtual void updateFilesEvent(const UpdatesList &updates) = 0;
	virtual void scanForSizeEvent(bool canceled, const Snapshot &snapshot) = 0;
	virtual bool scanForCopyEvent(bool canceled, const Snapshot &snapshot, ICopyControl *control, bool move) = 0;
	virtual bool scanForRemoveEvent(bool canceled, const Snapshot &snapshot) = 0;
	virtual bool performCopyEvent(bool canceled, const Snapshot &snapshot, bool move) = 0;
	virtual void performRemoveEvent(bool canceled, const Snapshot &snapshot) = 0;

private:
	void updateFiles();

	/* Tasks events */
	void updateFiles(const BaseTask::Event *event);
	void scanForSize(const BaseTask::Event *event);
	void scanForCopy(const BaseTask::Event *event);
	void scanForRemove(const BaseTask::Event *event);
	void performCopy(const BaseTask::Event *event);
	void performRemove(const BaseTask::Event *event);

private:
	class Container : public TasksNode::Container
	{
	public:
		Container();
		virtual ~Container();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

	private:
		friend class DefaultFolderNode;
		typedef ::Tools::Containers::HashedList<QString, DefaultFolderItem *> List;
		List m_list;
	};

private:
	IFileContainer::Holder m_container;
	Container m_itemsContainer;
	Container::List &m_items;
	bool m_updating;

	FolderProxyModel m_proxy;
	FolderDelegate m_delegate;
	INodeView::MenuActionList m_menuActions;







protected:
	/* Prepare tasks */
	void scanForSize(const TasksItemList &entries);
	void scanForCopy(const TasksItemList &entries, ICopyControl::Holder &destination, bool move);
	void scanForRemove(const TasksItemList &entries);
	void performCopy(BaseTask *oldTask, const Snapshot &Snapshot, ICopyControl::Holder &destination, bool move);
	void performRemove(BaseTask *oldTask, const Snapshot &Snapshot);

protected:
	const IFileContainer *container() const { return m_container.data(); }

	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERNODE_H_ */
