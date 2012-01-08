#ifndef FILESYSTEMPLUGINSMANAGER_H_
#define FILESYSTEMPLUGINSMANAGER_H_

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QList>
#include "filesystem_ns.h"
#include "filesystemnode.h"
#include "interfaces/filesystemiplugin.h"
#include "../de/filesystemfiletypeinfo.h"


FILE_SYSTEM_NS_BEGIN

class PluginsManager
{
	Q_DISABLE_COPY(PluginsManager)

public:
	PluginsManager();
	virtual ~PluginsManager();

	Node *node(const IFileInfo *info, Node *parent) const;

protected:
	typedef QList<FileTypeId> FileTypeIdList;

	void registerStatic(IPlugin *plugin);
	void registerStatic(IPlugin *plugin, const FileTypeIdList &types);

	void registerDynamic(IPlugin *plugin);
	void registerDynamic(IPlugin *plugin, const FileTypeIdList &types);

private:
	typedef QSet<IPlugin*>                PluginsSet;
	typedef QList<IPlugin*>               PluginsList;
	typedef QMap<FileTypeId, PluginsList> PluginsMap;

private:
	PluginsMap m_filePlugins;
	PluginsList m_otherPlugins;
	PluginsSet m_dynamicPlugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPLUGINSMANAGER_H_ */
