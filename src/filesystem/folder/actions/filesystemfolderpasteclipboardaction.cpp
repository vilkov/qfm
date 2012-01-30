#include "filesystemfolderpasteclipboardaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPasteClipboardAction::FolderPasteClipboardAction() :
	SyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste from clipboard"))
{}

void FolderPasteClipboardAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
