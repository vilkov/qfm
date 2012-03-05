#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include "../filesystemnode.h"
#include "../interfaces/filesystemifilecontainer.h"
#include "../folder/containers/filesystemitemscontainer.h"


FILE_SYSTEM_NS_BEGIN

class RootNode : public Node, public IFileContainer
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode();

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual bool exists() const;
	virtual size_type fileSize() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
	virtual void refresh();

	/* IFileOperations */
	virtual IFileInfo *info(const QModelIndex &idx) const;
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view);
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

	/* INode */
	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;

	/* IFileContainer */
	virtual bool isPhysical() const;
	virtual QString location() const;
	virtual QString location(const QString &fileName) const;
	virtual IFileInfo::size_type freeSpace() const;

	virtual bool contains(const QString &fileName) const;
	virtual bool remove(const QString &fileName, QString &error) const;
	virtual bool rename(const QString &oldName, const QString &newName, QString &error);

	virtual IFileAccessor *open(const QString &fileName, int mode, QString &error) const;
	virtual IFileContainer *open(const QString &fileName, bool create, QString &error) const;

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
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
