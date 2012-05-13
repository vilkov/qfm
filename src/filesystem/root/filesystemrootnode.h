#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "../interfaces/filesysteminodeview.h"
#include "../interfaces/filesystemiplugin.h"
#include "../../history/historyentry.h"


FILE_SYSTEM_NS_BEGIN

class RootNode
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode();

	::History::Entry *open(INodeView *nodeView, const QString &uri);

protected:
	void registerStatic(IPlugin *plugin);
	void registerStatic(IContentPlugin *plugin);
	void registerStatic(IFileReaderPlugin *plugin);

private:
	typedef QList<IPlugin *>                PluginsList;
	typedef QMap<FileTypeId, PluginsList>   PluginsMap;
	typedef QMap<QString, IContentPlugin *> ContentPluginsMap;

private:
	PluginsMap m_filePlugins;
	PluginsList m_otherPlugins;
	ContentPluginsMap m_contentPlugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
