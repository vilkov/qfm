#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include "filesystemnode.h"
#include "folder/containers/filesystemfoldernodevalues.h"


FILE_SYSTEM_NS_BEGIN

class RootNode : public Node
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode();

	/* QAbstractItemModel */
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const { return 0; }
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const { return 0; }
	virtual QVariant data(const QModelIndex &index, int role) const { return QVariant(); }
	virtual Qt::ItemFlags flags(const QModelIndex &index) const { return Qt::NoItemFlags; }
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const { return QVariant(); }
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const { return QModelIndex(); }
	virtual QModelIndex parent(const QModelIndex &child) const { return QModelIndex(); }

	/* INode */
	virtual IFileInfo *info(const QModelIndex &idx) const { return 0; }
	virtual IFileControl *createControl() const { return 0; }
	virtual IFileControl *createControl(const QModelIndex &idx, PluginsManager *plugins) { return 0; }

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
	virtual void remove(const QModelIndexList &list) {}
	virtual void cancel(const QModelIndexList &list) {}
	virtual void calculateSize(const QModelIndexList &list) {}
	virtual void pathToClipboard(const QModelIndexList &list) {}
	virtual void copy(const QModelIndexList &list, INode *destination) {}
	virtual void move(const QModelIndexList &list, INode *destination) {}

protected:
	/* Node */
	virtual QModelIndex rootIndex() const { return QModelIndex(); }
	virtual QAbstractItemModel *proxyModel() const { return 0; }
	virtual QAbstractItemDelegate *itemDelegate() const { return 0; }

	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected) { return 0; }
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

private:
	Node *createNode(const Info &info, PluginsManager *plugins) const;
	Values::Value createNode(const QString &fileName, PluginsManager *plugins) const;

private:
	Values m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
