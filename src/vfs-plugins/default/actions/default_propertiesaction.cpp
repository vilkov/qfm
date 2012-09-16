#include "default_propertiesaction.h"
#include <desktop/theme/desktop_theme.h>


DEFAULT_PLUGIN_NS_BEGIN

PropertiesAction::PropertiesAction() :
	SyncFileAction(::Desktop::Theme::current()->propertiesActionIcon(), tr("Properties..."))
{}

void PropertiesAction::process(const IFileContainer *container, const FilesList &files) const
{

}

DEFAULT_PLUGIN_NS_END
