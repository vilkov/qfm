#ifndef FILESYSTEMNODE_H_
#define FILESYSTEMNODE_H_

#include <QtCore/QAbstractItemModel>
#include "filesystem_ns.h"
#include "interfaces/filesysteminode.h"


FILE_SYSTEM_NS_BEGIN

class Node : public QAbstractItemModel, public INode
{
	Q_DISABLE_COPY(Node)

public:
	Node(Node *parent = 0) :
		QAbstractItemModel(parent)
	{}

	virtual void setParentEntryIndex(const QModelIndex &value) = 0;
	virtual void removeThis() = 0;
	virtual void switchTo(Node *node, const QModelIndex &selected) = 0;
	virtual void removeEntry(Node *entry) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMNODE_H_ */
