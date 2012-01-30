#include "filesystemfoldercopyaction.h"


FILE_SYSTEM_NS_BEGIN

FolderCopyAction::FolderCopyAction()
{}

const QAction *FolderCopyAction::action() const
{
	return &m_action;
}

void FolderCopyAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
