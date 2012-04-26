#ifndef IDMFOLDERNODE_H_
#define IDMFOLDERNODE_H_

#include "../../idmplugin_ns.h"
#include "../../containeres/idmcontainer.h"
#include "../../../default/defaultfoldernode.h"


IDM_PLUGIN_NS_BEGIN

class IdmFolderNode : public ::FileSystem::Plugins::Default::FolderNode
{
public:
	IdmFolderNode(IFileContainer::Holder &container, const IdmContainer &storage, Node *parent = 0);

	/* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void rename(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndexList &list, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);
	virtual void move(const INodeView *source, INodeView *destination);
	virtual void removeToTrash(const QModelIndexList &list, INodeView *view);

protected:
	virtual Node *createNode(const IFileInfo *file, PluginsManager *plugins) const;

private:
	friend class IdmRootNode;
	Node *privateViewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

private:
	IdmContainer m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDMFOLDERNODE_H_ */
