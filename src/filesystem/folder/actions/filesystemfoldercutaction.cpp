#include "filesystemfoldercutaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderCutAction::FolderCutAction() :
	SyncFileAction(Application::desktopService()->cutActionIcon(16), tr("Cut"))
{}

void FolderCutAction::process(const IFileContainer *container, const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
