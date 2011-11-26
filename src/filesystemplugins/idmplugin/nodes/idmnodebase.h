#ifndef IDMNODEBASE_H_
#define IDMNODEBASE_H_

#include "../idmplugin_ns.h"
#include "../functors/idmfunctors.h"
#include "../model/idmmodelcontainer.h"
#include "../containeres/idmcontainer.h"
#include "../../../filesystem/folder/filesystemfoldernode.h"


IDM_PLUGIN_NS_BEGIN

class IdmNodeBase : public FolderNode
{
public:
	IdmNodeBase(const IdmContainer &container, const Info &info, Node *parent = 0);

	/* INode::IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void menuAction(QAction *action, INodeView *view);
	virtual void createFile(const QModelIndex &index, INodeView *view);
	virtual void createDirectory(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndexList &list, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);
	virtual void cancel(const QModelIndexList &list, INodeView *view);
	virtual void calculateSize(const QModelIndexList &list, INodeView *view);
	virtual void pathToClipboard(const QModelIndexList &list, INodeView *view);
	virtual void copy(const INodeView *source, INodeView *destination);
	virtual void move(const INodeView *source, INodeView *destination);

protected:
	IdmContainer m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEBASE_H_ */
