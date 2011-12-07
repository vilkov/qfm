#include "idmfoldernode.h"


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

IDM_PLUGIN_NS_END
