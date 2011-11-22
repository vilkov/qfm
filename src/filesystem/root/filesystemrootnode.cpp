#include "filesystemrootnode.h"
#include "../filesystempluginsmanager.h"
#include "../folder/filesystemfoldernode.h"
#include "../folder/items/filesystementryitem.h"


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode() :
	Node(m_items)
{}

bool RootNode::isDir() const
{
	return true;
}

bool RootNode::isFile() const
{
	return false;
}

bool RootNode::exists() const
{
	return true;
}

QString RootNode::fileName() const
{
	return QString();
}

QString RootNode::absolutePath() const
{
	return QString();
}

QString RootNode::absoluteFilePath() const
{
	return QString();
}

QString RootNode::absoluteFilePath(const QString &fileName) const
{
	return QString();
}

QDateTime RootNode::lastModified() const
{
	return QDateTime();
}

void RootNode::refresh()
{

}

IFileInfo *RootNode::info(const QModelIndex &idx) const
{
	return 0;
}

ICopyControl *RootNode::createControl(INodeView *view) const
{
	return 0;
}

void RootNode::menuAction(QAction *action, INodeView *view)
{

}

void RootNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void RootNode::createDirectory(const QModelIndex &index, INodeView *view)
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

QModelIndex RootNode::rootIndex() const
{
	return QModelIndex();
}

QAbstractItemModel *RootNode::proxyModel() const
{
	return 0;
}

QAbstractItemDelegate *RootNode::itemDelegate() const
{
	return 0;
}

const INodeView::MenuActionList &RootNode::menuActions() const
{
	return m_menuActions;
}

Node *RootNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
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
		FileSystemEntryItem *item = static_cast<FileSystemEntryItem*>(m_items[index]);

		if (item->node())
			return item->node();
		else
			if (Node *node = createNode(item->info(), plugins))
			{
				item->setNode(node);
				return node;
			}
	}

	return 0;
}

void RootNode::nodeRemoved(Node *node)
{
	delete m_items.take(m_items.indexOf(node));
}

Node *RootNode::createNode(const Info &info, PluginsManager *plugins)
{
	if (Node *res = plugins->node(&info, this))
		return res;
	else
		if (info.isDir())
			return new FolderNode(info, this);
		else
			return 0;
}

FileSystemBaseItem *RootNode::createItem(const QString &fileName, PluginsManager *plugins)
{
	Info info(fileName);
	return new FileSystemEntryItem(info, createNode(info, plugins));
}

FILE_SYSTEM_NS_END
