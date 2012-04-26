#ifndef PLUGINSMANAGER_H_
#define PLUGINSMANAGER_H_

#include "filesystemplugins_ns.h"
#include "default/defaultplugin.h"
#include "m3uplugin/m3uplugin.h"
#include "idmplugin/idmplugin.h"
#include "arcplugin/arcplugin.h"
#include "../filesystem/filesystempluginsmanager.h"


FILESYSTEM_PLUGINS_NS_BEGIN

class MyPluginsManager : public PluginsManager
{
public:
	MyPluginsManager();

private:
	M3u::Plugin m_m3uplugin;
	Idm::Plugin m_idmplugin;
	Arc::Plugin m_arcplugin;

private:
	Default::Plugin m_default;
};

FILESYSTEM_PLUGINS_NS_END

#endif /* PLUGINSMANAGER_H_ */
