#include "arcnode.h"
#include "items/arcnoderootitem.h"
#include "../tasks/arcreadarchivetask.h"
#include <QtCore/QDebug>


ARC_PLUGIN_NS_BEGIN

ArcNode::ArcNode(const Info &info, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
	m_info(info)
{
	m_proxy.setSourceModel(this);
	m_items.push_back(new ArcNodeRootItem());
	handleTask(new ReadArchiveTask(m_info.absoluteFilePath(), this));
}

bool ArcNode::event(QEvent *e)
{
	switch (static_cast<ReadArchiveTask::Event::Type>(e->type()))
	{
		case ReadArchiveTask::Event::ScanComplete:
		{
			ReadArchiveTask::Event *event = static_cast<ReadArchiveTask::Event*>(e);

			event->accept();
			scanCompleteEvent(event->contents);

			return true;
		}
		default:
			break;
	}

	return TasksNode::event(e);
}

FileTypeId ArcNode::id() const
{
	return m_info.id();
}

QIcon ArcNode::icon() const
{
	return m_info.icon();
}

QString ArcNode::name() const
{
	return m_info.name();
}

QString ArcNode::description() const
{
	return m_info.description();
}

bool ArcNode::isDir() const
{
	return m_info.isDir();
}

bool ArcNode::isFile() const
{
	return m_info.isFile();
}

bool ArcNode::isLink() const
{
	return m_info.isLink();
}

bool ArcNode::exists() const
{
	return m_info.exists();
}

qint64 ArcNode::fileSize() const
{
	return m_info.fileSize();
}

QString ArcNode::fileName() const
{
	return m_info.fileName();
}

QString ArcNode::absolutePath() const
{
	return m_info.absolutePath();
}

QString ArcNode::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString ArcNode::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime ArcNode::lastModified() const
{
	return m_info.lastModified();
}

int ArcNode::permissions() const
{
	return m_info.permissions();
}

void ArcNode::refresh()
{

}

IFileInfo *ArcNode::info(const QModelIndex &idx) const
{
	return 0;
}

ICopyControl *ArcNode::createControl(INodeView *view) const
{
	return 0;
}

void ArcNode::menuAction(QAction *action, INodeView *view)
{

}

void ArcNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void ArcNode::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void ArcNode::rename(const QModelIndex &index, INodeView *view)
{

}

void ArcNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void ArcNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void ArcNode::cancel(const QModelIndexList &list, INodeView *view)
{

}

void ArcNode::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void ArcNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void ArcNode::copy(const INodeView *source, INodeView *destination)
{

}

void ArcNode::move(const INodeView *source, INodeView *destination)
{

}

QModelIndex ArcNode::rootIndex() const
{
	return m_proxy.mapFromSource(createIndex(0, 0, m_items.at(0)));
}

QAbstractItemModel *ArcNode::proxyModel() const
{
	return const_cast<ArcNodeProxyModel*>(&m_proxy);
}

QAbstractItemDelegate *ArcNode::itemDelegate() const
{
	return const_cast<ArcDelegate*>(&m_delegate);
}

const INodeView::MenuActionList &ArcNode::menuActions() const
{
	return m_actions;
}

Node *ArcNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	ArcNodeItem::Base *item;

	if ((item = static_cast<ArcNodeItem::Base*>(m_proxy.mapToSource(idx).internalPointer()))->isList())
	{

	}
	else
		if (static_cast<ArcNodeItem*>(item)->isRoot())
			return parentNode();

	return 0;
}

Node *ArcNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

void ArcNode::updateProgressEvent(TaskNodeItem::Base *item, quint64 progress, quint64 timeElapsed)
{

}

void ArcNode::completedProgressEvent(TaskNodeItem::Base *item, quint64 timeElapsed)
{

}

void ArcNode::scanCompleteEvent(const Archive::Contents &contents)
{
	if (!contents.items.isEmpty())
	{
		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + contents.items.size() - 1);
		m_items.append(contents.items);
		endInsertRows();
	}

	taskHandled();
}

ArcNode::ItemsContainer::ItemsContainer()
{}

ArcNode::ItemsContainer::~ItemsContainer()
{
	qDeleteAll(m_container);
}

ArcNode::ItemsContainer::size_type ArcNode::ItemsContainer::size() const
{
	return m_container.size();
}

ArcNode::ItemsContainer::Item *ArcNode::ItemsContainer::at(size_type index) const
{
	return m_container.at(index);
}

ArcNode::ItemsContainer::size_type ArcNode::ItemsContainer::indexOf(Item *item) const
{
	return m_container.indexOf(item);
}

ARC_PLUGIN_NS_END
