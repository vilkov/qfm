#include "idmnodequeryresults.h"
#include "functors/idmqueryresultsfunctor.h"
#include "control/idmqueryresultscopycontrol.h"
#include "events/idmqueryresultsmodelevents.h"
#include "tasks/scan/idmnodequeryresultsscantask.h"
#include "tasks/perform/idmnodequeryresultsperformremovetask.h"
#include "items/idmqueryresultvalueitem.h"
#include "items/idmqueryresultpropertyitem.h"
#include "items/idmqueryresultrootpathvalueitem.h"
#include "items/idmqueryresultpathpropertyitem.h"
#include "items/idmqueryresultcompositerootitem.h"
#include "../../gui/value/list/selectable/selectablevaluelistdialog.h"
#include "../../../default/defaultfoldernode.h"
#include "../../../../tools/containers/union.h"
#include "../../../../tools/widgets/stringdialog/stringdialog.h"
#include "../../../../filesystem/tools/filesystemcommontools.h"
#include "../../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmNodeQueryResults::IdmNodeQueryResults(const IdmContainer &container, const Select &query, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
	m_delegate(container),
	m_container(container),
	m_reader(m_container, query),
	m_label(tr("Found \"%1\" entities...").arg(query.entity()->name()))
{}

bool IdmNodeQueryResults::event(QEvent *e)
{
	switch (static_cast<ModelEvent::Type>(e->type()))
	{
		case ModelEvent::UpdateFiles:
		{
			e->accept();
			scanUpdates(static_cast<BaseTask::Event*>(e));
			return true;
		}
		case ModelEvent::ScanFilesForRemove:
		{
			e->accept();
			scanForRemove(static_cast<BaseTask::Event*>(e));
			return true;
		}
		case ModelEvent::RemoveFiles:
		{
			e->accept();
			performRemove(static_cast<BaseTask::Event*>(e));
			return true;
		}
		default:
			break;
	}

	return TasksNode::event(e);
}

int IdmNodeQueryResults::columnCount(const QModelIndex &parent) const
{
	return 1;
}

void IdmNodeQueryResults::fetchMore(const QModelIndex &parent)
{
	IdmEntityValue::Holder item;
	ItemsContainer::List list;
	Snapshot::Files files(m_container.container());

	list.reserve(PrefetchLimit);

	if (m_reader.entity()->type() == Database::Composite)
		for (qint32 actualLimit = 0; actualLimit < PrefetchLimit; ++actualLimit)
			if (item = m_reader.next())
				list.push_back(NodeItem::Holder(new QueryResultCompositeRootItem(files, m_container.container(), item)));
			else
				break;
	else
		for (qint32 actualLimit = 0; actualLimit < PrefetchLimit; ++actualLimit)
			if (item = m_reader.next())
				list.push_back(NodeItem::Holder(new QueryResultRootItem(item)));
			else
				break;

	if (!list.isEmpty())
	{
		if (!files.isEmpty())
			handleTask(new ScanFilesTask(ModelEvent::UpdateFiles, this, files));

		beginInsertRows(parent, m_items.size(), m_items.size() + list.size() - 1);
		m_items.append(list);
		endInsertRows();
	}
}

bool IdmNodeQueryResults::canFetchMore(const QModelIndex &parent) const
{
    return !parent.isValid() && !m_reader.eof();
}

Qt::ItemFlags IdmNodeQueryResults::flags(const QModelIndex &index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant IdmNodeQueryResults::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (section == 0 && orientation == Qt::Horizontal)
		switch (role)
		{
			case Qt::DisplayRole:
				return m_label;

			case Qt::TextAlignmentRole:
				return Qt::AlignCenter;

			default:
				break;
		}

	return QVariant();
}

