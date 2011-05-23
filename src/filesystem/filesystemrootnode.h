#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include "folder/filesystemfoldernodebase.h"


FILE_SYSTEM_NS_BEGIN

class RootNode : public FolderNodeBase
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

	/* IFileInfo */
	virtual bool isDir() const { return true; }
	virtual bool isFile() const { return false; }
	virtual bool exists() const { return true; }
	virtual QString fileName() const { return QString(); }
	virtual QString absolutePath() const { return QString(); }
	virtual QString absoluteFilePath() const { return QString(); }
	virtual QString absoluteFilePath(const QString &fileName) const { return QString(); }
	virtual QDateTime lastModified() const { return QDateTime(); }

	virtual IFile *open(IFile::OpenMode mode, QString &error) const { return 0; }
	virtual IFileInfo *create(const QString &fileName, FileType type, QString &error) const { return 0; }

	virtual void refresh() {}

	/* IFileOperations */
	virtual void remove(const QModelIndexList &list) {}
	virtual void calculateSize(const QModelIndexList &list) {}
	virtual void copy(const QModelIndexList &list, Node *destination) {}
	virtual void move(const QModelIndexList &list, Node *destination) {}

	/* Node */
	virtual bool isRootNode() const { return true; }
	virtual void view(INodeView *nodeView, const QModelIndex &selected) {}
	virtual void view(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins) {}
	virtual void view(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins);
	virtual void view(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins);

	virtual QModelIndex parentEntryIndex() const { return QModelIndex(); }
	virtual void setParentEntryIndex(const QModelIndex &value) {}

	virtual QModelIndex rootIndex() const { return QModelIndex(); }
	virtual bool isRootIndex(const QModelIndex &index) const { return false; }

	PluginsManager *plugins() const { return m_plugins; }

private:
	Node *createNode(const Info &info, PluginsManager *plugins) const;
	Values::Value createNode(const QString &fileName, PluginsManager *plugins, Node *&node) const;

private:
	PluginsManager *m_plugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
