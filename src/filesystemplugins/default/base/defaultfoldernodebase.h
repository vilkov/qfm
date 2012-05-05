#ifndef DEFAULTFOLDERNODEBASE_H_
#define DEFAULTFOLDERNODEBASE_H_

#include "../defaultfolderdelegate.h"
#include "../defaultfolderproxymodel.h"
#include "../model/items/defaultnodeitem.h"
#include "../../../filesystem/tasks/filesystemtasksnode.h"
#include "../../../filesystem/interfaces/filesystemifilecontainer.h"
#include "../../../tools/containers/hashedlist.h"
#include "../../../tools/containers/union.h"
#include "../../../tools/templates/functors.h"


DEFAULT_PLUGIN_NS_BEGIN

class FolderNodeBase : public TasksNode
{
	Q_DISABLE_COPY(FolderNodeBase)

public:
	FolderNodeBase(IFileContainer::Holder &container, Node *parent = 0);
	virtual ~FolderNodeBase();

    /* Model */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* TasksNode */
    virtual bool event(QEvent *event);

    /* INode */
	virtual void refresh();
	virtual QString title() const;
	virtual Sorting sorting() const;
	virtual Geometry geometry() const;
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
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);
	virtual void nodeRemoved(Node *node);

protected:
	typedef ::Tools::Templates::Functor3<Container::size_type, DefaultNodeItem *, WrappedNodeItem *> EventFunctor;

	class ScanForSizeEventFunctor;
	class ScanForSizeEventFunctor_canceled;
	class ScanForCopyEventFunctor;
	class ScanForCopyEventFunctor_canceled;
	class ScanForRemoveEventFunctor;
	class ScanForRemoveEventFunctor_canceled;
	class PerformCopyEventFunctor;
	class PerformCopyEventFunctor_canceled;
	class PerformRemoveEventFunctor;

	void cleanup(Snapshot &snapshot);
	void processScanEventSnapshot(Snapshot &snapshot, EventFunctor &functor);
	void processPerformEventSnapshot(Snapshot &snapshot, EventFunctor &functor);

	/* Task event handlers */
	virtual Snapshot updateFilesList() const;
	virtual void updateFilesEvent(Snapshot &updates);
	virtual void scanForSizeEvent(bool canceled, Snapshot &snapshot);
	virtual bool scanForCopyEvent(bool canceled, Snapshot &snapshot, ICopyControl *control, bool move);
	virtual bool scanForRemoveEvent(bool canceled, Snapshot &snapshot);
	virtual bool performCopyEvent(bool canceled, Snapshot &snapshot, bool move);
	virtual void performRemoveEvent(bool canceled, Snapshot &snapshot);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(const NodeItem::Holder &item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const NodeItem::Holder &item, quint64 timeElapsed);
	virtual void performActionEvent(const AsyncFileAction::FilesList &files);

protected:
	const IFileContainer::Holder &container() const { return m_container; }
	virtual Node *createNode(const IFileInfo *file, PluginsManager *plugins) const;

protected:
	class Container : public TasksNode::Container
	{
	public:
		typedef DefaultNodeItem::Holder value_type;

	public:
		Container();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

		const value_type &operator[](size_type index) const { return m_container.at(index); }

		const value_type &last() const { return m_container.last(); }
		value_type &last() { return m_container.last(); }

		bool isEmpty() const { return m_container.isEmpty(); }
		size_type lastIndex() const { return m_container.size() - 1; }
		size_type indexOf(Node *item) const
		{
			for (List::size_type i = 0, size = m_container.size(); i < size; ++i)
				if (m_container.at(i).as<DefaultNodeItem>()->node() == item)
					return i;

			return InvalidIndex;
		}
		size_type indexOf(const QString &fileName) const { return m_container.indexOf(fileName); }

		void add(const value_type &item) { m_container.add(item.as<DefaultNodeItem>()->info()->fileName(), item); }
		void add(const QString &fileName, const value_type &item) { m_container.add(fileName, item); }
		void remove(size_type index) { m_container.remove(index); }
		value_type take(size_type index) { return m_container.take(index); }
		void replace(size_type index, const QString &oldHash, const QString &newHash) { m_container.replace(index, oldHash, newHash); }
		void clear()
		{
			List::ByIndex list(m_container);
			value_type root = list.at(0);

			m_container.clear();
			m_container.add(root.as<DefaultNodeItem>()->info()->fileName(), root);
		}

	private:
		typedef ::Tools::Containers::HashedList<QString, value_type> List;
		List m_container;
	};

	const Container &items() const { return m_items; }
	Container &items() { return m_items; }

	const FolderProxyModel &proxy() const { return m_proxy; }
	FolderProxyModel &proxy() { return m_proxy; }

