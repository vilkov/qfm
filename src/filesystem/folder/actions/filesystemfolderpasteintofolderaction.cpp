#include "filesystemfolderpasteintofolderaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPasteIntoFolderAction::FolderPasteIntoFolderAction() :
	SyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste into folder"))
{}

void FolderPasteIntoFolderAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
