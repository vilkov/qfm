#include "arcnode.h"
#include "items/arcnoderootitem.h"
#include "../tasks/arcreadarchivetask.h"
#include "../tasks/arcperformcopytask.h"


ARC_PLUGIN_NS_BEGIN

ArcNode::ArcNode(const QString &filePath, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
    m_delegate(&m_proxy),
    m_filePath(filePath)
{
	m_proxy.setSourceModel(this);
	m_items.push_back(NodeItem::Holder(new ArcNodeRootItem()));

	m_items.at(RootItemIndex).as<ArcNodeItem>()->lock(tr("Scanning archive..."));
//	addTask(new ReadArchiveTask(m_info.absoluteFilePath(), this), TasksItemList() << m_items.at(RootItemIndex));
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

ICopyControl *ArcNode::createControl(INodeView *view) const
{
	return NULL;
}

void ArcNode::contextMenu(const QModelIndexList &list, INodeView *view)
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
	ArcNodeItem *item;

	if (!list.isEmpty() && !(item = static_cast<ArcNodeItem *>(m_proxy.mapToSource(list.at(0)).internalPointer()))->isRoot())
	{
		QString reason = tr("Canceling...");
		TasksMap::List items = cancelTaskAndTakeItems(NodeItem::Holder(item));

		for (TasksMap::List::size_type i = 0, size = items.size(); i < size; ++i)
		{
			items.at(i).as<ArcNodeItem>()->cancel(reason);
			updateFirstColumn(items.at(i));
		}
	}
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
//		ICopyControl::Holder control(destination->node()->createControl(destination));
//
//		if (control)
//		{
//			ArcNodeItem *item = static_cast<ArcNodeItem *>(index.internalPointer());
//
//			item->lock(tr("Extracting..."));
//			updateFirstColumn(item);
//			addTask(new PerformCopyTask(m_info.absoluteFilePath(), item, control, false, this), TasksItemList() << item);
//		}
	}
}

void ArcNode::move(const INodeView *source, INodeView *destination)
{

}

void ArcNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

QString ArcNode::location() const
{
	return QString();
}

QString ArcNode::location(const QString &fileName) const
{
	return QString();
}

void ArcNode::refresh()
{

}

QString ArcNode::title() const
{
	return m_filePath.mid(m_filePath.lastIndexOf(QChar('/')) + 1);
}

ArcNode::Sorting ArcNode::sorting() const
{
	return Sorting(0, Qt::AscendingOrder);
}

ArcNode::Geometry ArcNode::geometry() const
{
	return Geometry() << 100;
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

::History::Entry *ArcNode::menuAction(QAction *action, INodeView *view)
{
	return NULL;
}

QModelIndex ArcNode::rootIndex() const
{
	return m_proxy.mapFromSource(createIndex(0, 0, m_items.at(RootItemIndex)));
}

Node *ArcNode::viewChild(const QModelIndex &idx, QModelIndex &selected)
{
	if (static_cast<ArcNodeItem *>(m_proxy.mapToSource(idx).internalPointer())->isRoot())
	{
		cancelTask(m_items.at(RootItemIndex));
		return parentNode();
	}

	return NULL;
}

Node *ArcNode::viewChild(const QString &fileName, QModelIndex &selected)
{
	return NULL;
}

void ArcNode::updateProgressEvent(const NodeItem::Holder &item, quint64 progress, quint64 timeElapsed)
{
	item.as<ArcNodeItem>()->updateProgress(progress, timeElapsed);
	updateSecondColumn(item);
}

void ArcNode::completedProgressEvent(const NodeItem::Holder &item, quint64 timeElapsed)
{
	item.as<ArcNodeItem>()->updateProgress(item.as<ArcNodeItem>()->total(), timeElapsed);
	updateSecondColumn(item);
}

void ArcNode::performActionEvent(const AsyncFileAction::FilesList &files)
{

}

void ArcNode::scanCompleteEvent(TaskEvent *e)
{
	ReadArchiveTask::Event *event = static_cast<ReadArchiveTask::Event*>(e);

	if (!event->canceled && !event->contents.items.isEmpty())
	{
		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + event->contents.items.size() - 1);
		m_items.append(event->contents.items);
		endInsertRows();
	}

	m_items.at(RootItemIndex).as<ArcNodeItem>()->unlock();
	updateFirstColumn(m_items.at(RootItemIndex));
	removeAllTaskLinks(event->task);
}

void ArcNode::copyCompleteEvent(TaskEvent *e)
{
	PerformCopyTask::Event *event = static_cast<PerformCopyTask::Event*>(e);

	event->item.as<ArcNodeItem>()->unlock();

	updateFirstColumn(event->item);
	removeAllTaskLinks(event->task);
}

ArcNode::ItemsContainer::ItemsContainer()
{}

ArcNode::ItemsContainer::size_type ArcNode::ItemsContainer::size() const
{
	return m_container.size();
}

ArcNode::ItemsContainer::Item *ArcNode::ItemsContainer::at(size_type index) const
{
	return m_container.at(index).data();
}

ArcNode::ItemsContainer::size_type ArcNode::ItemsContainer::indexOf(Item *item) const
{
	/* FIXME: */
	ArcNodeItem::Holder holder(static_cast<ArcNodeItem *>(item));
	return m_container.indexOf(holder);
}

void ArcNode::updateFirstColumn(const ArcNodeItem::Holder &entry)
{
	QModelIndex index;

	if (ArcNodeListItem *parent = static_cast<ArcNodeListItem *>(entry->parent()))
		index = createIndex(parent->indexOf(entry.data()), 0, entry);
	else
		index = createIndex(m_items.indexOf(entry), 0, entry);

	emit dataChanged(index, index);
}

void ArcNode::updateSecondColumn(const ArcNodeItem::Holder &entry)
{
	QModelIndex index;

	if (ArcNodeListItem *parent = static_cast<ArcNodeListItem *>(entry->parent()))
		index = createIndex(parent->indexOf(entry.data()), 1, entry);
	else
		index = createIndex(m_items.indexOf(entry), 1, entry);

	emit dataChanged(index, index);
}

ARC_PLUGIN_NS_END
