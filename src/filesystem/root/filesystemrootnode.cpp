#include "filesystemrootnode.h"
#include "../filesystempluginsmanager.h"
#include "../folder/filesystemrootfoldernode.h"
#include "../folder/items/filesystemfolderitem.h"
#include "../interfaces/imp/filesystemfilecontainer.h"


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode() :
	Node(m_items)
{}

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
	ItemsContainer::size_type index = m_items.indexOf(fileName);

	if (index == ItemsContainer::InvalidIndex)
	{
		m_items.add(createItem(fileName, plugins));
		return m_items.last()->node();
	}
	else
	{
		FolderItem *item = static_cast<FolderItem *>(m_items[index]);

		if (item->node())
			return item->node();
		else
			if (Node *node = createNode(item->info(), plugins))
			{
				item->setNode(node);
				return node;
			}
	}

	return NULL;
}

void RootNode::nodeRemoved(Node *node)
{
	delete m_items.take(m_items.indexOf(node));
}

Node *RootNode::createNode(const Info &info, PluginsManager *plugins)
{
	if (Node *res = plugins->node(&m_container, &info, this))
		return res;
	else
		if (info.isDir())
		{
			IFileContainer::Holder container(new FileContainer(info.fileName()));
			return new RootFolderNode(container, this);
		}

	return NULL;
}

FolderBaseItem *RootNode::createItem(const QString &fileName, PluginsManager *plugins)
{
	Info info(fileName, Info::Refresh());
	return new FolderItem(info, createNode(info, plugins));
}

bool RootNode::Container::isPhysical() const
{
	return false;
}

QString RootNode::Container::location() const
{
	return QString();
}

QString RootNode::Container::location(const QString &fileName) const
{
	return QString();
}

IFileInfo::size_type RootNode::Container::freeSpace() const
{
	return 0;
}

bool RootNode::Container::contains(const QString &fileName) const
{
	return false;
}

bool RootNode::Container::remove(const QString &fileName, QString &error) const
{
	return false;
}

bool RootNode::Container::rename(const QString &oldName, const QString &newName, QString &error) const
{
	return false;
}

IFileContainer *RootNode::Container::open(QString &error) const
{
	return NULL;
}

IFileAccessor *RootNode::Container::open(const QString &fileName, int mode, QString &error) const
{
	return NULL;
}

IFileContainer *RootNode::Container::open(const QString &fileName, bool create, QString &error) const
{
	return NULL;
}

FILE_SYSTEM_NS_END
