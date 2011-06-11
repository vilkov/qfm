#ifndef FILESYSTEMFOLDERNODE_H_
#define FILESYSTEMFOLDERNODE_H_

#include "filesystemfolderdelegate.h"
#include "filesystemfolderproxymodel.h"
#include "events/filesystemmodelevent.h"
#include "info/filesystemfoldernodeinfo.h"
#include "containers/filesystemupdateslist.h"
#include "containers/filesystemfoldernodevalues.h"
#include "functors/filesystemfoldernodefunctors.h"
#include "../filesystemnode.h"
#include "../../tools/metatemplates.h"


FILE_SYSTEM_NS_BEGIN

class FolderNode : public Node
{
	Q_DISABLE_COPY(FolderNode)

public:
	FolderNode(const Info &info, Node *parent = 0);

    virtual bool event(QEvent *e);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* INode */
	virtual int columnCount() const;

	/* INode::IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual bool exists(IFileInfo *info) const;

	virtual IFile *open(IFile::OpenMode mode, QString &error) const;
	virtual void close(IFile *file) const;

	virtual IFileInfo *create(IFileInfo *info, QString &error) const;
	virtual IFileInfo *create(const QString &fileName, FileType type, QString &error) const;
	virtual void close(IFileInfo *info) const;

	virtual void refresh();

	/* INode::IFileOperations */
	virtual void remove(const QModelIndexList &list);
	virtual void calculateSize(const QModelIndexList &list);
	virtual void copy(const QModelIndexList &list, INode *destination);
	virtual void move(const QModelIndexList &list, INode *destination);

	/* INode::IFileNavigation */
	virtual void viewParent(INodeView *nodeView);
	virtual void viewThis(INodeView *nodeView, const QModelIndex &selected);
	virtual void viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins);
	virtual void viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins);
	virtual void viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins);

	/* Node */
	virtual void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }
	virtual void removeThis();
	virtual void switchTo(Node *node, const QModelIndex &selected);
	virtual void removeEntry(Node *entry);

protected:
	void processIndexList(const QModelIndexList &list, const FolderNodeFunctors::Functor &functor);
	void removeFunctor(FolderNodeItem *entry);
	void calculateSizeFunctor(FolderNodeItem *entry);
	void copyFunctor(FolderNodeItem *entry, INode *destination);
	void moveFunctor(FolderNodeItem *entry, INode *destination);

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

	void copyEntry(FolderNodeItem *entry, INode *destination, bool move);
	void scanForCopy(FolderNodeItem *entry, INode *destination, bool move);
	void scanForCopyEvent(const ModelEvent::Params *p);
	void copyCompleteEvent(const ModelEvent::Params *p);

	void questionAnswerEvent(const ModelEvent::Params *p);
	void updateProgressEvent(const ModelEvent::Params *p);

private:
	QModelIndex index(int column, FolderNodeItem *item) const;
	Node *createNode(const Info &info, PluginsManager *plugins) const;
	QModelIndex indexForFile(FolderNodeItem *item) const;
	QModelIndex indexForFile(FolderNodeItem *item, Values::size_type index) const;
	QModelIndex rootIndex() const;

	void updateFirstColumn(FolderNodeItem *entry);
	void updateSecondColumn(FolderNodeItem *entry);
	void updateBothColumns(FolderNodeItem *entry);
	void removeEntry(Values::size_type index);
	void removeEntry(const QModelIndex &index);

	void switchTo(Node *node, INodeView *nodeView, const QModelIndex &selected);
	void addView(INodeView *view);
	void removeView(INodeView *view);

private:
	typedef QSet<INodeView*> SetView;

private:
	bool m_updating;
	Info m_info;
	Values m_items;
	SetView m_view;
	FolderProxyModel m_proxy;
	FolderDelegate m_delegate;
	QModelIndex m_parentEntryIndex;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODE_H_ */
