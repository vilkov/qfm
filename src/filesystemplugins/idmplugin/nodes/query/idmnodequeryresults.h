#ifndef IDMNODEQUERYRESULTS_H_
#define IDMNODEQUERYRESULTS_H_

#include "idmqueryresultsdelegate.h"
#include "interface/idminodequeryresults.h"
#include "../../containeres/idmcontainer.h"
#include "../../storage/values/idmvaluereader.h"
#include "../../../../filesystem/tasks/filesystemtasksnode.h"
#include "../../../../tools/containers/union.h"


IDM_PLUGIN_NS_BEGIN
class Functor;
class QueryResultValueItem;


class IdmNodeQueryResults : public TasksNode, public IQueryResultsUpdater
{
public:
	IdmNodeQueryResults(const IdmContainer &container, const Select &query, const Info &info, Node *parent = 0);
	virtual ~IdmNodeQueryResults();

	/* QObject */
    virtual bool event(QEvent *event);

	/* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
    virtual void fetchMore(const QModelIndex &parent);
    virtual bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual bool exists() const;
	virtual qint64 fileSize() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
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

	/* INode */
	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(TaskNodeItem::Base *item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(TaskNodeItem::Base *item, quint64 timeElapsed);

protected:
	/* IQueryResultsUpdater */
	virtual void add(const QModelIndex &index, const IdmCompositeEntityValue::List &values);
	virtual void remove(const QModelIndex &index, const IdmCompositeEntityValue::List &values);

private:
	void process(const QModelIndexList &list, const Functor &functor);
	void processRemove(const QModelIndexList &list, const Functor &functor);
	void doRename(INodeView *view, const QModelIndex &index, QueryResultValueItem *value);
	void doRemove(INodeView *view, const QModelIndex &index, QueryResultValueItem *value);

private:
	typedef ::Tools::Containers::Union Union;

	void scanForRemove(const BaseTask::Event *e);
	void performRemove(const BaseTask::Event *e);

private:
	void lock(const TasksItemList &list, const QString &reason);
	void lock(const ScanedFiles::List &list, const QString &reason);
	void unlock(const ScanedFiles::List &list);

private:
    enum { PrefetchLimit = 256 };

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
		friend class IdmNodeQueryResults;
		List m_container;
	};

private:
    ItemsContainer m_itemsContainer;
    ItemsContainer::List &m_items;
    INodeView::MenuActionList m_actions;
	IdmQueryResultsDelegate m_delegate;
	IdmContainer m_container;
	IdmValueReader m_reader;
	Info m_info;
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTS_H_ */
