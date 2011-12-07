#ifndef IDMROOTNODE_H_
#define IDMROOTNODE_H_

#include "idmrootnodedelegate.h"
#include "../../model/idmmodelcontainer.h"
#include "../../containeres/idmcontainer.h"
#include "../../../../filesystem/tasks/filesystemtasksnode.h"


IDM_PLUGIN_NS_BEGIN

class IdmRootNode : public TasksNode
{
public:
	IdmRootNode(const Info &storage, Node *parent = 0);

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
	enum Items
	{
		RootItem = 0,
		FilesItem = 1
	};

private:
	IdmModelContainer m_itemsContainer;
	IdmModelContainer::Container &m_items;
	IdmContainer m_container;
	IdmRootNodeDelegate m_delegate;
	Info m_info;
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODE_H_ */
