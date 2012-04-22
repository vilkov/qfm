#include "defaultfolderpropertiesaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

FolderPropertiesAction::FolderPropertiesAction() :
	SyncFileAction(Application::desktopService()->propertiesActionIcon(16), tr("Properties..."))
{}

void FolderPropertiesAction::process(const IFileContainer *container, const FilesList &files) const
{
	Application::desktopService()->test(container->location(files.at(0).second->fileName()));
}

DEFAULT_PLUGIN_NS_END
