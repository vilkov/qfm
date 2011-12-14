#include "idmfoldernode.h"
#include "../../../../filesystem/filesystempluginsmanager.h"


IDM_PLUGIN_NS_BEGIN

IdmFolderNode::IdmFolderNode(const Info &info, Node *parent) :
	FolderNode(info, parent)
{}

ICopyControl *IdmFolderNode::createControl(INodeView *view) const
{
	return 0;
}

void IdmFolderNode::menuAction(QAction *action, INodeView *view)
{

}

void IdmFolderNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void IdmFolderNode::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void IdmFolderNode::rename(const QModelIndex &index, INodeView *view)
{

}

void IdmFolderNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void IdmFolderNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void IdmFolderNode::cancel(const QModelIndexList &list, INodeView *view)
{

}

void IdmFolderNode::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void IdmFolderNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void IdmFolderNode::copy(const INodeView *source, INodeView *destination)
{

}

void IdmFolderNode::move(const INodeView *source, INodeView *destination)
{

}

Node *IdmFolderNode::createNode(const Info &info, PluginsManager *plugins) const
{
	if (Node *res = plugins->node(&info, const_cast<IdmFolderNode*>(this)))
		return res;
	else
		if (info.isDir())
			return new IdmFolderNode(info, const_cast<IdmFolderNode*>(this));
		else
			return 0;
}

Node *IdmFolderNode::privateViewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return FolderNode::viewChild(fileName, plugins, selected);
}

IDM_PLUGIN_NS_END
