#include "pluginsmanager.h"


PluginsManager::PluginsManager() :
	FileSystem::PluginsManager()
{
	registerStatic(&m_m3uplugin);
}
