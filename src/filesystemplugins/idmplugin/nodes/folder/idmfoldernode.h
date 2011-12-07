#ifndef IDMFOLDERNODE_H_
#define IDMFOLDERNODE_H_

#include "../../idmplugin_ns.h"
#include "../../../../filesystem/folder/filesystemfoldernode.h"


IDM_PLUGIN_NS_BEGIN

class IdmFolderNode : public FolderNode
{
public:
	IdmFolderNode(const Info &info, Node *parent = 0);

	/* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void menuAction(QAction *action, INodeView *view);
	virtual void createFile(const QModelIndex &index, INodeView *view);
	virtual void createDirectory(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndexList &list, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);
	virtual void cancel(const QModelIndexList &list, INodeView *view);
	virtual void calculateSize(const QModelIndexList &list, INodeView *view);
	virtual void pathToClipboard(const QModelIndexList &list, INodeView *view);
	virtual void copy(const INodeView *source, INodeView *destination);
	virtual void move(const INodeView *source, INodeView *destination);
};

IDM_PLUGIN_NS_END

#endif /* IDMFOLDERNODE_H_ */
