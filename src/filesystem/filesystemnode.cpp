#include "filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

Node::Node(const Container &conteiner, Node *parent) :
	NodeModel(conteiner, parent),
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
			node->viewThis(nodeView, selected, 2);

		node->refresh();
		removeView(nodeView);

		return new HistoryEntry(node);
	}

	return NULL;
}

::History::Entry *Node::viewInNewTab(INodeView *nodeView, const QModelIndex &idx)
{
	Q_ASSERT(nodeView->node() == NULL);
	QModelIndex selected;

	if (Node *node = viewChild(idx, selected))
	{
		/* XXX: Add 2 links because of HistoryEntry */

		if (node == parentNode())
			node->viewThis(nodeView, m_parentEntryIndex, 2);
		else
			node->viewThis(nodeView, selected, 2);

		node->refresh();

		return new HistoryEntry(node);
	}

	return new HistoryEntry(this);
}

void Node::viewHistory(INodeView *nodeView, ::History::Entry *entry)
{
	Node *node = static_cast<HistoryEntry *>(entry)->node();

	if (node == parentNode())
		node->viewThis(nodeView, m_parentEntryIndex);
	else
		node->viewThis(nodeView, QModelIndex());

	node->refresh();
	removeView(nodeView);
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

::History::Entry *Node::switchTo(Node *node, INodeView *view)
{
	/* XXX: Add 2 links because of HistoryEntry */

	node->viewThis(view, QModelIndex(), 2);
	node->refresh();
	removeView(view);

	return new HistoryEntry(node);
}

Node *Node::viewChild(const Path::Iterator &path, QModelIndex &selected)
{
	Node *current = this;

	for (Node *next; !path.atEnd(); ++path)
		if (next = current->viewChild(*path, selected))
			current = next;
		else
			break;

	return current;
}

void Node::addLink()
{
	++m_links;

	if (Node *parent = parentNode())
		parent->addLink();
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
		parentNode()->allChildLinksRemoved(this);
}

Node::HistoryEntry::HistoryEntry(::FileSystem::Node *node) :
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

void Node::viewThis(INodeView *nodeView, const QModelIndex &selected)
{
	addView(nodeView);
	nodeView->setNode(this);

	if (selected.isValid())
		nodeView->select(selected);
	else
		nodeView->select(rootIndex());
}

void Node::viewThis(INodeView *nodeView, const QModelIndex &selected, qint32 links)
{
	addView(nodeView, links);
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

void Node::addView(INodeView *view)
{
	m_view.insert(view);
	addLink();
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

FILE_SYSTEM_NS_END