ICopyControl *IdmNodeQueryResults::createControl(INodeView *view) const
{
	QModelIndex index = view->currentIndex();

	if (static_cast<QueryResultItem *>(index.internalPointer())->isProperty())
	{
		QueryResultPropertyItem *item = static_cast<QueryResultPropertyItem *>(index.internalPointer());

		if (item->property().entity->type() == Database::Path)
			if (item->size() == 0)
			{
				ICopyControl::Holder control(m_container.container()->createControl(view));

				if (control)
					return new IdmQueryResultsCopyControl(
							control,
							m_container,
							static_cast<QueryResultRootItem *>(item->parent())->value(),
							item->property(),
							const_cast<IdmNodeQueryResults *>(this),
							index);
			}
			else
			{
				QString destination;
				Tools::DestinationFromPathList tree;

				for (QueryResultPropertyItem::size_type i = 0, size = item->size(); i < size; ++i)
					if (static_cast<QueryResultPathItem *>(item->at(i))->isFile())
						tree.add(static_cast<QueryResultPathItem *>(item->at(i))->fileName(), 1);
					else
						tree.add(static_cast<QueryResultPathItem *>(item->at(i))->fileName());

				if (!(destination = tree.choose(tr("Choose a directory"), Application::mainWindow())).isEmpty())
				{
					QString error;
					IFileContainer::Holder container(m_container.container()->open(destination, false, error));

					if (container)
					{
						ICopyControl::Holder control(container->createControl(view));

						return new IdmQueryResultsCopyControl(
								control,
								m_container,
								static_cast<QueryResultRootItem *>(item->parent())->value(),
								item->property(),
								const_cast<IdmNodeQueryResults *>(this),
								index);
					}
					else
						QMessageBox::critical(Application::mainWindow(), tr("Error"), error);
				}
			}
	}

	return NULL;
}

