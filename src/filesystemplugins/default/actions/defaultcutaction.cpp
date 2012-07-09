#include "defaultcutaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

CutAction::CutAction() :
	SyncFileAction(Application::desktopService()->cutActionIcon(), tr("Cut"))
{}

void CutAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
