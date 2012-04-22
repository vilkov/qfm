#include "defaultfolderpasteclipboardaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

FolderPasteClipboardAction::FolderPasteClipboardAction() :
	SyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste from clipboard"))
{}

void FolderPasteClipboardAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
