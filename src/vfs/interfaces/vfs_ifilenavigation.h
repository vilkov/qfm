#ifndef VFS_IFILENAVIGATION_H_
#define VFS_IFILENAVIGATION_H_

#include <QtCore/QString>
#include <QtCore/QModelIndex>
#include "vfs_inodeview.h"
#include "../../history/historyentry.h"


VFS_NS_BEGIN

class PluginsManager;

class IFileNavigation
{
public:
	virtual ~IFileNavigation();

	virtual void viewClosed(INodeView *nodeView) = 0;
	virtual ::History::Entry *viewParent(INodeView *nodeView) = 0;
	virtual ::History::Entry *viewChild(INodeView *nodeView, const QModelIndex &idx) = 0;
	virtual ::History::Entry *viewInNewTab(INodeView *nodeView, const QModelIndex &idx) = 0;
	virtual void viewHistory(INodeView *nodeView, ::History::Entry *entry) = 0;
};

VFS_NS_END

#endif /* VFS_IFILENAVIGATION_H_ */
