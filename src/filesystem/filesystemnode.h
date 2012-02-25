#ifndef FILESYSTEMNODE_H_
#define FILESYSTEMNODE_H_

#include <QtCore/QSet>
#include <QtCore/QAbstractItemModel>
#include <QtGui/QAbstractItemDelegate>
#include "filesystempath.h"
#include "model/filesystemmodel.h"
#include "model/filesystemmodelcontainer.h"
#include "interfaces/filesysteminode.h"
#include "../history/historyentry.h"


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

	/* IFileOperations */
	virtual IFileInfo *info(const QModelIndex &idx) const;

	/* IFileNavigation */
	virtual void viewClosed(INodeView *nodeView);
	virtual ::History::Entry *viewParent(INodeView *nodeView);
	virtual ::History::Entry *viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins);
	virtual ::History::Entry *viewAbsolute(INodeView *nodeView, const QString &filePath, PluginsManager *plugins);

	/* INode */
	virtual int columnsCount() const;
	virtual QAbstractItemView::SelectionMode selectionMode() const;

	void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }

protected:
	class HistoryEntry : public ::History::Entry
	{
	public:
		HistoryEntry(::FileSystem::Node *node);
		virtual ~HistoryEntry();

		virtual ::FileSystem::INode *node();

	private:
		::FileSystem::Node *m_node;
	};

	virtual QModelIndex rootIndex() const = 0;
	virtual HistoryEntry *historyEntry() const;
	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected) = 0;
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected) = 0;
	virtual void nodeRemoved(Node *node);

protected:
	Node *root() const;
	Node *parentNode() const { return static_cast<Node*>(QObject::parent()); }
	bool isVisible() const { return !m_view.isEmpty(); }
	void switchTo(Node *node, INodeView *view);

private:
	friend class TasksNode;
	friend class HistoryEntry;
	void addLink();
	void removeLink();

private:
	void viewThis(INodeView *nodeView, const QModelIndex &selected);
	void viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins);

private:
	bool isLinked() const;
	void removeLinks(qint32 count);
	void allChildLinksRemoved(Node *child);

	void addView(INodeView *view);
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
