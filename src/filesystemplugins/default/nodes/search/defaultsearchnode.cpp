#include "defaultsearchnode.h"
#include "tasks/defaultsearchtask.h"
#include "items/defaultsearchnodeitem.h"
#include "../../model/items/defaultrootnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

SearchNode::SearchNode(IFileContainer::Holder &container, Node *parent) :
	BaseNode(container, parent)
{
	RootNodeItem::Holder item(new RootNodeItem());
	items().add(item.as<RootNodeItem>()->label().toString(), item);

	items()[RootItemIndex].as<SearchNodeItem>()->lock(tr("Searching..."));
	addTask(new SearchTask(BaseNode::container().data(), this), items()[RootItemIndex]);
}

bool SearchNode::event(QEvent *e)
{
	switch (static_cast<FilesBaseTask::Event::Type>(e->type()))
	{
		case SearchTask::Event::SearchFiles:
		{
			e->accept();
			searchCompleteEvent(static_cast<BaseTask::Event *>(e));
			return true;
		}

		default:
			return BaseNode::event(e);
	}
}

ICopyControl *SearchNode::createControl(INodeView *view) const
{
	return NULL;
}

void SearchNode::contextMenu(const QModelIndexList &list, INodeView *view)
{

}

void SearchNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void SearchNode::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void SearchNode::rename(const QModelIndex &index, INodeView *view)
{

}

void SearchNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void SearchNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void SearchNode::cancel(const QModelIndexList &list, INodeView *view)
{
//	QString reason = tr("Canceling...");
//	SearchNodeItem *item;
//	TasksMap::List items;
//
//	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
//	{
//		item = static_cast<SearchNodeItem *>(m_proxy.mapToSource(list.at(i)).internalPointer());
//		items = cancelTaskAndTakeItems(NodeItem::Holder(item));
//
//		for (TasksMap::List::size_type i = 0, size = items.size(); i < size; ++i)
//		{
//			items.at(i).as<SearchNodeItem>()->cancel(reason);
//			updateFirstColumn(items.at(i));
//		}
//	}
}

void SearchNode::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void SearchNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void SearchNode::copy(const INodeView *source, INodeView *destination)
{
//	QModelIndex index = m_proxy.mapToSource(source->currentIndex());
//
//	if (index.isValid())
//	{
//		ICopyControl::Holder control(destination->node()->createControl(destination));
//
//		if (control)
//		{
//			SearchNodeItem *item = static_cast<SearchNodeItem *>(index.internalPointer());
//
//			item->lock(tr("Extracting..."));
//			updateFirstColumn(item);
//			addTask(new PerformCopyTask(m_info.absoluteFilePath(), item, control, false, this), TasksItemList() << item);
//		}
//	}
}

void SearchNode::move(const INodeView *source, INodeView *destination)
{

}

void SearchNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

::History::Entry *SearchNode::search(const QModelIndex &index, INodeView *view)
{
	return NULL;
}

void SearchNode::refresh()
{

}

QModelIndex SearchNode::rootIndex() const
{
	return proxy().mapFromSource(createIndex(0, 0, items()[RootItemIndex].data()));
}

Node *SearchNode::viewChild(const QModelIndex &idx, QModelIndex &selected)
{
	if (static_cast<SearchNodeItem *>(proxy().mapToSource(idx).internalPointer())->isRootItem())
		return parentNode();
	else
		return NULL;
}

Node *SearchNode::viewChild(const QString &fileName, QModelIndex &selected)
{
	return NULL;
}

void SearchNode::searchCompleteEvent(BaseTask::Event *e)
{
	SearchTask::Event *event = static_cast<SearchTask::Event *>(e);

	if (!event->canceled)
	{
		if (!event->snapshot.isEmpty())
		{
			NodeItem::Holder item;

			beginInsertRows(QModelIndex(), items().size(), items().size() + event->snapshot.size() - 1);
			for (Snapshot::const_iterator i = event->snapshot.begin(), end = event->snapshot.end(); i != end; ++i)
			{
				item = new SearchNodeItem((*i).second, NULL);
				items().add(item.as<SearchNodeItem>()->info()->fileName(), item);
			}
			endInsertRows();
		}

		items()[RootItemIndex].as<SearchNodeItem>()->unlock();
	}

	updateFirstColumn();
	removeAllTaskLinks(event->task);
}

void SearchNode::updateFirstColumn()
{
	QModelIndex index(createIndex(RootItemIndex, 0, items()[RootItemIndex].data()));
	emit dataChanged(index, index);
}

void SearchNode::updateSecondColumn()
{
	QModelIndex index(createIndex(RootItemIndex, 1, items()[RootItemIndex].data()));
	emit dataChanged(index, index);
}

DEFAULT_PLUGIN_NS_END
