#include "pluginsmanager.h"


FILESYSTEM_PLUGINS_NS_BEGIN

MyPluginsManager::MyPluginsManager() :
	PluginsManager()
{
	registerStatic(&m_m3uplugin);
	registerStatic(&m_idmplugin);
	registerStatic(&m_arcplugin);

	/* XXX: Must be last in the list. */
	registerStatic(&m_default);
}

FILESYSTEM_PLUGINS_NS_END
