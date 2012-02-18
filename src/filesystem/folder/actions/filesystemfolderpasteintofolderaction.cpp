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

bool FolderPasteIntoFolderAction::prepare(const FilesList &files)
{
	return false;
}

void FolderPasteIntoFolderAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &aborted) const
{

}

FILE_SYSTEM_NS_END
