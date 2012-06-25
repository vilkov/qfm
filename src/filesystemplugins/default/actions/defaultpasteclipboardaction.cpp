#include "defaultpasteclipboardaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

PasteClipboardAction::PasteClipboardAction() :
	SyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste from clipboard"))
{}

void PasteClipboardAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
