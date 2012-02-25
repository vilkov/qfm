#include "filesystemnode.h"
#include "info/filesysteminvalidinfo.h"


FILE_SYSTEM_NS_BEGIN
static InvalidInfo invalidInfo;


Node::Node(const ModelContainer &conteiner, Node *parent) :
	FileSystemModel(conteiner, parent),
	m_links(0)
{}

IFileInfo *Node::info(const QModelIndex &idx) const
{
	return &invalidInfo;
}

void Node::viewClosed(INodeView *nodeView)
{
	removeView(nodeView);
}

::History::Entry *Node::viewParent(INodeView *nodeView)
{
	if (static_cast<INode*>(parentNode()) != root())
		viewChild(nodeView, rootIndex(), 0);

	return NULL;
}

::History::Entry *Node::viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins)
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

	return NULL;
}

::History::Entry *Node::viewAbsolute(INodeView *nodeView, const QString &filePath, PluginsManager *plugins)
{
	Path path(filePath);
	Path::Iterator it = path.begin();

	if (!it.atEnd())
		root()->viewChild(nodeView, it, plugins);

	return NULL;
}

int Node::columnsCount() const
{
	return columnCount(QModelIndex());
}

QAbstractItemView::SelectionMode Node::selectionMode() const
{
	return QAbstractItemView::ExtendedSelection;
}

Node::HistoryEntry::HistoryEntry(::FileSystem::Node *node) :
	m_node(node)
{
	m_node->addLink();
}

Node::HistoryEntry::~HistoryEntry()
{
	m_node->removeLink();
}

::FileSystem::INode *Node::HistoryEntry::node()
{
	return m_node;
}

Node::HistoryEntry *Node::historyEntry() const
{
	return NULL;
}

void Node::nodeRemoved(Node *node)
{}

Node *Node::root() const
{
	if (Node *res = parentNode())
		return res->root();
	else
		return const_cast<Node *>(this);
}

void Node::switchTo(Node *node, INodeView *view)
{
	node->viewThis(view, QModelIndex());
	node->refresh();
	removeView(view);
}

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

void Node::viewThis(INodeView *nodeView, const QModelIndex &selected)
{
	addView(nodeView);
	nodeView->setNode(this);

	if (selected.isValid())
		nodeView->select(selected);
	else
		nodeView->select(rootIndex());
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
