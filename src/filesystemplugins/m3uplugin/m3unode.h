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
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* INode */
	virtual FileSystem::INode *root() const;
	virtual int columnCount() const;
	virtual FileSystem::IFileControl *createControl() const { return 0; }
	virtual QString absolutePath(const QModelIndex &idx) const { return QString(); }

	/* INode::IFileInfo */
	virtual bool isDir() const { return false; }
	virtual bool isFile() const { return false; }
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual bool exists(IFileInfo *info) const;
	virtual void refresh();

	/* INode::IFileOperations */
	virtual void remove(const QModelIndexList &list);
	virtual void calculateSize(const QModelIndexList &list);
	virtual void pathToClipboard(const QModelIndexList &list);
	virtual void copy(const QModelIndexList &list, INode *destination);
	virtual void move(const QModelIndexList &list, INode *destination);
	virtual void createDirectory(const QString &fileName, QString &error);

	/* INode::IFileNavigation */
	virtual void viewClosed(FileSystem::INodeView *nodeView);
	virtual void viewParent(FileSystem::INodeView *nodeView);
	virtual void viewThis(FileSystem::INodeView *nodeView, const QModelIndex &selected);
	virtual void viewChild(FileSystem::INodeView *nodeView, const QModelIndex &idx, FileSystem::PluginsManager *plugins);
	virtual void viewChild(FileSystem::INodeView *nodeView, const FileSystem::Path::Iterator &path, FileSystem::PluginsManager *plugins);
	virtual void viewAbsolute(FileSystem::INodeView *nodeView, const QString &absoluteFilePath, FileSystem::PluginsManager *plugins);

	/* Node */
	virtual void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }
	virtual void switchTo(Node *node, const QModelIndex &selected);

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
