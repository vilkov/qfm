#include "default_cutaction.h"
#include <desktop/theme/desktop_theme.h>


DEFAULT_PLUGIN_NS_BEGIN

CutAction::CutAction() :
	SyncFileAction(::Desktop::Theme::current()->cutActionIcon(), tr("Cut"))
{}

void CutAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
