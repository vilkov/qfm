#ifndef VFS_NODE_H_
#define VFS_NODE_H_

#include <QtCore/QSet>
#include "tools/vfs_path.h"
#include "model/vfs_nodemodel.h"
#include "interfaces/vfs_inode.h"
#include "../history/historyentry.h"


VFS_NS_BEGIN

/*
 *  Implements default file system tree navigation.
 *
 */

class Node : public NodeModel, public INode
{
	Q_DISABLE_COPY(Node)

public:
	Node(const Container &conteiner, Node *parent = 0);

	/* IFileNavigation */
	virtual void viewClosed(INodeView *nodeView);
	virtual ::History::Entry *viewParent(INodeView *nodeView);
	virtual ::History::Entry *viewChild(INodeView *nodeView, const QModelIndex &idx);
	virtual ::History::Entry *viewInNewTab(INodeView *nodeView, const QModelIndex &idx);
	virtual void viewHistory(INodeView *nodeView, ::History::Entry *entry);

	/* INode */
	virtual int columnsCount() const;
	virtual QAbstractItemView::SelectionMode selectionMode() const;

	void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }

protected:
	virtual QModelIndex rootIndex() const = 0;
	virtual Node *viewChild(const QModelIndex &idx, QModelIndex &selected) = 0;
	virtual Node *viewChild(const QString &fileName, QModelIndex &selected) = 0;
	virtual void nodeRemoved(Node *node);

protected:
	Node *parentNode() const { return static_cast<Node*>(QObject::parent()); }
	bool isVisible() const { return !m_view.isEmpty(); }
	::History::Entry *switchTo(Node *node, INodeView *view);
	Node *viewChild(const Path::Iterator &path, QModelIndex &selected);

private:
	friend class TasksNode;
	friend class HistoryEntry;
	virtual void addLink();
	virtual void addLinks(qint32 count);
	virtual void removeLink();

private:
	friend class RootNode;

	class HistoryEntry : public ::History::Entry
	{
	public:
		HistoryEntry(::VFS::Node *node);
		virtual ~HistoryEntry();

		virtual bool isEqual(const Entry *entry) const;
		::VFS::Node *node() const { return m_node; }

	private:
		::VFS::Node *m_node;
	};

	void viewThis(INodeView *nodeView, const QModelIndex &selected);
	void viewThis(INodeView *nodeView, const QModelIndex &selected, qint32 links);

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

VFS_NS_END

#endif /* VFS_NODE_H_ */
