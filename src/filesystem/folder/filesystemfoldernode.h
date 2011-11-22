#ifndef FILESYSTEMFOLDERNODE_H_
#define FILESYSTEMFOLDERNODE_H_

#include "filesystemfolderdelegate.h"
#include "filesystemfolderproxymodel.h"
#include "functors/filesystemfoldernodefunctors.h"
#include "base/filesystemfoldernodebase.h"
#include "containers/filesystemitemscontainer.h"
#include "../../tools/metatemplates.h"
#include "../../tools/rangeintersection.h"


FILE_SYSTEM_NS_BEGIN

class FolderNode : public FolderNodeBase
{
	Q_DISABLE_COPY(FolderNode)

public:
	FolderNode(const Info &info, Node *parent = 0);

    /* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* INode::IFileOperations */
	virtual IFileInfo *info(const QModelIndex &idx) const;
	virtual void menuAction(QAction *action, INodeView *view);
	virtual void createFile(const QModelIndex &index, INodeView *view);
	virtual void createDirectory(const QModelIndex &index, INodeView *view);
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
	/* FolderNodeBase */
	virtual UpdatesList::Map updateFilesMap() const;
	virtual void updateFilesEvent(const UpdatesList &updates);
	virtual void scanForSizeEvent(bool canceled, const InfoListItem *entries);
	virtual bool scanForCopyEvent(bool canceled, const InfoListItem *entries, ICopyControl *control, bool move);
	virtual bool scanForRemoveEvent(bool canceled, const InfoListItem *entries);
	virtual bool performCopyEvent(bool canceled, const InfoListItem *entries, bool move);
	virtual void performRemoveEvent(const InfoListItem *entries);

	virtual void updateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const QString &fileName, quint64 timeElapsed);

protected:
	typedef QPair<ItemsContainer::size_type, FileSystemBaseItem*> ProcessedValue;
	class ProcessedList : public Functors::Functor, public QList<ProcessedValue>
	{
	protected:
		virtual void call(ItemsContainer::size_type index, FileSystemBaseItem *entry)
		{
			push_back(ProcessedValue(index, entry));
		}
	};

	class AbsoluteFilePathList : public Functors::Functor, public QStringList
	{
	protected:
		virtual void call(ItemsContainer::size_type index, FileSystemBaseItem *entry)
		{
			push_back(entry->info().absoluteFilePath());
		}
	};

	class CancelFunctor : public Functors::Functor
	{
	public:
		CancelFunctor(FolderNode *node) :
			m_node(node)
		{}

	protected:
		virtual void call(ItemsContainer::size_type index, FileSystemBaseItem *entry);

	private:
		FolderNode *m_node;
	};
	friend class CancelFunctor;

	class RenameFunctor : public Functors::Functor
	{
	public:
		RenameFunctor(IFileInfo *info, ItemsContainer &items) :
			m_info(info),
			m_items(items)
		{}

	protected:
		virtual void call(ItemsContainer::size_type index, FileSystemBaseItem *entry);

	private:
		IFileInfo *m_info;
		ItemsContainer &m_items;
	};

	void processIndexList(const QModelIndexList &list, Functors::Functor &functor);
	void processLockedIndexList(const QModelIndexList &list, Functors::Functor &functor);

private:
	void scanForRemove(const ProcessedList &entries);
	void scanForSize(const ProcessedList &entries);
	void scanForCopy(const ProcessedList &entries, INodeView *destination, bool move);

private:
	QModelIndex index(int column, FileSystemBaseItem *item) const;
	Node *createNode(const Info &info, PluginsManager *plugins) const;
	QModelIndex indexForFile(FileSystemBaseItem *item) const;
	QModelIndex indexForFile(FileSystemBaseItem *item, ItemsContainer::size_type index) const;

	void updateFirstColumn(FileSystemBaseItem *entry);
	void updateFirstColumn(const RangeIntersection &range);
	void updateFirstColumn(ItemsContainer::size_type index, FileSystemBaseItem *entry);
	void updateSecondColumn(FileSystemBaseItem *entry);
	void updateSecondColumn(const RangeIntersection &range);
	void updateSecondColumn(ItemsContainer::size_type index, FileSystemBaseItem *entry);
	void updateBothColumns(FileSystemBaseItem *entry);
	void updateBothColumns(const RangeIntersection &range);
	void updateBothColumns(ItemsContainer::size_type index, FileSystemBaseItem *entry);
	void updateColumns(const RangeIntersection &range, int lastColumn);
	void removeEntry(ItemsContainer::size_type index);
	void removeEntry(const QModelIndex &index);

private:
	ItemsContainer m_items;
	FolderProxyModel m_proxy;
	FolderDelegate m_delegate;
	INodeView::MenuActionList m_menuActions;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODE_H_ */
