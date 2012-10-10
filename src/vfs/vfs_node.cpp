#include "vfs_node.h"
#include "../tools/optimizations/optimizations.h"


VFS_NS_BEGIN

Node::Node(const Container &conteiner, Node *parent) :
	Model(conteiner, parent),
	m_links(0)
{}

void Node::viewClosed(INodeView *nodeView)
{
	removeView(nodeView);
}

::History::Entry *Node::viewParent(INodeView *nodeView)
{
	if (parentNode())
		return viewChild(nodeView, rootIndex());
	else
		return NULL;
}

::History::Entry *Node::viewChild(INodeView *nodeView, const QModelIndex &idx)
{
	QModelIndex selected;

	if (Node *node = viewChild(idx, selected))
	{
		/* XXX: Add 2 links because of HistoryEntry */

		if (node == parentNode())
			node->viewThis(nodeView, m_parentEntryIndex, 2);
		else
		{
			node->m_parentEntryIndex = idx;
			node->viewThis(nodeView, selected, 2);
		}

		node->refresh();

		return new HistoryEntry(node);
	}

	return NULL;
}

::History::Entry *Node::viewInNewTab(INodeView *nodeView, const QModelIndex &idx)
{
	Q_ASSERT(nodeView->node() == NULL);

	/* XXX: Add 2 links because of HistoryEntry */

	viewThis(nodeView, idx, 2);
	refresh();

	return new HistoryEntry(this);
}

void Node::viewHistory(INodeView *nodeView, ::History::Entry *entry)
{
	Node *node = static_cast<HistoryEntry *>(entry)->node();
	node->viewThis(nodeView, QModelIndex());
	node->refresh();
}

Node::Sorting Node::sorting() const
{
	return Sorting(0, Qt::AscendingOrder);
}

Node::Geometry Node::geometry() const
{
	return Geometry() << 300 << 80 << 50;
}

int Node::columnsCount() const
{
	return columnCount(QModelIndex());
}

QAbstractItemView::SelectionMode Node::selectionMode() const
{
	return QAbstractItemView::ExtendedSelection;
}

void Node::nodeRemoved(Node *node)
{}

void Node::switchTo(Node *node, INodeView *view)
{
	/* XXX: Add 2 links because of HistoryEntry */

	node->viewThis(view, QModelIndex(), 2);
	node->refresh();

	view->save(new HistoryEntry(node));
}

Node *Node::viewChild(const Uri::Iterator &path, QModelIndex &selected)
{
	Node *current = this;

	for (Node *next; !path.atEnd(); ++path)
		if (next = current->viewChild(*path, selected))
			current = next;
		else
			break;

	return current;
}

void Node::addLinks(qint32 count)
{
	m_links += count;

	if (Node *parent = parentNode())
		parent->addLinks(count);
}

void Node::removeLink()
{
	removeLinks(1);

	if (!isLinked())
		if (Node *parent = parentNode())
			parent->allChildLinksRemoved(this);
		else
			delete this;
}

Node::HistoryEntry::HistoryEntry(::VFS::Node *node) :
	m_node(node)
{}

Node::HistoryEntry::~HistoryEntry()
{
	m_node->removeLink();
}

bool Node::HistoryEntry::isEqual(const Entry *entry) const
{
	return m_node == static_cast<const HistoryEntry *>(entry)->node();
}

void Node::viewThis(INodeView *nodeView, const QString &selected, qint32 links)
{
	viewThis(nodeView, childIndex(selected), links);
}

void Node::viewThis(INodeView *nodeView, const QModelIndex &selected, qint32 links)
{
	addView(nodeView, links);

	/**
	 * XXX: Assumption: navigation will occur
	 * more frequently than creating new tabs.
	 */
	INode *node = nodeView->node();
	if (LIKELY(node != NULL))
		node->viewClosed(nodeView);

	nodeView->setNode(this);

	if (selected.isValid())
		nodeView->select(selected);
	else
		nodeView->select(rootIndex());
}

bool Node::isLinked() const
{
	return m_links;
}

void Node::removeLinks(qint32 count)
{
	m_links -= count;

	if (Node *parent = parentNode())
		parent->removeLinks(count);
}

void Node::allChildLinksRemoved(Node *child)
{
	if (isLinked())
	{
		nodeRemoved(child);
		delete child;
	}
	else
		if (Node *parent = parentNode())
		{
			delete child;
			parent->allChildLinksRemoved(this);
		}
		else
		{
			nodeRemoved(child);
			delete child;
			delete this;
		}
}

void Node::addView(INodeView *view, qint32 links)
{
	m_view.insert(view);
	addLinks(links);
}

void Node::removeView(INodeView *view)
{
	if (m_view.remove(view))
		removeLink();
}

VFS_NS_END
