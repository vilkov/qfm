#ifndef IDMNODEQUERYRESULTS_H_
#define IDMNODEQUERYRESULTS_H_

#include "../../items/idmbaseitem.h"
#include "../../model/idmdelegate.h"
#include "../../model/idmmodelcontainer.h"
#include "../../containeres/idmcontainer.h"
#include "../../storage/values/idmvaluereader.h"
#include "../../../../filesystem/tasks/filesystemtasksnode.h"


IDM_PLUGIN_NS_BEGIN

class IdmNodeQueryResults : public TasksNode
{
public:
	IdmNodeQueryResults(const IdmContainer &container, const Select &query, const Info &info, Node *parent = 0);
	virtual ~IdmNodeQueryResults();

    /* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
    virtual void fetchMore(const QModelIndex &parent);
    virtual bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
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

	/* INode */
	virtual IFileInfo *info(const QModelIndex &idx) const;
	virtual ICopyControl *createControl() const;

	/* IFileOperations */
	virtual void menuAction(QAction *action, INodeView *view);
	virtual void createFile(const QModelIndex &index);
	virtual void createDirectory(const QModelIndex &index);
	virtual void rename(const QModelIndexList &list);
	virtual void remove(const QModelIndexList &list);
	virtual void cancel(const QModelIndexList &list);
	virtual void calculateSize(const QModelIndexList &list);
	virtual void pathToClipboard(const QModelIndexList &list);
	virtual void copy(const QModelIndexList &list, INode *destination);
	virtual void move(const QModelIndexList &list, INode *destination);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual QAbstractItemModel *proxyModel() const;
	virtual QAbstractItemDelegate *itemDelegate() const;
	virtual const INodeView::MenuActionList &menuActions() const;

	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

private:
    enum { PrefetchLimit = 256 };

private:
    INodeView::MenuActionList m_actions;
	IdmModelContainer m_itemsContainer;
	IdmModelContainer::Container &m_items;
	IdmDelegate m_delegate;
	IdmContainer m_container;
	IdmValueReader m_reader;
	Info m_info;
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTS_H_ */
