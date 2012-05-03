#ifndef FILESYSTEMINODE_H_
#define FILESYSTEMINODE_H_

#include "filesystemifilelocation.h"
#include "filesystemifileoperations.h"
#include "filesystemifilenavigation.h"


FILE_SYSTEM_NS_BEGIN

class INode : public IFileLocation, public IFileOperations, public IFileNavigation
{
public:
    typedef QList<qint32>             Geometry;
    typedef QPair<int, Qt::SortOrder> Sorting;

public:
	virtual ~INode();

    virtual void refresh() = 0;
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

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINODE_H_ */
