#ifndef FILESYSTEMIFILENAVIGATION_H_
#define FILESYSTEMIFILENAVIGATION_H_

#include <QtCore/QString>
#include <QtCore/QModelIndex>
#include "filesysteminodeview.h"
#include "../../history/historyentry.h"


FILE_SYSTEM_NS_BEGIN

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

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILENAVIGATION_H_ */
