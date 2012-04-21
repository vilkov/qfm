#ifndef FILESYSTEMFOLDERNODE_H_
#define FILESYSTEMFOLDERNODE_H_

#include "filesystemrootfoldernode.h"


FILE_SYSTEM_NS_BEGIN

class FolderNode : public RootFolderNode
{
	Q_DISABLE_COPY(FolderNode)

public:
	FolderNode(IFileContainer::Holder &container, Node *parent = 0);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;

protected:
	/* FolderNodeBase */
	virtual UpdatesList::Changes updateFilesMap() const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODE_H_ */
