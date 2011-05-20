#ifndef M3UNODE_H_
#define M3UNODE_H_

#include <QtCore/QFileInfo>
#include "m3udelegate.h"
#include "m3uproxymodel.h"
#include "items/m3uitem.h"
#include "../../filesystem/filesystemnode.h"
#include "../../filesystem/filesystempluginsmanager.h"


class M3uNode : public FileSystem::Node
{
public:
	M3uNode(const QFileInfo &info, Node *parent = 0);
	virtual ~M3uNode();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* IFileInfo */
	virtual bool isDir() const { return false; }
	virtual bool isFile() const { return false; }
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;

	virtual FileSystem::IFile *open(FileSystem::IFile::OpenMode mode, QString &error) const { return 0; }
	virtual IFileInfo *create(const QString &fileName, FileType type, QString &error) const { return 0; }

	virtual void refresh();

	/* IFileOperations */
	virtual void remove(const QModelIndexList &list);
	virtual void calculateSize(const QModelIndexList &list);
	virtual void copy(const QModelIndexList &list, Node *destination);
	virtual void move(const QModelIndexList &list, Node *destination);

	/* Node */
	virtual bool isRootNode() const { return false; }
	virtual void view(FileSystem::INodeView *nodeView, const QModelIndex &selected);
	virtual void view(FileSystem::INodeView *nodeView, const QModelIndex &idx, FileSystem::PluginsManager *plugins);
	virtual void view(FileSystem::INodeView *nodeView, const FileSystem::Path::Iterator &path, FileSystem::PluginsManager *plugins);

	virtual QModelIndex parentEntryIndex() const { return m_parentEntryIndex; }
	virtual void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }

	virtual QModelIndex rootIndex() const;
	virtual bool isRootIndex(const QModelIndex &index) const;

protected:
	M3uItem *rootItem() const { return m_items.at(0); }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

private:
	typedef QList<M3uItem*> ItemsList;

private:
	QString m_tag;
	bool m_updating;
	ItemsList m_items;
	M3uProxyModel m_proxy;
	M3uDelegate m_delegate;
	QModelIndex m_parentEntryIndex;
};

#endif /* M3UNODE_H_ */
