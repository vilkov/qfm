#include "filesystemfoldercopyaction.h"


FILE_SYSTEM_NS_BEGIN

FolderCopyAction::FolderCopyAction() :
	m_action(tr("Copy"), 0)
{
	m_action.setData(qVariantFromValue(static_cast<void *>(this)));
}

const QAction *FolderCopyAction::action() const
{
	return &m_action;
}

void FolderCopyAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
