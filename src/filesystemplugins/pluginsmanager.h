#ifndef PLUGINSMANAGER_H_
#define PLUGINSMANAGER_H_

#include "filesystemplugins_ns.h"
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
	M3u::M3uPlugin m_m3uplugin;
	Idm::IdmPlugin m_idmplugin;
	Arc::ArcPlugin m_arcplugin;
};

FILESYSTEM_PLUGINS_NS_END

#endif /* PLUGINSMANAGER_H_ */
