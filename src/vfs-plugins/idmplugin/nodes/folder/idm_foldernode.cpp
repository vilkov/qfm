#include "idm_foldernode.h"
#include "../control/idm_copycontrol.h"
#include "../../gui/choose/idm_chooseentitydialog.h"
#include "../../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

FolderNode::FolderNode(IFileContainer::Holder &container, const IdmContainer &storage, ::VFS::Node *parent) :
	Default::Node(container, parent),
	m_container(storage)
{}

ICopyControl *FolderNode::createControl(INodeView *view) const
{
	if (Entity *entity = ChooseEntityDialog::chooseFile(m_container, Application::mainWindow()))
	{
		ICopyControl::Holder dest(container()->createControl(view));

		if (dest)
			return new CopyControl(dest, m_container, entity);
	}

	return NULL;
}

void FolderNode::rename(const QModelIndex &index, INodeView *view)
{

}

void FolderNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void FolderNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void FolderNode::move(const INodeView *source, INodeView *destination)
{

}

void FolderNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

::VFS::Node *FolderNode::createNode(const IFileInfo *file) const
{
	if (file->isDir())
	{
		QString error;
		IFileContainer::Holder folder(container()->open(file, error));

		if (folder)
			return new FolderNode(folder, m_container, const_cast<FolderNode *>(this));
		else
			QMessageBox::critical(Application::mainWindow(), tr("Error"), error);
	}
//	else
//		if (Node *res = plugins->node(container().data(), file, const_cast<IdmFolderNode *>(this)))
//			return res;

	return NULL;
}

::VFS::Node *FolderNode::privateViewChild(const QString &fileName, QModelIndex &selected)
{
	return Node::viewChild(fileName, selected);
}

IDM_PLUGIN_NS_END