void IdmNodeQueryResults::contextMenu(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeQueryResults::createFile(const QModelIndex &index, INodeView *view)
{
	if (static_cast<QueryResultItem *>(index.internalPointer())->isProperty())
	{
		QueryResultPropertyItem *item = static_cast<QueryResultPropertyItem *>(index.internalPointer());

		if (m_container.transaction())
		{
			SelectableValueListDialog dialog(m_container, Select(item->property().entity), Application::mainWindow());

			if (dialog.exec() == SelectableValueListDialog::Accepted)
			{
				IdmEntityValue::Holder value = dialog.takeValue();

				if (m_container.addValue(static_cast<QueryResultRootItem *>(item->parent())->value(), value))
					if (m_container.commit())
						if (item->property().entity->type() == Database::Path)
						{
							beginInsertRows(index, item->size(), item->size());
							static_cast<QueryResultPathPropertyItem *>(item)->add(m_container.container(), value);
							endInsertRows();
						}
						else
						{
							beginInsertRows(index, item->size(), item->size());
							item->add(value);
							endInsertRows();
						}
					else
					{
						m_container.rollback();
						QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
					}
				else
				{
					m_container.rollback();
					QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
				}
			}
		}
		else
			QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
	}
}

void IdmNodeQueryResults::createDirectory(const QModelIndex &index, INodeView *view)
{
	createFile(index, view);
}

void IdmNodeQueryResults::rename(const QModelIndex &index, INodeView *view)
{
	view->edit(index);
}

void IdmNodeQueryResults::rename(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeQueryResults::remove(const QModelIndexList &list, INodeView *view)
{
	if (m_container.transaction())
	{
		QModelIndex index;
		Snapshot::Files files(m_container.container());
		QueryResultValueItem *valueItem;
		QueryResultRootPathValueItem *pathItem;
		QueryResultPropertyItem *property;
		QueryResultPropertyItem::size_type idx;

		for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
			if (static_cast<QueryResultItem *>((index = list.at(i)).internalPointer())->isRootPathValue())
			{
				pathItem = static_cast<QueryResultRootPathValueItem *>(index.internalPointer());

				if (!pathItem->isLocked())
					files.add(pathItem->fileName(), NodeItem::Holder(pathItem));
			}
			else
				if (static_cast<QueryResultItem *>(index.internalPointer())->isValue())
				{
					valueItem = static_cast<QueryResultValueItem *>(index.internalPointer());

					if (!valueItem->isLocked())
					{
						property = static_cast<QueryResultPropertyItem *>(valueItem->parent());

						if (m_container.removeValue(static_cast<QueryResultRootItem *>(property->parent())->value(), valueItem->value()))
						{
							idx = property->indexOf(valueItem);

							beginRemoveRows(Model::parent(index), idx, idx);
							property->remove(idx);
							endRemoveRows();
						}
						else
						{
							QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
							m_container.rollback();
							return;
						}
					}
				}

		if (m_container.commit())
		{
			if (!files.isEmpty())
			{
				lock(files, tr("Scanning for remove..."));
				addTask(new ScanFilesTask(ModelEvent::ScanFilesForRemove, this, files), files);
			}
		}
		else
		{
			QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
			m_container.rollback();
		}
	}
	else
		QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
}

void IdmNodeQueryResults::cancel(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeQueryResults::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeQueryResults::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeQueryResults::copy(const INodeView *source, INodeView *destination)
{

}

void IdmNodeQueryResults::move(const INodeView *source, INodeView *destination)
{

}

void IdmNodeQueryResults::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

QString IdmNodeQueryResults::location() const
{
	return m_container.container()->location();
}

QString IdmNodeQueryResults::location(const QString &fileName) const
{
	return m_container.container()->location(fileName);
}

void IdmNodeQueryResults::refresh()
{
//	QueryResultItem *item;
//	Snapshot::Files files(m_container.container());
//
//	for (ItemsContainer::size_type i = 0, size = m_items.size(); i < size; ++i)
//		for (QueryResultItem::size_type q = 0, size = (item = static_cast<QueryResultItem *>(m_items.at(i))); q < size; ++q)
//			if (static_cast<QueryResultPropertyItem *>(item->at(q))->property().entity->type() == Database::Path)
//			{
//
//			}
}

QString IdmNodeQueryResults::title() const
{
	QString res = m_container.container()->location();
	return res.mid(res.lastIndexOf(QChar('/')));
}

IdmNodeQueryResults::Sorting IdmNodeQueryResults::sorting() const
{
	return Sorting(0, Qt::AscendingOrder);
}

IdmNodeQueryResults::Geometry IdmNodeQueryResults::geometry() const
{
	return Geometry() << 100;
}

QAbstractItemModel *IdmNodeQueryResults::model() const
{
	return const_cast<IdmNodeQueryResults *>(this);
}

QAbstractItemDelegate *IdmNodeQueryResults::delegate() const
{
	return const_cast<IdmQueryResultsDelegate *>(&m_delegate);
}

const INodeView::MenuActionList &IdmNodeQueryResults::actions() const
{
	return m_actions;
}

::History::Entry *IdmNodeQueryResults::menuAction(QAction *action, INodeView *view)
{
	return NULL;
}

QModelIndex IdmNodeQueryResults::rootIndex() const
{
	return QModelIndex();
}

Node *IdmNodeQueryResults::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	if (idx.isValid())
	{
		QueryResultItem *item = static_cast<QueryResultItem *>(idx.internalPointer());

		if (!item->isLocked() && item->isPath())

			if (Node *node = static_cast<QueryResultPathItem *>(item)->node())
				return node;
			else
			{
				if (static_cast<QueryResultPathItem *>(item)->isDir())
				{
					QString error;
					IFileContainer::Holder folder(m_container.container()->open(static_cast<QueryResultPathItem *>(item)->fileName(), false, error));

					if (folder)
					{
						node = new ::FileSystem::Plugins::Default::FolderNode(folder, this);
						static_cast<QueryResultPathItem *>(item)->setNode(node);
						return node;
					}
					else
						QMessageBox::critical(Application::mainWindow(), tr("Error"), error);
				}
				else
					if (static_cast<QueryResultPathItem *>(item)->isFile())
						static_cast<QueryResultPathItem *>(item)->open();
			}
	}
	else
		return parentNode();

	return NULL;
}

Node *IdmNodeQueryResults::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return NULL;
}

void IdmNodeQueryResults::updateProgressEvent(const NodeItem::Holder &item, quint64 progress, quint64 timeElapsed)
{

}

void IdmNodeQueryResults::completedProgressEvent(const NodeItem::Holder &item, quint64 timeElapsed)
{

}

void IdmNodeQueryResults::performActionEvent(const AsyncFileAction::FilesList &files)
{

}

void IdmNodeQueryResults::add(const QModelIndex &index, const IdmCompositeEntityValue::List &values)
{
	QueryResultPathPropertyItem *item = static_cast<QueryResultPathPropertyItem *>(index.internalPointer());

	beginInsertRows(index, item->size(), item->size() + values.size() - 1);
	item->add(m_container.container(), values);
	endInsertRows();
}

void IdmNodeQueryResults::remove(const QModelIndex &index, const IdmCompositeEntityValue::List &values)
{

}

void IdmNodeQueryResults::refresh(const QModelIndex &index)
{
	Snapshot::Files files(m_container.container());
	QueryResultItem *item = static_cast<QueryResultItem *>(index.internalPointer());

	for (QueryResultItem::size_type i = 0, size = item->size(); i < size; ++i)
		files.add(static_cast<QueryResultPathItem *>(item->at(i))->fileName(), NodeItem::Holder(static_cast<QueryResultItem *>(item->at(i))));

	if (!files.isEmpty())
		handleTask(new ScanFilesTask(ModelEvent::UpdateFiles, this, files));
}

void IdmNodeQueryResults::process(const QModelIndexList &list, const Functor &functor)
{
	QueryResultItem *item;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if ((item = static_cast<QueryResultItem *>(list.at(i).internalPointer()))->isValue())
			functor(list.at(i), static_cast<QueryResultValueItem *>(item));
}

void IdmNodeQueryResults::processRemove(const QModelIndexList &list, const Functor &functor)
{
	QueryResultItem *item;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if ((item = static_cast<QueryResultItem *>(list.at(i).internalPointer()))->isValue())
			functor(list.at(i), static_cast<QueryResultValueItem *>(item));
}

void IdmNodeQueryResults::doRemove(INodeView *view, const QModelIndex &index, QueryResultValueItem *value)
{
	if (m_container.transaction())
	{
		QueryResultPropertyItem *property = static_cast<QueryResultPropertyItem*>(value->parent());

		if (m_container.removeValue(static_cast<QueryResultRootItem *>(property->parent())->value(), value->value()))
			if (m_container.commit())
			{
				beginRemoveRows(Model::parent(index), index.row(), index.row());
				property->remove(index.row());
				endRemoveRows();
			}
			else
			{
				m_container.rollback();
				QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
			}
		else
		{
			m_container.rollback();
			QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
		}
	}
	else
	{
		m_container.rollback();
		QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
	}
}

void IdmNodeQueryResults::scanUpdates(const BaseTask::Event *e)
{
	typedef const ScanFilesTask::UpdatesEvent *Event;
	typedef ScanFilesTask::UpdatesEvent *NotConstEvent;
	Event event = static_cast<Event>(e);

	update(const_cast<NotConstEvent>(event)->snapshot);
	taskHandled(event->task);
}

void IdmNodeQueryResults::scanForRemove(const BaseTask::Event *e)
{
	typedef const ScanFilesTask::Event *Event;
	typedef ScanFilesTask::Event *NotConstEvent;
	Event event = static_cast<Event>(e);

	if (!event->canceled)
	{
		QString question;
		QStringList removed;
		QStringList folders;
		QStringList files;
		WrappedNodeItem *entry;
		ItemsContainer::List removedItems;

		for (Snapshot::iterator i = const_cast<NotConstEvent>(event)->snapshot.begin(), end = const_cast<NotConstEvent>(event)->snapshot.end(); i != end;)
			if (entry = (*i).second)
			{
				if (entry->info()->isDir())
					folders.push_back(entry->info()->fileName());
				else
					files.push_back(entry->info()->fileName());

				++i;
			}
			else
			{
				removed.push_back((*i).first.as<QueryResultPathItem>()->fileName());
				removedItems.push_back((*i).first);

				i = const_cast<NotConstEvent>(event)->snapshot.erase(i);
			}

		question = tr("Would you like to remove").
				append(QString::fromLatin1("\n\t")).
				append(tr("directories:")).append(QString::fromLatin1("\n\t\t")).
				append(folders.join(QString::fromLatin1("\n\t\t"))).

				append(QString::fromLatin1("\n\t---\n\t")).
				append(tr("files:")).append(QString::fromLatin1("\n\t\t")).
				append(files.join(QString::fromLatin1("\n\t\t"))).

				append(QString::fromLatin1("\n\t---\n\t")).
				append(tr("doesn't exist:")).append(QString::fromLatin1("\n\t\t")).
				append(removed.join(QString::fromLatin1("\n\t\t"))).

				append(QString::fromLatin1("\n---\n")).
				append(tr("it will free ").append(Tools::humanReadableSize(event->snapshot.totalSize())));

		if (QMessageBox::question(
				Application::mainWindow(),
				tr("Remove..."),
				question,
				QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			lock(event->snapshot, tr("Removing..."));
			resetTask(new PerformRemoveTask(this, event->snapshot), event->task);
			return;
		}
	}

	unlock(event->snapshot);
	removeAllTaskLinks(event->task);
}

void IdmNodeQueryResults::performRemove(const BaseTask::Event *e)
{
	typedef const PerformRemoveTask::Event *Event;
	QueryResultPropertyItem::size_type idx;
	Event event = static_cast<Event>(e);
	QueryResultPropertyItem *property;
	QModelIndex modelIdx;

	if (m_container.transaction())
	{
		bool ok = true;

		for (Snapshot::const_iterator i = event->snapshot.begin(), end = event->snapshot.end(); i != end; ++i)
			if ((*i).second->isRemoved())
			{
				property = static_cast<QueryResultPropertyItem *>((*i).first->parent());

				if (m_container.removeValue(static_cast<QueryResultRootItem *>(property->parent())->value(), (*i).first.as<QueryResultValueItem>()->value()))
				{
					idx = property->indexOf((*i).first.data());

					beginRemoveRows(index(property), idx, idx);
					property->remove(idx);
					endRemoveRows();
				}
				else
				{
					QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
					m_container.rollback();
					ok = false;
					break;
				}
			}

		if (ok)
			if (!m_container.commit())
			{
				QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
				m_container.rollback();
			}
	}
	else
		QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());

	for (Snapshot::const_iterator i = event->snapshot.begin(), end = event->snapshot.end(); i != end; ++i)
		if (!(*i).second->isRemoved())
		{
			(*i).first.as<QueryResultValueItem>()->unlock();
			modelIdx = index((*i).first.data());
			emit dataChanged(modelIdx, modelIdx);
		}

	removeAllTaskLinks(event->task);
}

void IdmNodeQueryResults::lock(const Snapshot &snapshot, const QString &reason)
{
	typedef QMap<QueryResultItem*, Union> Map;
	qint32 lastColumn = columnCount(QModelIndex()) - 1;
	QueryResultPropertyItem *property;
	Map map;

	for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
	{
		property = static_cast<QueryResultPropertyItem *>((*i).first->parent());
		map[property].add(property->indexOf((*i).first.data()));
		(*i).first.as<QueryResultValueItem>()->lock(reason);
	}

	for (Map::const_iterator i = map.constBegin(), end = map.constEnd(); i != end; ++i)
		for (Union::List::size_type q = 0, size = (*i).size(); q < size; ++q)
			emit dataChanged(createIndex((*i).at(q).top(), 0, i.key()->at((*i).at(q).top())),
							 createIndex((*i).at(q).bottom(), lastColumn, i.key()->at((*i).at(q).bottom())));
}

void IdmNodeQueryResults::unlock(const Snapshot &snapshot)
{
	typedef QMap<QueryResultItem*, Union> Map;
	qint32 lastColumn = columnCount(QModelIndex()) - 1;
	QueryResultPropertyItem *property;
	Map map;

	for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
	{
		property = static_cast<QueryResultPropertyItem *>((*i).first->parent());
		map[property].add(property->indexOf((*i).first.data()));
		(*i).first.as<QueryResultValueItem>()->unlock();
	}

	for (Map::const_iterator i = map.constBegin(), end = map.constEnd(); i != end; ++i)
		for (Union::List::size_type q = 0, size = (*i).size(); q < size; ++q)
			emit dataChanged(createIndex((*i).at(q).top(), 0, i.key()->at((*i).at(q).top())),
							 createIndex((*i).at(q).bottom(), lastColumn, i.key()->at((*i).at(q).bottom())));
}

void IdmNodeQueryResults::update(Snapshot &updates)
{
	typedef QMap<QueryResultItem*, Union> Map;
	qint32 lastColumn = columnCount(QModelIndex()) - 1;
	QueryResultPropertyItem *property;
	QueryResultItem *item;
	Map map;

	for (Snapshot::iterator i = updates.begin(), end = updates.end(); i != end; i = updates.erase(i))
	{
		item = (*i).first.as<QueryResultItem>();
		property = static_cast<QueryResultPropertyItem *>(item->parent());
		map[property].add(property->indexOf(item));

		if (item->size())
		{
			beginRemoveRows(Model::index(item), 0, item->size() - 1);
			static_cast<QueryResultRootPathValueItem *>(item)->update((*i).second);
			endRemoveRows();
		}
		else
			static_cast<QueryResultRootPathValueItem *>(item)->update((*i).second);

		static_cast<QueryResultValueItem *>(item)->unlock();
	}

	for (Map::const_iterator i = map.constBegin(), end = map.constEnd(); i != end; ++i)
		for (Union::List::size_type q = 0, size = (*i).size(); q < size; ++q)
			emit dataChanged(createIndex((*i).at(q).top(), 0, i.key()->at((*i).at(q).top())),
							 createIndex((*i).at(q).bottom(), lastColumn, i.key()->at((*i).at(q).bottom())));
}

IdmNodeQueryResults::ItemsContainer::ItemsContainer() :
	TasksNode::Container()
{}

IdmNodeQueryResults::ItemsContainer::size_type IdmNodeQueryResults::ItemsContainer::size() const
{
	return m_container.size();
}

IdmNodeQueryResults::ItemsContainer::Item *IdmNodeQueryResults::ItemsContainer::at(size_type index) const
{
	return m_container.at(index).data();
}

IdmNodeQueryResults::ItemsContainer::size_type IdmNodeQueryResults::ItemsContainer::indexOf(Item *item) const
{
	NodeItem::Holder holder(static_cast<NodeItem *>(item));
	return m_container.indexOf(holder);
}

IDM_PLUGIN_NS_END
