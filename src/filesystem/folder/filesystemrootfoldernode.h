#ifndef FILESYSTEMROOTFOLDERNODE_H_
#define FILESYSTEMROOTFOLDERNODE_H_

#include "filesystemfolderdelegate.h"
#include "filesystemfolderproxymodel.h"
#include "functors/filesystemfoldernodefunctors.h"
#include "base/filesystemfoldernodebase.h"
#include "containers/filesystemitemscontainer.h"
#include "../../tools/containers/union.h"


FILE_SYSTEM_NS_BEGIN

class RootFolderNode : public FolderNodeBase
{
	Q_DISABLE_COPY(RootFolderNode)

public:
	RootFolderNode(IFileContainer::Holder &container, Node *parent = 0);
	virtual ~RootFolderNode();

    /* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* IFileOperations */
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
	using FolderNodeBase::location;
	virtual QString location(const QModelIndex &index) const;

	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual ::History::Entry *menuAction(QAction *action, INodeView *view);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);
	virtual void nodeRemoved(Node *node);

protected:
	/* FolderNodeBase */
	virtual void doesNotExistAnyMore();

protected:
	/* FolderNodeBase */
	virtual UpdatesList::Changes updateFilesMap() const;
	virtual void updateFilesEvent(const UpdatesList &updates);
	virtual void scanForSizeEvent(bool canceled, const Snapshot &snapshot);
	virtual bool scanForCopyEvent(bool canceled, const Snapshot &snapshot, ICopyControl *control, bool move);
	virtual bool scanForRemoveEvent(bool canceled, const Snapshot &snapshot);
	virtual bool performCopyEvent(bool canceled, const Snapshot &snapshot, bool move);
	virtual void performRemoveEvent(bool canceled, const Snapshot &snapshot);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(const NodeItem *item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const NodeItem *item, quint64 timeElapsed);
	virtual void performActionEvent(const AsyncFileAction::FilesList &files);

protected:
	virtual Node *createNode(const IFileInfo *file, PluginsManager *plugins) const;

protected:
	typedef QPair<ItemsContainer::size_type, FolderBaseItem*> ProcessedValue;
	class ProcessedList : public Functors::Functor, public QList<ProcessedValue>
	{
	protected:
		virtual void call(ItemsContainer::size_type index, FolderBaseItem *entry)
		{
			push_back(ProcessedValue(index, entry));
		}
	};

	class AbsoluteFilePathList : public Functors::Functor, public QStringList
	{
	public:
		AbsoluteFilePathList(const IFileContainer *container) :
			m_container(container)
		{}

	protected:
		virtual void call(ItemsContainer::size_type index, FolderBaseItem *entry)
		{
			push_back(m_container->location(entry->info().fileName()));
		}

	private:
		const IFileContainer *m_container;
	};

	class CancelFunctor : public Functors::Functor
	{
	public:
		CancelFunctor(RootFolderNode *node, const QString &reason) :
			m_node(node),
			m_reason(reason)
		{}

		const ::Tools::Containers::Union &updateUnion() const { return m_union; }

	protected:
		virtual void call(ItemsContainer::size_type index, FolderBaseItem *entry);

	private:
		RootFolderNode *m_node;
		QString m_reason;
		TasksItemList m_items;
		::Tools::Containers::Union m_union;
	};
	friend class CancelFunctor;

	class RenameFunctor : public Functors::Functor
	{
	public:
		RenameFunctor(const IFileContainer *container, ItemsContainer &items) :
			m_container(container),
			m_items(items)
		{}

	protected:
		virtual void call(ItemsContainer::size_type index, FolderBaseItem *entry);

	private:
		const IFileContainer *m_container;
		ItemsContainer &m_items;
	};

	void processIndexList(const QModelIndexList &list, Functors::Functor &functor);
	void processLockedIndexList(const QModelIndexList &list, Functors::Functor &functor);

private:
	void scanForRemove(const ProcessedList &entries);
	void scanForSize(const ProcessedList &entries);
	void scanForCopy(const ProcessedList &entries, INodeView *destination, bool move);

private:
	QModelIndex index(int column, FolderBaseItem *item) const;
	QModelIndex indexForFile(FolderBaseItem *item) const;
	QModelIndex indexForFile(FolderBaseItem *item, ItemsContainer::size_type index) const;

private:
	typedef ::Tools::Containers::Union Union;

	void updateFirstColumn(FolderBaseItem *entry);
	void updateFirstColumn(const Union &range);
	void updateFirstColumn(ItemsContainer::size_type index, FolderBaseItem *entry);
	void updateSecondColumn(FolderBaseItem *entry);
	void updateSecondColumn(const Union &range);
	void updateSecondColumn(ItemsContainer::size_type index, FolderBaseItem *entry);
	void updateBothColumns(FolderBaseItem *entry);
	void updateBothColumns(const Union &range);
	void updateBothColumns(ItemsContainer::size_type index, FolderBaseItem *entry);
	void updateColumns(const Union &range, int lastColumn);
	void removeEntry(ItemsContainer::size_type index);
	void removeEntry(const QModelIndex &index);

protected:
	ItemsContainer m_items;
	FolderProxyModel m_proxy;
	FolderDelegate m_delegate;
	INodeView::MenuActionList m_menuActions;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTFOLDERNODE_H_ */
