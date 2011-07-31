#ifndef PLUGINSMANAGER_H_
#define PLUGINSMANAGER_H_

#include "m3uplugin/m3uplugin.h"
#include "../filesystem/filesystempluginsmanager.h"


FILE_SYSTEM_NS_BEGIN

class MyPluginsManager : public PluginsManager
{
public:
	MyPluginsManager();

private:
	M3uPlugin m_m3uplugin;
};

FILE_SYSTEM_NS_END

#endif /* PLUGINSMANAGER_H_ */
