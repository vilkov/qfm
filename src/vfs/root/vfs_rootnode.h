#ifndef VFS_ROOTNODE_H_
#define VFS_ROOTNODE_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "../interfaces/vfs_inodeview.h"
#include "../interfaces/vfs_iplugin.h"
#include "../../history/historyentry.h"


VFS_NS_BEGIN

class RootNode
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode();
	virtual ~RootNode();

	::History::Entry *open(INodeView *nodeView, const QString &uri, const QString &currentFile = QString()) const;
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

VFS_NS_END

#endif /* VFS_ROOTNODE_H_ */
