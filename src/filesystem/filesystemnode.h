#ifndef FILESYSTEMNODE_H_
#define FILESYSTEMNODE_H_

#include <QtCore/QSet>
#include "tools/filesystempath.h"
#include "model/filesystemnodemodel.h"
#include "model/filesystemnodemodelcontainer.h"
#include "interfaces/filesysteminode.h"
#include "../history/historyentry.h"


FILE_SYSTEM_NS_BEGIN

/*
 *  Implements default file system tree navigation.
 *
 */

class Node : public NodeModel, public INode
{
	Q_DISABLE_COPY(Node)

public:
	Node(const NodeModelContainer &conteiner, Node *parent = 0);

	/* IFileNavigation */
	virtual void viewClosed(INodeView *nodeView);
	virtual ::History::Entry *viewParent(INodeView *nodeView);
	virtual ::History::Entry *viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins);
	virtual ::History::Entry *viewAbsolute(INodeView *nodeView, const QString &filePath, PluginsManager *plugins);
	virtual void viewHistory(INodeView *nodeView, ::History::Entry *entry);

	/* INode */
	virtual int columnsCount() const;
	virtual QAbstractItemView::SelectionMode selectionMode() const;

	void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }

protected:
	virtual QModelIndex rootIndex() const = 0;
	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected) = 0;
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected) = 0;
	virtual void nodeRemoved(Node *node);

protected:
	Node *root() const;
	Node *parentNode() const { return static_cast<Node*>(QObject::parent()); }
	bool isVisible() const { return !m_view.isEmpty(); }
	::History::Entry *switchTo(Node *node, INodeView *view);

private:
	friend class TasksNode;
	friend class HistoryEntry;
	void addLink();
	void addLinks(qint32 count);
	void removeLink();

private:
	class HistoryEntry : public ::History::Entry
	{
	public:
		HistoryEntry(::FileSystem::Node *node);
		virtual ~HistoryEntry();

		virtual bool isEqual(const Entry *entry) const;
		::FileSystem::Node *node() const { return m_node; }

	private:
		::FileSystem::Node *m_node;
	};

	void viewThis(INodeView *nodeView, const QModelIndex &selected);
	void viewThis(INodeView *nodeView, const QModelIndex &selected, qint32 links);
	Node *viewChild(INodeView *nodeView, const Path::Iterator &path, QModelIndex &selected, PluginsManager *plugins);

private:
	bool isLinked() const;
	void removeLinks(qint32 count);
	void allChildLinksRemoved(Node *child);

	void addView(INodeView *view);
	void addView(INodeView *view, qint32 links);
	void removeView(INodeView *view);

private:
	typedef QSet<INodeView*> ViewSet;

private:
	ViewSet m_view;
	qint32 m_links;
	QModelIndex m_parentEntryIndex;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMNODE_H_ */
