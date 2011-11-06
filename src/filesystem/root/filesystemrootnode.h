#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include "../filesystemnode.h"
#include "../folder/containers/filesystemitemscontainer.h"


FILE_SYSTEM_NS_BEGIN

class RootNode : public Node
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode();

	/* INode */
	virtual IFileInfo *info(const QModelIndex &idx) const { return 0; }
	virtual ICopyControl *createControl() const { return 0; }

	/* INode::IFileInfo */
	virtual bool isDir() const { return true; }
	virtual bool isFile() const { return false; }
	virtual bool exists() const { return true; }
	virtual QString fileName() const { return QString(); }
	virtual QString absolutePath() const { return QString(); }
	virtual QString absoluteFilePath() const { return QString(); }
	virtual QString absoluteFilePath(const QString &fileName) const { return QString(); }
	virtual QDateTime lastModified() const { return QDateTime(); }
	virtual void refresh() {}

	/* INode::IFileOperations */
	virtual void menuAction(QAction *action, INodeView *view) {}
	virtual void createFile(const QModelIndex &index, INodeView *view) {}
	virtual void createDirectory(const QModelIndex &index, INodeView *view) {}
	virtual void rename(const QModelIndexList &list, INodeView *view) {}
	virtual void remove(const QModelIndexList &list, INodeView *view) {}
	virtual void cancel(const QModelIndexList &list, INodeView *view) {}
	virtual void calculateSize(const QModelIndexList &list, INodeView *view) {}
	virtual void pathToClipboard(const QModelIndexList &list, INodeView *view) {}
	virtual void copy(const QModelIndexList &list, INode *destination, INodeView *view) {}
	virtual void move(const QModelIndexList &list, INode *destination, INodeView *view) {}

protected:
	/* Node */
	virtual QModelIndex rootIndex() const { return QModelIndex(); }
	virtual QAbstractItemModel *proxyModel() const { return 0; }
	virtual QAbstractItemDelegate *itemDelegate() const { return 0; }
	virtual const INodeView::MenuActionList &menuActions() const { return m_menuActions; }

	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected) { return 0; }
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);
	virtual void nodeRemoved(Node *node);

private:
	Node *createNode(const Info &info, PluginsManager *plugins);
	FileSystemBaseItem *createItem(const QString &fileName, PluginsManager *plugins);

private:
	ItemsContainer m_items;
	INodeView::MenuActionList m_menuActions;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
