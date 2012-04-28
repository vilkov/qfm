#ifndef DEFAULTFOLDERNODE_H_
#define DEFAULTFOLDERNODE_H_

#include "base/defaultfoldernodebase.h"


DEFAULT_PLUGIN_NS_BEGIN

class FolderNode : public FolderNodeBase
{
	Q_DISABLE_COPY(FolderNode)

public:
	FolderNode(IFileContainer::Holder &container, Node *parent = 0);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;

protected:
	virtual Snapshot updateFilesList() const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERNODE_H_ */
