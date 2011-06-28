#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include "filesystemnode.h"
#include "folder/containers/filesystemfoldernodevalues.h"


FILE_SYSTEM_NS_BEGIN

class RootNode : public Node
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode(PluginsManager *plugins);

	/* QAbstractItemModel */
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const { return 0; }
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const { return 0; }
	virtual QVariant data(const QModelIndex &index, int role) const { return QVariant(); }
	virtual Qt::ItemFlags flags(const QModelIndex &index) const { return Qt::NoItemFlags; }
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const { return QVariant(); }
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const { return QModelIndex(); }
	virtual QModelIndex parent(const QModelIndex &child) const { return QModelIndex(); }

	/* INode */
	virtual INode *root() const { return (INode*)this; }
	virtual int columnCount() const { return 0; }
	virtual IFileControl *createControl() const { return 0; }
	virtual QString absolutePath(const QModelIndex &idx) const { return QString(); }

	/* INode::IFileInfo */
	virtual bool isDir() const { return true; }
	virtual bool isFile() const { return false; }
	virtual bool exists() const { return true; }
	virtual QString fileName() const { return QString(); }
	virtual QString absolutePath() const { return QString(); }
	virtual QString absoluteFilePath() const { return QString(); }
	virtual QString absoluteFilePath(const QString &fileName) const { return QString(); }
	virtual QDateTime lastModified() const { return QDateTime(); }
	virtual bool exists(IFileInfo *info) const { return false; }
	virtual void refresh() {}

	/* INode::IFileOperations */
	virtual void remove(const QModelIndexList &list) {}
	virtual void calculateSize(const QModelIndexList &list) {}
	virtual void pathToClipboard(const QModelIndexList &list) {}
	virtual void copy(const QModelIndexList &list, INode *destination) {}
	virtual void move(const QModelIndexList &list, INode *destination) {}

	/* INode::IFileNavigation */
	virtual void viewClosed(INodeView *nodeView) {}
	virtual void viewParent(INodeView *nodeView) {}
	virtual void viewThis(INodeView *nodeView, const QModelIndex &selected) {}
	virtual void viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins) {}
	virtual void viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins);
	virtual void viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins);

	/* Node */
	virtual void setParentEntryIndex(const QModelIndex &value) {}
	virtual void removeThis() {}
	virtual void switchTo(Node *node, const QModelIndex &selected) {}
	virtual void removeEntry(Node *entry) {}

	PluginsManager *plugins() const { return m_plugins; }

private:
	Node *createNode(const Info &info, PluginsManager *plugins) const;
	Values::Value createNode(const QString &fileName, PluginsManager *plugins, Node *&node) const;

private:
	Values m_items;
	PluginsManager *m_plugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
