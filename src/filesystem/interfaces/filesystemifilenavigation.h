#ifndef FILESYSTEMIFILENAVIGATION_H_
#define FILESYSTEMIFILENAVIGATION_H_

#include <QtCore/QString>
#include <QtCore/QModelIndex>
#include "filesysteminodeview.h"
#include "../filesystempath.h"
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class PluginsManager;

class IFileNavigation
{
public:
	virtual ~IFileNavigation() {}

	virtual void viewParent(INodeView *nodeView) = 0;
	virtual void viewThis(INodeView *nodeView, const QModelIndex &selected) = 0;
	virtual void viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins) = 0;
	virtual void viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins) = 0;
	virtual void viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILENAVIGATION_H_ */