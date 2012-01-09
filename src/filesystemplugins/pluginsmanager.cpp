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
	registerStatic(&m_arcplugin,
			FileTypeIdList() <<
			Application::instance()->desktopEnvironment().fileTypeId(DesktopEnvironment::FileTypes::Application::GZipFile));
}

FILESYSTEM_PLUGINS_NS_END
