#include "../filesystemrootnode.h"
#include "../../filesystempluginsmanager.h"
#include "../../folder/filesystemrootfoldernode.h"
#include "../../interfaces/imp/filesystemrootfilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class LocalRootFolderNode;
static LocalRootFolderNode *rootNode = NULL;


class LocalRootFolderNode : public RootFolderNode
{
public:
	LocalRootFolderNode(IFileContainer::Holder &container, Node *parent = 0) :
		RootFolderNode(container, parent)
	{}
	virtual ~LocalRootFolderNode()
	{
		rootNode = NULL;
	}
};


RootNode::RootNode() :
	Node(m_items)
{
	Q_ASSERT(rootNode == NULL);

	IFileContainer::Holder container(new RootFileContainer());
	rootNode = new LocalRootFolderNode(container, this);
}

RootNode::~RootNode()
{
	Q_ASSERT_X(rootNode == NULL, "RootNode::~RootNode", "Bad links counting!");
}

IFileInfo *RootNode::info(const QModelIndex &idx) const
{
	return NULL;
}

ICopyControl *RootNode::createControl(INodeView *view) const
{
	return NULL;
}

void RootNode::contextMenu(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void RootNode::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void RootNode::rename(const QModelIndex &index, INodeView *view)
{

}

void RootNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::cancel(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::copy(const INodeView *source, INodeView *destination)
{

}

void RootNode::move(const INodeView *source, INodeView *destination)
{

}

void RootNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::refresh()
{

}

QString RootNode::title() const
{
	return QString();
}

QString RootNode::location() const
{
	return QString();
}

QString RootNode::location(const QString &fileName) const
{
	return QString();
}

QString RootNode::location(const QModelIndex &index) const
{
	return QString();
}

QAbstractItemModel *RootNode::model() const
{
	return NULL;
}

QAbstractItemDelegate *RootNode::delegate() const
{
	return NULL;
}

const INodeView::MenuActionList &RootNode::actions() const
{
	return m_menuActions;
}

::History::Entry *RootNode::menuAction(QAction *action, INodeView *view)
{
	return NULL;
}

QModelIndex RootNode::rootIndex() const
{
	return QModelIndex();
}

Node *RootNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	return NULL;
}

Node *RootNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	if (fileName == QLatin1String("/"))
		return rootNode;
	else
		return NULL;
}

FILE_SYSTEM_NS_END
