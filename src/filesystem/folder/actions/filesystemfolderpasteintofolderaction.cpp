#include "filesystemfolderpasteintofolderaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPasteIntoFolderAction::FolderPasteIntoFolderAction() :
	AsyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste into folder")),
	m_lockReason(tr("Copying..."))
{}

const QString &FolderPasteIntoFolderAction::lockReason() const
{
	return m_lockReason;
}

PerformActionTask *FolderPasteIntoFolderAction::process(TasksNode *receiver, const FilesList &files) const
{
	return NULL;
}

FILE_SYSTEM_NS_END
