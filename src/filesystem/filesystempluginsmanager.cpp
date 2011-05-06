#include "filesystempluginsmanager.h"
#include <QtCore/QStringList>


FILE_SYSTEM_NS_BEGIN

PluginsManager::PluginsManager()
{

}

PluginsManager::~PluginsManager()
{
	qDeleteAll(m_dynamicFilePlugins);
	qDeleteAll(m_dynamicFoldersPlugins);
}

Node *PluginsManager::node(const Info &info, Node *parent) const
{
	return 0;
}

FILE_SYSTEM_NS_END
