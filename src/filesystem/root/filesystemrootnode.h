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

	::History::Entry *open(INodeView *nodeView, const QString &uri) const;
	Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

protected:
	void registerStatic(IFilePlugin *plugin);
	void registerStatic(IContainerPlugin *plugin);

private:
	typedef QList<IFilePlugin *>              PluginsList;
	typedef QMap<FileTypeId, PluginsList>     PluginsMap;
	typedef QMap<QString, IContainerPlugin *> ContentPluginsMap;

private:
	PluginsMap m_fileTypePlugins;
	ContentPluginsMap m_containerPlugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
