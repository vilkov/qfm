#ifndef FILESYSTEMFOLDERNODE_H_
#define FILESYSTEMFOLDERNODE_H_

#include "filesystemfoldernodebase.h"
#include "filesystemchangeslist.h"
#include "filesystemfolderdelegate.h"
#include "filesystemfolderproxymodel.h"
#include "events/filesystemmodelevent.h"
#include "info/filesystemfoldernodeinfo.h"
#include "functors/filesystemfoldernodefunctors.h"
#include "../../tools/metatemplates.h"


FILE_SYSTEM_NS_BEGIN

class FolderNode : public FolderNodeBase
{
	Q_DISABLE_COPY(FolderNode)

public:
	FolderNode(const Info &info, Node *parent = 0);

    virtual bool event(QEvent *e);

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

protected:
	void processIndexList(const QModelIndexList &list, const Functors::Functor &functor);
	void removeFunctor(FolderNodeItem *entry);
	void calculateSizeFunctor(FolderNodeItem *entry);
	void copyFunctor(FolderNodeItem *entry, Node *destination);
	void moveFunctor(FolderNodeItem *entry, Node *destination);

protected:
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

protected:
	void list(FolderNodeItem *fileSystemTree);
	void listEvent(const ModelEvent::Params *p);

	void updateFiles();
	void updateFilesEvent(const ModelEvent::Params *p);

	void removeEntry(FolderNodeItem *entry);
	void scanForRemove(FolderNodeItem *entry);
	void scanForRemoveEvent(const ModelEvent::Params *p);
	void removeCompleteEvent(const ModelEvent::Params *p);
	void removeCanceledEvent(const ModelEvent::Params *p);

	void scanForSize(FolderNodeItem *entry);
	void scanForSizeEvent(const ModelEvent::Params *p);

	void copyEntry(FolderNodeItem *entry, Node *destination);
	void scanForCopy(FolderNodeItem *entry, Node *destination);
	void moveEntry(FolderNodeItem *entry, Node *destination);
	void scanForMove(FolderNodeItem *entry, Node *destination);
	void scanForCopyEvent(const ModelEvent::Params *p);
	void scanForMoveEvent(const ModelEvent::Params *p);
	void copyCompleteEvent(const ModelEvent::Params *p);
	void copyCanceledEvent(const ModelEvent::Params *p);

	void questionAnswerEvent(const ModelEvent::Params *p);
	void updateProgressEvent(const ModelEvent::Params *p);

private:
	ChangesList makeChangeSet() const;
	QModelIndex index(int column, FolderNodeItem *item) const;
	Node *createNode(const Info &info, PluginsManager *plugins) const;
	Values::Value createNode(const QString &fileName, PluginsManager *plugins, Node *&node) const;
	QModelIndex indexForFile(FolderNodeItem *item);

	void updateFirstColumn(FolderNodeItem *entry);
	void updateSecondColumn(FolderNodeItem *entry);
	void updateBothColumns(FolderNodeItem *entry);
	void removeEntry(Values::size_type index);
	void removeEntry(const QModelIndex &index);

	void switchTo(Node *node);
	void addView(INodeView *view);
	void removeView(INodeView *view);

private:
	typedef QSet<INodeView*> SetView;

private:
	bool m_updating;
	Info m_info;
	SetView m_view;
	FolderProxyModel m_proxy;
	FolderDelegate m_delegate;
	QModelIndex m_parentEntryIndex;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODE_H_ */