private:
	/* Prepare tasks */
	void updateFiles();
	void scanForSize(const Snapshot &snapshot);
	void scanForCopy(const Snapshot &snapshot, ICopyControl::Holder &destination, bool move);
	void scanForRemove(const Snapshot &snapshot);
	void performCopy(BaseTask *oldTask, const Snapshot &snapshot, ICopyControl::Holder &destination, bool move);
	void performRemove(BaseTask *oldTask, const Snapshot &snapshot);
	void performRemove(BaseTask *oldTask, const ICopyControl *destination, const Snapshot &snapshot);

	/* Tasks events */
	void updateFiles(const BaseTask::Event *event);
	void scanForSize(const BaseTask::Event *event);
	void scanForCopy(const BaseTask::Event *event);
	void scanForRemove(const BaseTask::Event *event);
	void performCopy(const BaseTask::Event *event);
	void performRemove(const BaseTask::Event *event);

private:
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

private:
	typedef ::Tools::Templates::Functor2<Container::size_type, DefaultNodeItem *> Functor;

	typedef QPair<Container::size_type, DefaultNodeItem *> ProcessedValue;
	class ProcessedList : public Functor, public QList<ProcessedValue>
	{
	protected:
		virtual void call(Container::size_type index, DefaultNodeItem *item)
		{
			push_back(ProcessedValue(index, item));
		}
	};


	class AbsoluteFilePathList : public Functor, public QStringList
	{
	public:
		AbsoluteFilePathList(const IFileContainer *container) :
			m_container(container)
		{}

	protected:
		virtual void call(Container::size_type index, DefaultNodeItem *item)
		{
			push_back(m_container->location(item->info()->fileName()));
		}

	private:
		const IFileContainer *m_container;
	};


	class CancelFunctor : public Functor
	{
	public:
		CancelFunctor(FolderNodeBase *node, const QString &reason) :
			m_node(node),
			m_reason(reason)
		{}

		const ::Tools::Containers::Union &updateUnion() const { return m_union; }

	protected:
		virtual void call(Container::size_type index, DefaultNodeItem *item);

	private:
		FolderNodeBase *m_node;
		QString m_reason;
		TasksMap::List m_items;
		::Tools::Containers::Union m_union;
	};
	friend class CancelFunctor;


	class RenameFunctor : public Functor
	{
	public:
		RenameFunctor(const IFileContainer *container, Container &items) :
			m_container(container),
			m_items(items)
		{}

	protected:
		virtual void call(Container::size_type index, DefaultNodeItem *item);

	private:
		const IFileContainer *m_container;
		Container &m_items;
	};


	void processIndexList(const QModelIndexList &list, Functor &functor);
	void processLockedIndexList(const QModelIndexList &list, Functor &functor);

private:
	void scanForRemove(const ProcessedList &entries);
	void scanForSize(const ProcessedList &entries);
	void scanForCopy(const ProcessedList &entries, INodeView *destination, bool move);

private:
	QModelIndex index(int column, DefaultNodeItem *item) const;
	QModelIndex indexForFile(DefaultNodeItem *item) const;
	QModelIndex indexForFile(DefaultNodeItem *item, Container::size_type index) const;

private:
	typedef ::Tools::Containers::Union Union;

	void updateFirstColumn(DefaultNodeItem *entry);
	void updateFirstColumn(const Union &range);
	void updateFirstColumn(Container::size_type index, DefaultNodeItem *entry);
	void updateSecondColumn(DefaultNodeItem *entry);
	void updateSecondColumn(const Union &range);
	void updateSecondColumn(Container::size_type index, DefaultNodeItem *entry);
	void updateBothColumns(DefaultNodeItem *entry);
	void updateBothColumns(const Union &range);
	void updateBothColumns(Container::size_type index, DefaultNodeItem *entry);
	void updateColumns(const Union &range, int lastColumn);
	void removeEntry(Container::size_type index);
	void removeEntry(const QModelIndex &index);

private:
	IFileContainer::Holder m_container;
	Container m_items;
	bool m_updating;

	FolderProxyModel m_proxy;
	FolderDelegate m_delegate;
	INodeView::MenuActionList m_menuActions;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERNODEBASE_H_ */
