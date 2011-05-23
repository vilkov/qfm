#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include "filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

class RootNode : public Node
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode(PluginsManager *plugins);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;

	virtual IFile *open(IFile::OpenMode mode, QString &error) const;
	virtual IFileInfo *create(const QString &fileName, FileType type, QString &error) const;

	virtual void refresh();

	/* IFileOperations */
	virtual void remove(const QModelIndexList &list);
	virtual void calculateSize(const QModelIndexList &list);
	virtual void copy(const QModelIndexList &list, Node *destination);
	virtual void move(const QModelIndexList &list, Node *destination);

	/* Node */
	virtual bool isRootNode() const { return m_info.isRoot(); }
	virtual void view(INodeView *nodeView, const QModelIndex &selected);
	virtual void view(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins);
	virtual void view(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins);
	virtual void view(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins);

	virtual QModelIndex parentEntryIndex() const { return m_parentEntryIndex; }
	virtual void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }

	virtual QModelIndex rootIndex() const;
	virtual bool isRootIndex(const QModelIndex &index) const;

	PluginsManager *plugins() const { return m_plugins; }

private:
	PluginsManager *m_plugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
