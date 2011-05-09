#ifndef PLUGINSMANAGER_H_
#define PLUGINSMANAGER_H_

#include "m3uplugin/m3uplugin.h"
#include "../filesystem/filesystempluginsmanager.h"


class PluginsManager : public FileSystem::PluginsManager
{
public:
	PluginsManager();

private:
	M3uPlugin m_m3uplugin;
};

#endif /* PLUGINSMANAGER_H_ */
