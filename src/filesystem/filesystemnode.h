#ifndef FILESYSTEMNODE_H_
#define FILESYSTEMNODE_H_

#include <QtCore/QAbstractItemModel>
#include <QtGui/QAbstractItemView>
#include "filesystem_ns.h"
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

	virtual void update() = 0;
	virtual Node *subnode(const QModelIndex &idx, PluginsManager *plugins) = 0;
	virtual void remove(Node *subnode) = 0;
	virtual void view(QAbstractItemView *itemView) = 0;
	virtual QModelIndex indexFor(const QString &fileName) = 0;

	virtual QModelIndex parentEntryIndex() const = 0;
	virtual void setParentEntryIndex(const QModelIndex &value) = 0;

	virtual QModelIndex rootIndex() const = 0;
	virtual bool isRootIndex(const QModelIndex &index) const = 0;

protected:
	friend class RootNode;
	virtual bool isRoot() const = 0;
	virtual Node *node(const QString &fileName, PluginsManager *plugins) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMNODE_H_ */
