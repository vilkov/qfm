#include "filesystemfolderpropertiesaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPropertiesAction::FolderPropertiesAction() :
	SyncFileAction(Application::desktopService()->propertiesActionIcon(16), tr("Properties..."))
{}

void FolderPropertiesAction::process(const IFileContainer *container, const FilesList &files) const
{
	Application::desktopService()->test();
}

FILE_SYSTEM_NS_END
