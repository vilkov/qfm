#include "pluginsmanager.h"


FILE_SYSTEM_NS_BEGIN

MyPluginsManager::MyPluginsManager() :
	PluginsManager()
{
	registerStatic(&m_m3uplugin);
	registerStatic(&m_idmplugin);
}

FILE_SYSTEM_NS_END
