#include "filesystemfolderpasteaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPasteAction::FolderPasteAction() :
	SyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste"))
{}

void FolderPasteAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
