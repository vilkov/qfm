#include "pluginsmanager.h"
#include "../application.h"


FILESYSTEM_PLUGINS_NS_BEGIN

MyPluginsManager::MyPluginsManager() :
	PluginsManager()
{
	registerStatic(&m_m3uplugin,
			FileTypeIdList() <<
			Application::instance()->desktopEnvironment().fileTypeId(DesktopEnvironment::FileTypes::Audio::M3uFile));
	registerStatic(&m_idmplugin);
}

FILESYSTEM_PLUGINS_NS_END
