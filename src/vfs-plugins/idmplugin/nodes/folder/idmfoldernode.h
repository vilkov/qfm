#ifndef IDMFOLDERNODE_H_
#define IDMFOLDERNODE_H_

#include "../../idmplugin_ns.h"
#include "../../containeres/idmcontainer.h"
#include "../../../default/nodes/default_node.h"


IDM_PLUGIN_NS_BEGIN

class IdmFolderNode : public Default::Node
{
public:
	IdmFolderNode(IFileContainer::Holder &container, const IdmContainer &storage, ::VFS::Node *parent = 0);

	/* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void rename(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndexList &list, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);
	virtual void move(const INodeView *source, INodeView *destination);
	virtual void removeToTrash(const QModelIndexList &list, INodeView *view);

protected:
	virtual ::VFS::Node *createNode(const IFileInfo *file) const;

private:
	friend class IdmRootNode;
	::VFS::Node *privateViewChild(const QString &fileName, QModelIndex &selected);

private:
	IdmContainer m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDMFOLDERNODE_H_ */
