#ifndef IDMNODEQUERYRESULTS_H_
#define IDMNODEQUERYRESULTS_H_

#include "idmqueryresultsdelegate.h"
#include "interface/idminodequeryresults.h"
#include "../../items/idmbaseitem.h"
#include "../../model/idmmodelcontainer.h"
#include "../../containeres/idmcontainer.h"
#include "../../storage/values/idmvaluereader.h"
#include "../../../../filesystem/tasks/filesystemtasksnode.h"


IDM_PLUGIN_NS_BEGIN
class Functor;
class QueryResultValueItem;


class IdmNodeQueryResults : public TasksNode, public IQueryResultsUpdater
{
public:
	IdmNodeQueryResults(const IdmContainer &container, const Select &query, const Info &info, Node *parent = 0);

    /* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
    virtual void fetchMore(const QModelIndex &parent);
    virtual bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
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

	/* IQueryResultsUpdater */
	virtual void add(const QModelIndex &index, const IdmCompositeEntityValue::List &values);
	virtual void remove(const QModelIndex &index, const IdmCompositeEntityValue::List &values);

private:
	void process(const QModelIndexList &list, const Functor &functor);
	void doRename(INodeView *view, const QModelIndex &index, QueryResultValueItem *value);

private:
    enum { PrefetchLimit = 256 };

private:
    INodeView::MenuActionList m_actions;
	IdmModelContainer m_itemsContainer;
	IdmModelContainer::Container &m_items;
	IdmQueryResultsDelegate m_delegate;
	IdmContainer m_container;
	IdmValueReader m_reader;
	Info m_info;
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTS_H_ */
