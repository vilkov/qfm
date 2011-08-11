#include "filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

Node::Node(Node *parent) :
	QAbstractItemModel(parent)
{}

INode *Node::root() const
{
	if (INode *res = static_cast<Node*>(Node::parent()))
		return res->root();
	else
		return (INode*)this;
}

int Node::columnsCount() const
{
	return columnCount(QModelIndex());
}

void Node::viewClosed(INodeView *nodeView)
{
	removeView(nodeView);
}

void Node::viewParent(INodeView *nodeView)
{
	if (exists())
		switchTo(static_cast<Node*>(Node::parent()), nodeView, m_parentEntryIndex);
	else
		removeThis();
}

void Node::viewThis(INodeView *nodeView, const QModelIndex &selected)
{
	addView(nodeView);
	nodeView->setNode(this, proxyModel(), itemDelegate());

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
		removeView(nodeView);

		if (node == static_cast<Node*>(Node::parent()))
			node->viewThis(nodeView, m_parentEntryIndex);
		else
			node->viewThis(nodeView, selected);

		node->refresh();
	}
}

void Node::viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins)
{
	QModelIndex selected;

	if (Node *node = viewChild(*path, plugins, selected))
	{
		removeView(nodeView);

		if ((++path).atEnd())
		{
			if (node == static_cast<Node*>(Node::parent()))
				node->viewThis(nodeView, m_parentEntryIndex);
			else
				node->viewThis(nodeView, selected);

			node->refresh();
		}
		else
			node->viewChild(nodeView, path, plugins);
	}
}

void Node::viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins)
{
	Path path(absoluteFilePath);

	if (path.isAbsolute())
	{
		Node *node = this;
		removeView(nodeView);

		while (node->parent())
			node = static_cast<Node*>(node->parent());

		node->viewChild(nodeView, path.begin(), plugins);
	}
	else
		viewChild(nodeView, path.begin(), plugins);
}

void Node::switchTo(Node *node, const QModelIndex &selected)
{
	for (ViewSet::iterator it = m_view.begin(), end = m_view.end(); it != end; it = m_view.erase(it))
		node->viewThis(*it, selected);
}

void Node::removeThis()
{
	Node *parent = static_cast<Node*>(Node::parent());

	while (!parent->exists())
		parent = static_cast<Node*>(parent->Node::parent());

	switchTo(parent, QModelIndex());
}

void Node::switchTo(Node *node, INodeView *nodeView, const QModelIndex &selected)
{
	removeView(nodeView);
	node->viewThis(nodeView, selected);
	node->refresh();
}

FILE_SYSTEM_NS_END
