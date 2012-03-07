#include "idmfoldernode.h"
#include "../../control/idmcopycontrol.h"
#include "../../gui/choose/choosefileentitydialog.h"
#include "../../../../application.h"
#include "../../../../filesystem/filesystempluginsmanager.h"


IDM_PLUGIN_NS_BEGIN

IdmFolderNode::IdmFolderNode(const IdmContainer &container, const Info &info, Node *parent) :
	FolderNode(info, parent),
	m_container(container)
{}

ICopyControl *IdmFolderNode::createControl(INodeView *view) const
{
	if (IdmEntity *entity = ChooseFileEntityDialog::choose(m_container, Application::mainWindow()))
		return new IdmCopyControl(m_container, entity, absoluteFilePath());
	else
		return 0;
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

void IdmFolderNode::move(const INodeView *source, INodeView *destination)
{

}

Node *IdmFolderNode::createNode(const Info &info, PluginsManager *plugins) const
{
	if (Node *res = plugins->node(this, &info, const_cast<IdmFolderNode*>(this)))
		return res;
	else
		if (info.isDir())
			return new IdmFolderNode(m_container, info, const_cast<IdmFolderNode*>(this));
		else
			return 0;
}

Node *IdmFolderNode::privateViewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return FolderNode::viewChild(fileName, plugins, selected);
}

IDM_PLUGIN_NS_END
