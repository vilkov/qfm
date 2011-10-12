#include "filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

Node::Node(Node *parent) :
	QSharedData(),
	FileSystemModel(parent),
	m_links(0)
{}

INode *Node::root() const
{
	if (INode *res = Node::parent())
		return res->root();
	else
		return (INode*)this;
}

int Node::columnsCount() const
{
	return columnCount(QModelIndex());
}

void Node::viewCloseAll()
{
	if (!m_view.isEmpty())
	{
		QModelIndex index;
		qint32 count = m_view.size();
		Node *parent = Node::parent();

		while (!parent->exists())
			parent = static_cast<Node*>(parent->Node::parent());

		for (ViewSet::iterator it = m_view.begin(), end = m_view.end(); it != end; it = m_view.erase(it))
			parent->viewThis(*it, index);

		removeLinks(count);
	}

	if (!isLinked())
		static_cast<Node*>(parent())->allChildLinksRemoved(this);
}

void Node::viewClosed(INodeView *nodeView)
{
	removeView(nodeView);
}

void Node::viewParent(INodeView *nodeView)
{
	if (exists())
	{
		Node *parent = Node::parent();

		parent->viewThis(nodeView, m_parentEntryIndex);
		parent->refresh();

		removeView(nodeView);
	}
	else
		viewCloseAll();
}

void Node::viewThis(INodeView *nodeView, const QModelIndex &selected)
{
	addView(nodeView);
	nodeView->setNode(this, proxyModel(), itemDelegate(), menuActions());

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

		if (node == Node::parent())
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
			if (node == Node::parent())
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

QStringList Node::toFileNameList(const InfoListItem *files) const
{
	QStringList res;
	res.reserve(files->size());

	for (InfoListItem::size_type i = 0, size = files->size(); i < size; ++i)
		res.push_back(files->at(i)->fileName());

	return res;
}

bool Node::isLinked() const
{
	return m_links;
}

void Node::addLink()
{
	++m_links;

	if (parent())
		static_cast<Node*>(parent())->addLink();
}

void Node::removeLink()
{
	removeLinks(1);

	if (!isLinked())
		static_cast<Node*>(parent())->allChildLinksRemoved(this);
}

void Node::removeLinks(qint32 count)
{
	m_links -= count;

	if (parent())
		static_cast<Node*>(parent())->removeLinks(count);
}

void Node::allChildLinksRemoved(Node *child)
{
	if (isLinked())
		removeChild(child);
	else
		if (parent())
			static_cast<Node*>(parent())->allChildLinksRemoved(this);
		else
			removeChild(child);
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
