#include "filesystemrootnode.h"
#include "../filesystempluginsmanager.h"
#include "../folder/filesystemfoldernode.h"
#include "../folder/items/filesystementryitem.h"


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode() :
	Node(m_items)
{}

FileTypeId RootNode::id() const
{
	return FileTypeId();
}

QIcon RootNode::icon() const
{
	return QIcon();
}

QString RootNode::name() const
{
	return QString();
}

QString RootNode::description() const
{
	return QString();
}

bool RootNode::isDir() const
{
	return true;
}

bool RootNode::isFile() const
{
	return false;
}

bool RootNode::isLink() const
{
	return false;
}

bool RootNode::exists() const
{
	return true;
}

RootNode::size_type RootNode::fileSize() const
{
	return 0;
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

int RootNode::permissions() const
{
	return 0;
}

void RootNode::refresh()
{

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

void RootNode::menuAction(QAction *action, INodeView *view)
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
		FileSystemEntryItem *item = static_cast<FileSystemEntryItem *>(m_items[index]);

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
	if (Node *res = plugins->node(&info, this))
		return res;
	else
		if (info.isDir())
			return new FolderNode(info, this);
		else
			return NULL;
}

FileSystemBaseItem *RootNode::createItem(const QString &fileName, PluginsManager *plugins)
{
	Info info(fileName, true);
	return new FileSystemEntryItem(info, createNode(info, plugins));
}

FILE_SYSTEM_NS_END
