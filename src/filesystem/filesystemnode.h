#ifndef FILESYSTEMNODE_H_
#define FILESYSTEMNODE_H_

#include <QtCore/QAbstractItemModel>
#include <QtGui/QAbstractItemView>
#include "filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class PluginsManager;

class Node : public QAbstractItemModel
{
	Q_DISABLE_COPY(Node)

public:
	Node(Node *parent = 0) :
		QAbstractItemModel(parent)
	{}

	virtual QString fileName() const = 0;
	virtual QString absolutePath() const = 0;
	virtual QString absoluteFilePath() const = 0;

	virtual void refresh() = 0;
	virtual void activated(const QModelIndex &index) = 0;
	virtual void remove(const QModelIndex &index) = 0;
	virtual void copy(const QModelIndex &index, Node *destination) = 0;
	virtual void move(const QModelIndex &index, Node *destination) = 0;
	virtual void createFolder(const QString &name) = 0;
	virtual void createFile(const QString &name) = 0;

protected:
	friend class RootNode;
	virtual bool isRoot() const = 0;
	virtual void view(QAbstractItemView *itemView) = 0;
	virtual Node *node(const QString &fileName, PluginsManager *plugins) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMNODE_H_ */
