#include "idmfoldernode.h"
#include "../control/idmcopycontrol.h"
#include "../../gui/choose/chooseentitydialog.h"
#include "../../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmFolderNode::IdmFolderNode(IFileContainer::Holder &container, const IdmContainer &storage, FileSystem::Node *parent) :
	Default::Node(container, parent),
	m_container(storage)
{}

ICopyControl *IdmFolderNode::createControl(INodeView *view) const
{
	if (IdmEntity *entity = ChooseEntityDialog::chooseFile(m_container, Application::mainWindow()))
	{
		ICopyControl::Holder dest(container()->createControl(view));

		if (dest)
			return new IdmCopyControl(dest, m_container, entity);
	}

	return NULL;
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

void IdmFolderNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

FileSystem::Node *IdmFolderNode::createNode(const IFileInfo *file) const
{
	if (file->isDir())
	{
		QString error;
		IFileContainer::Holder folder(container()->open(file, error));

		if (folder)
			return new IdmFolderNode(folder, m_container, const_cast<IdmFolderNode *>(this));
		else
			QMessageBox::critical(Application::mainWindow(), tr("Error"), error);
	}
//	else
//		if (Node *res = plugins->node(container().data(), file, const_cast<IdmFolderNode *>(this)))
//			return res;

	return NULL;
}

FileSystem::Node *IdmFolderNode::privateViewChild(const QString &fileName, QModelIndex &selected)
{
	return Node::viewChild(fileName, selected);
}

IDM_PLUGIN_NS_END
