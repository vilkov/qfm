#ifndef DEFAULTNODE_H_
#define DEFAULTNODE_H_

#include "base/defaultbasenode.h"


DEFAULT_PLUGIN_NS_BEGIN

class Node : public BaseNode
{
	Q_DISABLE_COPY(Node)

public:
	Node(IFileContainer::Holder &container, ::VFS::Node *parent = 0);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;

protected:
	virtual Snapshot updateFilesList() const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTNODE_H_ */
