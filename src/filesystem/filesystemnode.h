#ifndef FILESYSTEMNODE_H_
#define FILESYSTEMNODE_H_

#include <QtCore/QSet>
#include <QtCore/QAbstractItemModel>
#include <QtGui/QAbstractItemDelegate>
#include "filesystempath.h"
#include "model/filesystemmodel.h"
#include "model/filesystemmodelcontainer.h"
#include "interfaces/filesysteminode.h"


FILE_SYSTEM_NS_BEGIN

/*
 *  Implements default file system tree navigation.
 *
 */

class Node : public FileSystemModel, public INode
{
	Q_DISABLE_COPY(Node)

public:
	Node(const ModelContainer &conteiner, Node *parent = 0);

	/* INode */
	virtual INode *root() const;
	virtual int columnsCount() const;

	/* IFileNavigation */
	virtual void viewCloseAll();
	virtual void viewClosed(INodeView *nodeView);
	virtual void viewParent(INodeView *nodeView);
	virtual void viewThis(INodeView *nodeView, const QModelIndex &selected);
	virtual void viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins);
	virtual void viewAbsolute(INodeView *nodeView, const QString &filePath, PluginsManager *plugins);

	void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }

protected:
	virtual QModelIndex rootIndex() const = 0;
	virtual QAbstractItemModel *proxyModel() const = 0;
	virtual QAbstractItemDelegate *itemDelegate() const = 0;
	virtual const INodeView::MenuActionList &menuActions() const = 0;

	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected) = 0;
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected) = 0;
	virtual void nodeRemoved(Node *node);

protected:
	Node *parentNode() const { return static_cast<Node*>(QObject::parent()); }
	bool isVisible() const { return !m_view.isEmpty(); }
	void switchTo(Node *node, INodeView *view);

protected:
	void addLink();
	void removeLink();

private:
	void viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins);

private:
	bool isLinked() const;
	void removeLinks(qint32 count);
	void allChildLinksRemoved(Node *child);

	void addView(INodeView *view);
	void removeView(INodeView *view);

private:
	class Nodes : public QList<Node*>
	{
	public:
		enum { InvalidIndex = (size_type)-1 };
	};
	typedef QSet<INodeView*> ViewSet;

private:
	ViewSet m_view;
	qint32 m_links;
	QModelIndex m_parentEntryIndex;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMNODE_H_ */
