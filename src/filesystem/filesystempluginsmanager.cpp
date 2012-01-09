#include "filesystempluginsmanager.h"
#include "../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

PluginsManager::PluginsManager()
{}

PluginsManager::~PluginsManager()
{
	qDeleteAll(m_dynamicPlugins);
}

Node *PluginsManager::node(const IFileInfo *info, Node *parent) const
{
	Node *res;

	if (info->isFile())
	{
		const PluginsList &list = m_filePlugins[info->id()];

		for (PluginsList::size_type i = 0, size = list.size(); i < size; ++i)
			if (res = list.at(i)->node(info, parent))
				return res;
	}

	for (PluginsList::size_type i = 0, size = m_otherPlugins.size(); i < size; ++i)
		if (res = m_otherPlugins.at(i)->node(info, parent))
			return res;

	return 0;
}

void PluginsManager::registerStatic(IPlugin *plugin)
{
	m_otherPlugins.push_back(plugin);
}

void PluginsManager::registerStatic(IFileReaderPlugin *plugin)
{
	IFileReaderPlugin::FileTypeIdList types = plugin->fileTypes();

	for (IFileReaderPlugin::FileTypeIdList::const_iterator i = types.constBegin(), end = types.constEnd(); i != end; ++i)
		m_filePlugins[*i].push_back(plugin);
}

void PluginsManager::registerDynamic(IPlugin *plugin)
{
	m_otherPlugins.push_back(plugin);
	m_dynamicPlugins.insert(plugin);
}

void PluginsManager::registerDynamic(IFileReaderPlugin *plugin)
{
	IFileReaderPlugin::FileTypeIdList types = plugin->fileTypes();

	for (IFileReaderPlugin::FileTypeIdList::const_iterator i = types.constBegin(), end = types.constEnd(); i != end; ++i)
	{
		m_filePlugins[*i].push_back(plugin);
		m_dynamicPlugins.insert(plugin);
	}
}

FILE_SYSTEM_NS_END
