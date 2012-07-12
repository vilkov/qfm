#ifndef VFS_INODEVIEW_H_
#define VFS_INODEVIEW_H_

#include <QtCore/QList>
#include <QtCore/QAbstractItemModel>
#include <QtGui/QAbstractItemDelegate>
#include <QtGui/QAbstractItemView>
#include <QtGui/QAction>
#include "../vfs_ns.h"


VFS_NS_BEGIN
class INode;


class INodeView
{
public:
	typedef QList<QAction*> MenuActionList;

public:
	virtual ~INodeView();

	virtual INode *node() const = 0;
	virtual QModelIndex currentIndex() const = 0;
	virtual QModelIndexList selectedIndexes() const = 0;

	virtual void edit(const QModelIndex &index) = 0;
	virtual void select(const QModelIndex &index) = 0;
	virtual void setNode(INode *node) = 0;
};

VFS_NS_END

#endif /* VFS_INODEVIEW_H_ */
