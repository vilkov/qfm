#include "defaultpropertiesaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

PropertiesAction::PropertiesAction() :
	SyncFileAction(Application::desktopService()->propertiesActionIcon(), tr("Properties..."))
{}

void PropertiesAction::process(const IFileContainer *container, const FilesList &files) const
{
	Application::desktopService()->test(container->location(files.at(0).second->fileName()));
}

DEFAULT_PLUGIN_NS_END
