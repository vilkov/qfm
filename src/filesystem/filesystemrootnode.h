#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include "folder/filesystemfoldernode.h"


FILE_SYSTEM_NS_BEGIN

class RootNode : public FolderNode
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode(PluginsManager *plugins);

	PluginsManager *plugins() const { return m_plugins; }
//	void view(const QString &absoluteFilePath, INodeView *nodeView, QModelIndex &index);

protected:
	virtual bool isRoot() const { return true; }

private:
	PluginsManager *m_plugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
