#ifndef FILESYSTEMINODEVIEW_H_
#define FILESYSTEMINODEVIEW_H_

#include <QtCore/QAbstractItemModel>
#include <QtGui/QAbstractItemDelegate>
#include <QtGui/QAbstractItemView>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class Node;

class INodeView
{
public:
	virtual ~INodeView() {}

	virtual void close() = 0;
	virtual void select(const QModelIndex &index) = 0;
	virtual void setNode(Node *node, QAbstractItemModel *model, QAbstractItemDelegate *delegate = 0) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINODEVIEW_H_ */
