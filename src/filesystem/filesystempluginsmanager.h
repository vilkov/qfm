#ifndef FILESYSTEMPLUGINSMANAGER_H_
#define FILESYSTEMPLUGINSMANAGER_H_

#include "filesystem_ns.h"
#include "filesystemnode.h"
#include "info/filesysteminfo.h"


FILE_SYSTEM_NS_BEGIN

class PluginsManager
{
	Q_DISABLE_COPY(PluginsManager)

public:
	class Plugin
	{
	public:
		virtual ~Plugin() {}
	};
	class FilePlugin : public Plugin
	{};
	class FolderPlugin : public Plugin
	{};
	typedef QList<Plugin*> PluginList;


public:
	PluginsManager();
	virtual ~PluginsManager();

	Node *node(const Info &info, Node *parent) const;

	void registerStatic(FilePlugin *plugin) { m_staticFilePlugins.push_back(plugin); }
	void registerDynamic(FilePlugin *plugin) { m_dynamicFilePlugins.push_back(plugin); }
	void registerStatic(const QString &filePath, FolderPlugin *plugin) { m_staticFoldersPlugins[filePath] = plugin; }
	void registerDynamic(const QString &filePath, FolderPlugin *plugin) { m_dynamicFoldersPlugins[filePath] = plugin; }

private:
	typedef QMap<QString, Plugin*> FoldersMap;

private:
	PluginList m_staticFilePlugins;
	PluginList m_dynamicFilePlugins;
	FoldersMap m_staticFoldersPlugins;
	FoldersMap m_dynamicFoldersPlugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPLUGINSMANAGER_H_ */
