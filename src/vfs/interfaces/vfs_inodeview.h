#ifndef VFS_INODEVIEW_H_
#define VFS_INODEVIEW_H_

#include <QtCore/QList>
#include <QtCore/QModelIndexList>
#include <QtCore/QAbstractItemModel>
#include <QtGui/QAbstractItemDelegate>
#include <QtGui/QAbstractItemView>
#include <QtGui/QAction>
#include "../vfs_ns.h"
#include "../../history/historyentry.h"


VFS_NS_BEGIN
class INode;


class INodeView
{
public:
    typedef QList<qint32>             Geometry;
    typedef QPair<int, Qt::SortOrder> Sorting;
	typedef QList<QAction*>           MenuActionList;

public:
	virtual ~INodeView();

	virtual INode *node() const = 0;
	virtual INodeView *opposite() const = 0;
	virtual QModelIndex currentIndex() const = 0;
	virtual QModelIndexList selectedIndexes() const = 0;

	virtual void save(::History::Entry *entry) = 0;
	virtual void edit(const QModelIndex &index) = 0;
	virtual void select(const QModelIndex &index) = 0;
	virtual void setNode(INode *node) = 0;
};

VFS_NS_END

#endif /* VFS_INODEVIEW_H_ */
