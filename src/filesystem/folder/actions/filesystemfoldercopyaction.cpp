#include "filesystemfoldercopyaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderCopyAction::FolderCopyAction() :
	SyncFileAction(Application::desktopService()->copyActionIcon(16), tr("Copy"))
{}

void FolderCopyAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
