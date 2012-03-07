#ifndef IDMFOLDERNODE_H_
#define IDMFOLDERNODE_H_

#include "../../idmplugin_ns.h"
#include "../../containeres/idmcontainer.h"
#include "../../../../filesystem/folder/filesystemfoldernode.h"


IDM_PLUGIN_NS_BEGIN

class IdmFolderNode : public FolderNode
{
public:
	IdmFolderNode(const IdmContainer &container, const Info &info, Node *parent = 0);

	/* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void menuAction(QAction *action, INodeView *view);
	virtual void rename(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndexList &list, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);
	virtual void move(const INodeView *source, INodeView *destination);

protected:
	virtual Node *createNode(const Info &info, PluginsManager *plugins) const;

private:
	friend class IdmRootNode;
	Node *privateViewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

private:
	IdmContainer m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDMFOLDERNODE_H_ */
