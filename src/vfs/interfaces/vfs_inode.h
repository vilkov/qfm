#ifndef VFS_INODE_H_
#define VFS_INODE_H_

#include "vfs_ifileoperations.h"
#include "vfs_ifilenavigation.h"


VFS_NS_BEGIN

class INode : public IFileOperations, public IFileNavigation
{
public:
    typedef QList<qint32>             Geometry;
    typedef QPair<int, Qt::SortOrder> Sorting;

public:
	virtual ~INode();

    virtual void refresh() = 0;
	virtual QString location() const = 0;
	virtual QString title() const = 0;

	virtual Sorting sorting() const = 0;
	virtual Geometry geometry() const = 0;

	virtual int columnsCount() const = 0;
	virtual QAbstractItemModel *model() const = 0;
	virtual QAbstractItemDelegate *delegate() const = 0;
	virtual const INodeView::MenuActionList &actions() const = 0;
	virtual QAbstractItemView::SelectionMode selectionMode() const = 0;
	virtual ::History::Entry *menuAction(QAction *action, INodeView *view) = 0;

private:
	friend class Node;
	friend class TasksNode;
	virtual void addLink() = 0;
	virtual void addLinks(qint32 count) = 0;
	virtual void removeLink() = 0;
};

VFS_NS_END

#endif /* VFS_INODE_H_ */
