#include "filesystemfolderpropertiesaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPropertiesAction::FolderPropertiesAction() :
	SyncFileAction(Application::desktopService()->propertiesActionIcon(16), tr("Properties..."))
{}

void FolderPropertiesAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
