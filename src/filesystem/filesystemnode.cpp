#include "filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

Node::Node(Node *parent) :
	QAbstractItemModel(parent)
{}

INode *Node::root() const
{
	if (INode *res = static_cast<Node*>(Node::parent()))
		return res->root();
	else
		return (INode*)this;
}

int Node::columnsCount() const
{
	return columnCount(QModelIndex());
}

FILE_SYSTEM_NS_END
