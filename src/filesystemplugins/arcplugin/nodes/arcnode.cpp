#include "arcnode.h"
#include "items/arcnoderootitem.h"
#include "items/arcnodedirentryitem.h"
#include "../tasks/arcreadarchivetask.h"
#include "../tasks/arcperformcopytask.h"


ARC_PLUGIN_NS_BEGIN

ArcNode::ArcNode(const Info &info, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
	m_info(info)
{
	m_proxy.setSourceModel(this);
	m_items.push_back(new ArcNodeRootItem());

	addTask(new ReadArchiveTask(m_info.absoluteFilePath(), this), TasksItemList() << m_items.at(RootItemIndex));
}

bool ArcNode::event(QEvent *e)
{
	switch (static_cast<TaskEvent::Type>(e->type()))
	{
		case TaskEvent::ScanComplete:
		{
			e->accept();
			scanCompleteEvent(static_cast<TaskEvent*>(e));

			return true;
		}

		case TaskEvent::CopyComplete:
		{
			e->accept();
			copyCompleteEvent(static_cast<TaskEvent*>(e));

			return true;
		}

		default:
			break;
	}

	return TasksNode::event(e);
}

int ArcNode::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant ArcNode::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		switch (section)
		{
			case 0:
			{
				return tr("Name");
				break;
			}
			case 1:
			{
				return tr("Size");
				break;
			}
			case 2:
			{
				return tr("Modified");
				break;
			}
		}

	return QVariant();
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
	QModelIndex index = m_proxy.mapToSource(source->currentIndex());

	if (index.isValid())
	{
		PScopedPointer<ICopyControl> control(destination->node()->createControl(destination));

		if (control)
		{
			TasksItemList list;

			if (static_cast<ArcNodeItem::Base*>(index.internalPointer())->isList())
				static_cast<ArcNodeListItem*>(index.internalPointer())->lock(tr("Extracting..."));
			else
				static_cast<ArcNodeItem*>(index.internalPointer())->lock(tr("Extracting..."));

			updateFirstColumn(static_cast<ArcNodeItem::Base*>(index.internalPointer()));
			addTask(new PerformCopyTask(m_info.absoluteFilePath(), static_cast<ArcNodeItem::Base*>(index.internalPointer()), control, false, this), list);
		}
	}
}

void ArcNode::move(const INodeView *source, INodeView *destination)
{

}

QAbstractItemModel *ArcNode::model() const
{
	return const_cast<ArcNodeProxyModel*>(&m_proxy);
}

QAbstractItemDelegate *ArcNode::delegate() const
{
	return const_cast<ArcDelegate*>(&m_delegate);
}

const INodeView::MenuActionList &ArcNode::actions() const
{
	return m_actions;
}

QAbstractItemView::SelectionMode ArcNode::selectionMode() const
{
	return QAbstractItemView::SingleSelection;
}

QModelIndex ArcNode::rootIndex() const
{
	return m_proxy.mapFromSource(createIndex(0, 0, m_items.at(RootItemIndex)));
}

Node *ArcNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	ArcNodeItem::Base *item;

	if ((item = static_cast<ArcNodeItem::Base*>(m_proxy.mapToSource(idx).internalPointer()))->isList())
	{

	}
	else
		if (static_cast<ArcNodeItem*>(item)->isRoot())
		{
			cancelTask(m_items.at(RootItemIndex));
			return parentNode();
		}

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

void ArcNode::scanCompleteEvent(TaskEvent *e)
{
	ReadArchiveTask::Event *event = static_cast<ReadArchiveTask::Event*>(e);

	if (event->canceled)
		qDeleteAll(event->contents.items);
	else
		if (!event->contents.items.isEmpty())
		{
			beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + event->contents.items.size() - 1);
			m_items.append(event->contents.items);
			endInsertRows();
		}

	removeAllTaskLinks(event->task);
}

void ArcNode::copyCompleteEvent(TaskEvent *e)
{
	PerformCopyTask::Event *event = static_cast<PerformCopyTask::Event*>(e);
	ArcNodeItem::Base *item = const_cast<ArcNodeItem::Base *>(event->item);

	if (item->isList())
		static_cast<ArcNodeListItem*>(item)->unlock();
	else
		static_cast<ArcNodeItem*>(item)->unlock();

	updateFirstColumn(item);
	removeAllTaskLinks(event->task);
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

void ArcNode::updateFirstColumn(ArcNodeItem::Base *entry)
{
	QModelIndex index;

	if (ArcNodeDirEntryItem *parent = static_cast<ArcNodeDirEntryItem *>(entry->parent()))
		index = createIndex(parent->indexOf(entry), 0, entry);
	else
		index = createIndex(m_items.indexOf(entry), 0, entry);

	emit dataChanged(index, index);
}

ARC_PLUGIN_NS_END
