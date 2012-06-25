#include "defaultcopyaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

CopyAction::CopyAction() :
	SyncFileAction(Application::desktopService()->copyActionIcon(16), tr("Copy"))
{}

void CopyAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
