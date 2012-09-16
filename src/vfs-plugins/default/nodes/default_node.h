#ifndef DEFAULT_NODE_H_
#define DEFAULT_NODE_H_

#include "base/default_basenode.h"


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

#endif /* DEFAULT_NODE_H_ */
