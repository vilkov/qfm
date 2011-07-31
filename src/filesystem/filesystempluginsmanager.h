#ifndef FILESYSTEMPLUGINSMANAGER_H_
#define FILESYSTEMPLUGINSMANAGER_H_

#include "filesystem_ns.h"
#include "filesystemnode.h"
#include "interfaces/filesystemifilecontrol.h"


FILE_SYSTEM_NS_BEGIN

class PluginsManager
{
	Q_DISABLE_COPY(PluginsManager)

public:
	class Plugin
	{
	public:
		virtual ~Plugin() {}

		virtual Node *node(const IFileInfo *info, Node *parent) const = 0;
		virtual Node *node(const IFileInfo *info, IFile *file, Node *parent) const = 0;
	};

public:
	PluginsManager();
	virtual ~PluginsManager();

	Node *node(const IFileControl *control, Node *parent) const;

	void registerStatic(Plugin *plugin) { m_staticPlugins.push_back(plugin); }
	void registerDynamic(Plugin *plugin) { m_dynamicPlugins.push_back(plugin); }

private:
	typedef QList<Plugin*> PluginsList;

private:
	PluginsList m_staticPlugins;
	PluginsList m_dynamicPlugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPLUGINSMANAGER_H_ */
