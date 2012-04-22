#include "defaultfoldercutaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

FolderCutAction::FolderCutAction() :
	SyncFileAction(Application::desktopService()->cutActionIcon(16), tr("Cut"))
{}

void FolderCutAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
