#include "pluginsmanager.h"


FILESYSTEM_PLUGINS_NS_BEGIN

MyPluginsManager::MyPluginsManager() :
	PluginsManager()
{
	registerStatic(&m_m3uplugin);
	registerStatic(&m_idmplugin);
	registerStatic(&m_arcplugin);
}

FILESYSTEM_PLUGINS_NS_END
