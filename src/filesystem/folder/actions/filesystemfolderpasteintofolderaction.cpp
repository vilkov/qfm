#include "filesystemfolderpasteintofolderaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPasteIntoFolderAction::FolderPasteIntoFolderAction() :
	AsyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste into folder"))
{}

bool FolderPasteIntoFolderAction::prepare(const FilesList &files)
{
	return false;
}

void FolderPasteIntoFolderAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

FILE_SYSTEM_NS_END
