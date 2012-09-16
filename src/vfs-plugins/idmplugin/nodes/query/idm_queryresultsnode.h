#ifndef IDM_QUERYRESULTS_NODE_H_
#define IDM_QUERYRESULTS_NODE_H_

#include <vfs/tasks/vfs_tasksnode.h>
#include "idm_queryresultsdelegate.h"
#include "interface/idm_inodequeryresults.h"
#include "../../containeres/idm_container.h"
#include "../../storage/values/idm_valuereader.h"
#include "../../../../tools/containers/union.h"


IDM_PLUGIN_NS_BEGIN
class Functor;
class QueryResultValueItem;


class QueryResultsNode : public TasksNode, public IQueryResultsUpdater
{
public:
	QueryResultsNode(const IdmContainer &container, const Select &query, Node *parent = 0);

	/* QObject */
    virtual bool event(QEvent *event);

	/* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
    virtual void fetchMore(const QModelIndex &parent);
    virtual bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
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
	virtual ::History::Entry *search(const QModelIndex &index, INodeView *view);

	/* INode */
    virtual void refresh();
	virtual QString title() const;
	virtual QString location() const;
	virtual Sorting sorting() const;
	virtual Geometry geometry() const;
	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual ::History::Entry *menuAction(QAction *action, INodeView *view);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, QModelIndex &selected, bool newTab);
	virtual Node *viewChild(const QString &fileName, QModelIndex &selected);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const Item::Holder &item, quint64 timeElapsed);
	virtual void performActionEvent(const AsyncFileAction::FilesList &files, const QString &error);

protected:
	/* IQueryResultsUpdater */
	virtual void add(const QModelIndex &index, const IdmCompositeEntityValue::List &values);
	virtual void remove(const QModelIndex &index, const IdmCompositeEntityValue::List &values);
	virtual void refresh(const QModelIndex &index);

private:
	void process(const QModelIndexList &list, const Functor &functor);
	void processRemove(const QModelIndexList &list, const Functor &functor);
	void doRename(INodeView *view, const QModelIndex &index, QueryResultValueItem *value);
	void doRemove(INodeView *view, const QModelIndex &index, QueryResultValueItem *value);

private:
	typedef ::Tools::Containers::Union Union;

	void scanUpdates(const BaseTask::Event *e);
	void scanForRemove(const BaseTask::Event *e);
	void performRemove(const BaseTask::Event *e);

private:
    enum { PrefetchLimit = 64 };

private:
	class ItemsContainer : public Container
	{
	public:
		typedef QList<VFS::Item::Holder> List;

	public:
		ItemsContainer();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

	private:
		friend class QueryResultsNode;
		List m_container;
	};

private:
	void lock(const Snapshot &snapshot, const QString &reason);
	void unlock(const Snapshot &snapshot);
	void update(Snapshot &updates);

private:
    ItemsContainer m_itemsContainer;
    ItemsContainer::List &m_items;
    INodeView::MenuActionList m_actions;
	QueryResultsDelegate m_delegate;
	IdmContainer m_container;
	IdmValueReader m_reader;
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTS_NODE_H_ */
