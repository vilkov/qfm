#ifndef FILESYSTEMNODE_H_
#define FILESYSTEMNODE_H_

#include <QtCore/QAbstractItemModel>
#include "filesystem_ns.h"
#include "filesystempath.h"
#include "interfaces/filesysteminodeview.h"
#include "interfaces/filesystemifileinfo.h"
#include "interfaces/filesystemifileoperations.h"

FILE_SYSTEM_NS_BEGIN

class PluginsManager;

class Node : public QAbstractItemModel, public IFileInfo, public IFileOperations
{
	Q_DISABLE_COPY(Node)

public:
	Node(Node *parent = 0) :
		QAbstractItemModel(parent)
	{}

	virtual bool isRootNode() const = 0;
	virtual void view(INodeView *nodeView) = 0;
	virtual void view(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins) = 0;
	virtual void view(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins) = 0;

	virtual QModelIndex parentEntryIndex() const = 0;
	virtual void setParentEntryIndex(const QModelIndex &value) = 0;

	virtual QModelIndex rootIndex() const = 0;
	virtual bool isRootIndex(const QModelIndex &index) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMNODE_H_ */
