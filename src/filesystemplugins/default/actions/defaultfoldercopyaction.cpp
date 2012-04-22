#include "defaultfoldercopyaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

FolderCopyAction::FolderCopyAction() :
	SyncFileAction(Application::desktopService()->copyActionIcon(16), tr("Copy"))
{}

void FolderCopyAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
