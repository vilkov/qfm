/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "arc_node.h"
#include "items/arc_rootnodeitem.h"
#include "items/arc_entrynodeitem.h"
#include "../tasks/arc_readarchivetask.h"
#include "../tasks/arc_performcopytask.h"
#include "../../../application.h"

#include <QtGui/QMessageBox>


ARC_PLUGIN_NS_BEGIN

ArcNode::ArcNode(IFileContainer::Holder &container, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_container(container.take()),
	m_items(m_itemsContainer.m_container),
    m_delegate(&m_proxy)
{
	m_items.push_back(NodeItem::Holder(new RootNodeItem()));

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);

	m_items.at(RootItemIndex).as<NodeItem>()->lock(tr("Scanning archive..."));
	addTask(new ReadArchiveTask(m_container.data(), this), m_items.at(RootItemIndex));
}

bool ArcNode::event(QEvent *e)
{
	switch (static_cast<FilesBaseTask::Event::Type>(e->type()))
	{
		case FilesBaseTask::Event::ScanComplete:
		{
			e->accept();
			scanCompleteEvent(static_cast<BaseTask::Event *>(e));
			return true;
		}

		case FilesBaseTask::Event::CopyComplete:
		{
			e->accept();
			copyCompleteEvent(static_cast<BaseTask::Event *>(e));
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

void ArcNode::cancel(const QModelIndexList &list, INodeView *view)
{
	QString reason = tr("Canceling...");
	NodeItem *item;
	TasksMap::List items;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
	{
		item = static_cast<NodeItem *>(m_proxy.mapToSource(list.at(i)).internalPointer());
		items = cancelTaskAndTakeItems(NodeItem::Holder(item));

		for (TasksMap::List::size_type i = 0, size = items.size(); i < size; ++i)
		{
			items.at(i).as<NodeItem>()->cancel(reason);
			updateFirstColumn(items.at(i));
		}
	}
}

void ArcNode::refresh()
{
	if (!isUpdating() && m_items.size() == 1)
		addTask(new ReadArchiveTask(m_container.data(), this), m_items.at(RootItemIndex));
}

QString ArcNode::title() const
{
	QString location(m_container->location().as<QString>());
	return location.mid(location.lastIndexOf(QChar(L'/')) + 1);
}

QString ArcNode::location() const
{
	return m_container->location();
}

QString ArcNode::fileName(const QModelIndex &index) const
{
	return QString();
}

bool ArcNode::shortcut(INodeView *view, QKeyEvent *event)
{
	return false;
}

ArcNode::Sorting ArcNode::sorting() const
{
	return Sorting(0, Qt::AscendingOrder);
}

ArcNode::Geometry ArcNode::geometry() const
{
	return Geometry() << 300 << 80 << 50;
}

QAbstractItemModel *ArcNode::model() const
{
	return const_cast<ProxyModel*>(&m_proxy);
}

QAbstractItemDelegate *ArcNode::delegate() const
{
	return const_cast<Delegate*>(&m_delegate);
}

const INodeView::MenuActionList &ArcNode::actions() const
{
	return m_actions;
}

QAbstractItemView::SelectionMode ArcNode::selectionMode() const
{
	return QAbstractItemView::SingleSelection;
}

void ArcNode::menuAction(INodeView *view, QAction *action)
{

}

QModelIndex ArcNode::rootIndex() const
{
	return m_proxy.mapFromSource(createIndex(0, 0, m_items.at(RootItemIndex).data()));
}

QModelIndex ArcNode::childIndex(const QString &fileName)
{
	return QModelIndex();
}

Node *ArcNode::viewChild(const QModelIndex &idx, QModelIndex &selected)
{
	if (static_cast<NodeItem *>(m_proxy.mapToSource(idx).internalPointer())->isRoot())
		return parentNode();
	else
		return NULL;
}

Node *ArcNode::viewChild(const QString &fileName, QModelIndex &selected)
{
	return NULL;
}

void ArcNode::updateProgressEvent(const NodeItem::Holder &item, quint64 progress, quint64 timeElapsed)
{
	item.as<NodeItem>()->updateProgress(progress, timeElapsed);
	updateSecondColumn(item);
}

void ArcNode::completedProgressEvent(const NodeItem::Holder &item, quint64 timeElapsed)
{
	item.as<NodeItem>()->updateProgress(item.as<NodeItem>()->total(), timeElapsed);
	updateSecondColumn(item);
}

void ArcNode::performActionEvent(const AsyncAction::FilesList &files, const QString &error)
{

}

void ArcNode::scanCompleteEvent(BaseTask::Event *e)
{
	ReadArchiveTask::Event *event = static_cast<ReadArchiveTask::Event*>(e);

	if (!event->canceled)
		if (!event->error.isEmpty())
		{
			QMessageBox::critical(
					Application::mainWindow(),
					tr("Error"),
					event->error,
					QMessageBox::Ok);

			m_items.at(RootItemIndex).as<NodeItem>()->cancel(event->error);
		}
		else
		{
			if (!event->snapshot.isEmpty())
			{
				beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + event->snapshot.size() - 1);
				for (Snapshot::const_iterator i = event->snapshot.begin(), end = event->snapshot.end(); i != end; ++i)
					m_items.push_back(NodeItem::Holder(new EntryNodeItem((*i).second, NULL)));
				endInsertRows();
			}

			m_items.at(RootItemIndex).as<NodeItem>()->unlock();
		}

	updateFirstColumn(m_items.at(RootItemIndex));
	removeAllTaskLinks(event->task);
}

void ArcNode::copyCompleteEvent(BaseTask::Event *e)
{
//	PerformCopyTask::Event *event = static_cast<PerformCopyTask::Event*>(e);
//
//	event->item.as<ArcNodeItem>()->unlock();
//
//	updateFirstColumn(event->item);
//	removeAllTaskLinks(event->task);
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

bool ArcNode::isUpdating() const
{
	return m_items.at(RootItemIndex)->isLocked();
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
	NodeItem::Holder holder(static_cast<NodeItem *>(item));
	return m_container.indexOf(holder);
}

void ArcNode::updateFirstColumn(const NodeItem::Holder &entry)
{
	QModelIndex index;

	if (EntryNodeItem *parent = static_cast<EntryNodeItem *>(entry->parent()))
		index = createIndex(parent->indexOf(entry.data()), 0, entry.data());
	else
		index = createIndex(m_items.indexOf(entry), 0, entry.data());

	emit dataChanged(index, index);
}

void ArcNode::updateSecondColumn(const NodeItem::Holder &entry)
{
	QModelIndex index;

	if (EntryNodeItem *parent = static_cast<EntryNodeItem *>(entry->parent()))
		index = createIndex(parent->indexOf(entry.data()), 1, entry);
	else
		index = createIndex(m_items.indexOf(entry), 1, entry);

	emit dataChanged(index, index);
}

ARC_PLUGIN_NS_END
