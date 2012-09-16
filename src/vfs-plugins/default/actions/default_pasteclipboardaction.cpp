#include "default_pasteclipboardaction.h"
#include <desktop/theme/desktop_theme.h>


DEFAULT_PLUGIN_NS_BEGIN

PasteClipboardAction::PasteClipboardAction() :
	SyncFileAction(::Desktop::Theme::current()->pasteActionIcon(), tr("Paste from clipboard"))
{}

void PasteClipboardAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
