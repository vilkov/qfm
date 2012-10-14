/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef VFS_NODE_H_
#define VFS_NODE_H_

#include <QtCore/QSet>
#include "tools/vfs_uri.h"
#include "model/vfs_model.h"
#include "interfaces/vfs_inode.h"
#include "../history/historyentry.h"


VFS_NS_BEGIN

/**
 * Implements default file system tree navigation.
 */

class Node : public Model, public INode
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
	virtual Sorting sorting() const;
	virtual Geometry geometry() const;
	virtual int columnsCount() const;
	virtual QAbstractItemView::SelectionMode selectionMode() const;

protected:
	virtual QModelIndex rootIndex() const = 0;
	virtual QModelIndex childIndex(const QString &fileName) = 0;
	virtual Node *viewChild(const QModelIndex &idx, QModelIndex &selected) = 0;
	virtual Node *viewChild(const QString &fileName, QModelIndex &selected) = 0;
	virtual void nodeRemoved(Node *node);

protected:
	Node *parentNode() const { return static_cast<Node *>(QObject::parent()); }
	bool isVisible() const { return !m_view.isEmpty(); }
	void switchTo(Node *node, INodeView *view);
	Node *viewChild(const Uri::Iterator &path, QModelIndex &selected);

private:
	friend class TasksNode;
	friend class HistoryEntry;
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

	void viewThis(INodeView *nodeView, const QString &selected, qint32 links = 1);
	void viewThis(INodeView *nodeView, const QModelIndex &selected, qint32 links = 1);

private:
	bool isLinked() const;
	void removeLinks(qint32 count);
	void allChildLinksRemoved(Node *child);

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
