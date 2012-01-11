#include "filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

Node::Node(const ModelContainer &conteiner, Node *parent) :
	FileSystemModel(conteiner, parent),
	m_links(0)
{}

void Node::viewCloseAll()
{
	if (!m_view.isEmpty())
	{
		QModelIndex index;
		qint32 count = m_view.size();
		Node *parent = parentNode();

		while (!parent->exists())
			parent = parent->parentNode();

		for (ViewSet::iterator it = m_view.begin(), end = m_view.end(); it != end; it = m_view.erase(it))
			parent->viewThis(*it, index);

		removeLinks(count);
	}

	if (!isLinked())
		parentNode()->allChildLinksRemoved(this);
}

void Node::viewClosed(INodeView *nodeView)
{
	removeView(nodeView);
}

void Node::viewParent(INodeView *nodeView)
{
	if (static_cast<INode*>(parentNode()) != root())
		viewChild(nodeView, rootIndex(), 0);
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

void Node::viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins)
{
	QModelIndex selected;

	if (Node *node = viewChild(idx, plugins, selected))
	{
		if (node == parentNode())
			node->viewThis(nodeView, m_parentEntryIndex);
		else
			node->viewThis(nodeView, selected);

		node->refresh();
		removeView(nodeView);
	}
}

void Node::viewAbsolute(INodeView *nodeView, const QString &filePath, PluginsManager *plugins)
{
	Path path(filePath);
	Path::Iterator it = path.begin();

	if (!it.atEnd())
		viewChild(nodeView, it, plugins);
}

INode *Node::root() const
{
	if (INode *res = parentNode())
		return res->root();
	else
		return (INode*)this;
}

int Node::columnsCount() const
{
	return columnCount(QModelIndex());
}

QAbstractItemView::SelectionMode Node::selectionMode() const
{
	return QAbstractItemView::ExtendedSelection;
}

void Node::switchTo(Node *node, INodeView *view)
{
	node->viewThis(view, QModelIndex());
	node->refresh();
	removeView(view);
}

void Node::nodeRemoved(Node *node)
{}

void Node::addLink()
{
	++m_links;

	if (Node *parent = parentNode())
		parent->addLink();
}

void Node::removeLink()
{
	removeLinks(1);

	if (!isLinked())
		parentNode()->allChildLinksRemoved(this);
}

void Node::viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins)
{
	QModelIndex selected;

	if (Node *node = viewChild(*path, plugins, selected))
	{
		if ((++path).atEnd())
		{
			if (node == parentNode())
				node->viewThis(nodeView, m_parentEntryIndex);
			else
				node->viewThis(nodeView, selected);

			node->refresh();

			if (node == this)
				return;
		}
		else
			node->viewChild(nodeView, path, plugins);

		removeView(nodeView);
	}
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
		}
}

void Node::addView(INodeView *view)
{
	m_view.insert(view);
	addLink();
}

void Node::removeView(INodeView *view)
{
	if (m_view.remove(view))
		removeLink();
}

FILE_SYSTEM_NS_END
