#include "defaultcopyaction.h"
#include <desktop/theme/desktop_theme.h>


DEFAULT_PLUGIN_NS_BEGIN

CopyAction::CopyAction() :
	SyncFileAction(::Desktop::Theme::current()->copyActionIcon(), tr("Copy"))
{}

void CopyAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
