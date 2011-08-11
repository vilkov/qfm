#include "filesystemfoldernode.h"
#include "items/filesystemfoldernoderoot.h"
#include "items/filesystemfoldernodeentry.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "tasks/perform/performcopytask.h"
#include "tasks/perform/performremovetask.h"
#include "../filesystempluginsmanager.h"
#include "../tools/filesystemcommontools.h"
#include "../../application.h"
#include <QtGui/QMessageBox>
#include <QtGui/QClipboard>


FILE_SYSTEM_NS_BEGIN

FolderNode::FolderNode(const Info &info, Node *parent) :
	FolderNodeBase(parent),
	m_updating(false),
	m_info(info),
	m_proxy(this),
	m_delegate(&m_proxy)
{
	if (!m_info.isRoot())
		m_items.add(new FolderNodeRoot());

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

bool FolderNode::event(QEvent *e)
{
	switch (static_cast<ModelEvent::Type>(e->type()))
	{
		case ModelEvent::UpdateFiles:
		{
			e->accept();
			updateFilesEvent(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::ScanFilesForRemove:
		{
			e->accept();
			scanForRemoveEvent(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::RemoveFiles:
		{
			e->accept();
			removeCompleteEvent(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::ScanFilesForSize:
		{
			e->accept();
			scanForSizeEvent(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::ScanFilesForCopy:
		{
			e->accept();
			scanForCopyEvent(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::CopyFiles:
		{
			e->accept();
			copyCompleteEvent(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::QuestionAnswer:
		{
			e->accept();
			questionAnswerEvent(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::UpdateProgress:
		{
			e->accept();
			updateProgressEvent(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::CompletedProgress:
		{
			e->accept();
			completedProgressEvent(static_cast<ModelEvent*>(e));
			return true;
		}
		default:
			break;
	}

	return QAbstractItemModel::event(e);
}

int FolderNode::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
        return 0;
	else
    	return m_items.size();
}

int FolderNode::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant FolderNode::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<FolderNodeItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row()).item->data(index.column(), role);
}

Qt::ItemFlags FolderNode::flags(const QModelIndex &index) const
{
    if (index.isValid())
    	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::NoItemFlags;
}

QVariant FolderNode::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex FolderNode::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		return createIndex(row, column, m_items.at(row).item);
    else
        return QModelIndex();
}

QModelIndex FolderNode::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

IFileInfo *FolderNode::info(const QModelIndex &idx) const
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<FolderNodeItem*>(index.internalPointer())->isRootItem())
		return static_cast<Node*>(Node::parent());
	else
		return m_items.at(index.row()).item;
}

IFileControl *FolderNode::createControl() const
{
	return new Info(m_info);
}

IFileControl *FolderNode::createControl(const QModelIndex &idx, PluginsManager *plugins)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<FolderNodeItem*>(index.internalPointer())->isRootItem())
		return static_cast<Node*>(Node::parent())->createControl();
	else
	{
		Values::Value &value = m_items[index.row()];

		if (value.node)
			return value.node->createControl();
		else
			if (value.node = createNode(*value.item, plugins))
				return value.node->createControl();
			else
				return new Info(*value.item);
	}
}

bool FolderNode::isDir() const
{
	return true;
}

bool FolderNode::isFile() const
{
	return false;
}

bool FolderNode::exists() const
{
	((FolderNode*)this)->m_info.refresh();
	return m_info.exists();
}

QString FolderNode::fileName() const
{
	if (m_info.isRoot())
		return m_info.absoluteFilePath();
	else
		return m_info.fileName();
}

QString FolderNode::absolutePath() const
{
	return m_info.absolutePath();
}

QString FolderNode::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString FolderNode::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime FolderNode::lastModified() const
{
	return m_info.lastModified();
}

void FolderNode::refresh()
{
	if (isUpdating())
		return;

	if (m_info.isRoot())
		updateFiles();
	else
		if (exists())
			updateFiles();
		else
			removeThis();
}

void FolderNode::remove(const QModelIndexList &list)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForRemove(entries);
}

void FolderNode::cancel(const QModelIndexList &list)
{
	CancelFunctor cancelFunctor(tasks());
	processLockedIndexList(list, cancelFunctor);
}

void FolderNode::calculateSize(const QModelIndexList &list)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForSize(entries);
}

void FolderNode::pathToClipboard(const QModelIndexList &list)
{
	AbsoluteFilePathList pathList;
	processIndexList(list, pathList);
	Application::instance()->clipboard()->setText(pathList.join(QChar('\r')));
}

void FolderNode::copy(const QModelIndexList &list, INode *destination)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, false);
}

void FolderNode::move(const QModelIndexList &list, INode *destination)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, true);
}

void FolderNode::switchTo(Node *node, const QModelIndex &selected)
{
	FolderNodeBase::switchTo(node, selected);

	Node *child;
	for (Values::size_type i = 0, size = m_items.size(); i < size; ++i)
		if (child = m_items.at(i).node)
			child->switchTo(node, selected);
}

QModelIndex FolderNode::rootIndex() const
{
	if (m_info.isRoot())
		return QModelIndex();
	else
		return m_proxy.mapFromSource(createIndex(0, 0, m_items.at(0).item));
}

Node *FolderNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<FolderNodeItem*>(index.internalPointer())->isRootItem())
		return static_cast<Node*>(Node::parent());
	else
		if (!static_cast<FolderNodeEntry*>(index.internalPointer())->isLocked())
		{
			FolderNodeItem *entry = static_cast<FolderNodeItem*>(index.internalPointer());
			entry->refresh();

			if (entry->exists())
			{
				Q_ASSERT(m_items.indexOf(entry) != Values::InvalidIndex);
				Values::Value &value = m_items[m_items.indexOf(entry)];

				if (value.node != 0)
					value.node->setParentEntryIndex(idx);
				else
					if (value.node = createNode(*value.item, plugins))
						value.node->setParentEntryIndex(idx);

				return value.node;
			}
			else
				removeEntry(index);
		}

	return 0;
}

Node *FolderNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	Values::size_type index = m_items.indexOf(fileName);

	if (index == Values::InvalidIndex)
	{
		Info info(absoluteFilePath(fileName));

		if (Node *node = createNode(info, plugins))
		{
			beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
			m_items.add(Values::Value(new FolderNodeEntry(info), node));
			endInsertRows();

			return node;
		}
		else
		{
			if (info.isFile())
			{
				FolderNodeItem *item;

				beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
				m_items.add(Values::Value(item = new FolderNodeEntry(info)));
				endInsertRows();

				selected = indexForFile(item, m_items.lastIndex());
			}

			return this;
		}
	}
	else
	{
		Values::Value &value = m_items[index];

		if (value.node || (value.node = createNode(*value.item, plugins)))
			return value.node;
		else
		{
			if (value.item->isFile())
				selected = indexForFile(value.item, index);

			return this;
		}
	}

	return 0;
}

void FolderNode::CancelFunctor::call(Values::size_type index, FolderNodeItem *entry)
{
	if (TasksPool::Task *task = m_tasks.take(entry->fileName()))
		static_cast<BaseTask*>(task)->cancel();
}

void FolderNode::processIndexList(const QModelIndexList &list, Functors::Functor &functor)
{
	QModelIndex index;
	ProcessedList res;
	FolderNodeItem *entry;
	QSet<FolderNodeItem*> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<FolderNodeItem*>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && !static_cast<FolderNodeEntry*>(entry)->isLocked())
			{
				entry->refresh();

				if (entry->exists())
					functor(index.row(), entry);
				else
					removeEntry(index);
			}
		}
}

void FolderNode::processLockedIndexList(const QModelIndexList &list, Functors::Functor &functor)
{
	QModelIndex index;
	ProcessedList res;
	FolderNodeItem *entry;
	QSet<FolderNodeItem*> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<FolderNodeItem*>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && static_cast<FolderNodeEntry*>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void FolderNode::updateFiles()
{
	if (isVisible())
	{
		UpdatesList::Map changes;

		for (Values::size_type i = m_info.isRoot() ? 0 : 1, size = m_items.size(); i < size; ++i)
			changes.insert(m_items.at(i).item->fileName(), *m_items.at(i).item);

		PScopedPointer<UpdateFilesTask> task(new UpdateFilesTask(this, m_info, changes));
		setUpdating(true);
		Application::instance()->taskPool().handle(task.take());
	}
}

void FolderNode::updateFilesEvent(const ModelEvent *e)
{
	typedef const UpdateFilesTask::Event * Event;
	Event event = static_cast<Event>(e);

	UpdatesList updates = event->updates;
	RangeIntersection updateRange;
	Values::size_type index;

	for (UpdatesList::iterator update = updates.begin(), end = updates.end(); update != end;)
		switch (update.value().type())
		{
			case UpdatesList::Updated:
			{
				if ((index = m_items.indexOf(update.key())) != Values::InvalidIndex)
				{
					(*m_items[index].item) = update.value().info();
					updateRange.add(index, index);
				}

				update = updates.erase(update);
				break;
			}
			case UpdatesList::Deleted:
			{
				if ((index = m_items.indexOf(update.key())) != Values::InvalidIndex &&
					!static_cast<FolderNodeEntry*>(m_items[index].item)->isLocked())
					removeEntry(index);

				update = updates.erase(update);
				break;
			}
			default:
			{
				++update;
				break;
			}
		}

	for (RangeIntersection::RangeList::size_type i = 0, size = updateRange.size(); i < size; ++i)
		emit dataChanged(createIndex(updateRange.at(i).top(), 0),
						 createIndex(updateRange.at(i).bottom(), columnCount(QModelIndex()) - 1));

	if (!updates.isEmpty())
	{
		UpdatesList::Values added = updates.values();

		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + added.size() - 1);
		for (UpdatesList::Values::size_type i = 0, size = added.size(); i < size; ++i)
			m_items.add(new FolderNodeEntry(added.at(i).info()));
		endInsertRows();
	}

	if (event->isLastEvent)
		setUpdating(false);
}

void FolderNode::scanForRemove(const ProcessedList &entries)
{
	FolderNodeEntry *entry;
	Values::size_type index;
	RangeIntersection updateRange;
	ScanFilesForRemoveTask::EntryList list;

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
	{
		index = entries.at(i).first;

		if ((entry = static_cast<FolderNodeEntry*>(entries.at(i).second))->isDir())
			entry->lock(tr("Scanning folder for remove..."));
		else
			entry->lock(tr("Removing..."));

		updateRange.add(index, index);
		list.push_back(entry->fileName());
	}

	PScopedPointer<ScanFilesForRemoveTask> task(new ScanFilesForRemoveTask(this, m_info, list));
	tasks().add(task.data(), list);
	updateFirstColumn(updateRange);
	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::scanForRemoveEvent(const ModelEvent *e)
{
	typedef ScanFilesForRemoveTask::Event * NotConstEvent;
	typedef const ScanFilesForRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	PScopedPointer<FileSystemList> entries(const_cast<NotConstEvent>(event)->entries.take());
	RangeIntersection updateRange;
	Values::size_type index;
	FileSystemItem *entry;
	QStringList folders;
	QStringList files;

	for (FileSystemList::size_type i = 0; i < entries->size();)
	{
		(entry = entries->at(i))->refresh();

		if (entry->exists())
		{
			if (entry->isDir())
				folders.push_back(entry->fileName());
			else
				files.push_back(entry->fileName());

			++i;
		}
		else
		{
			tasks().remove(entry->fileName());
			removeEntry(m_items.indexOf(entry->fileName()));

			if (entry->isDir())
				entries->decTotalSize(static_cast<FileSystemList*>(entry)->totalSize());

			entries->remove(i);
		}
	}

	if (!event->canceled &&
		!entries->isEmpty() &&
		QMessageBox::question(
			&Application::instance()->mainWindow(),
			tr("Remove..."),
			tr("Would you like to remove").
				append(QString::fromLatin1("\n\t")).
				append(tr("directories:")).append(QString::fromLatin1("\n\t\t")).
				append(folders.join(QString::fromLatin1("\n\t\t"))).
				append(QString::fromLatin1("\n\t")).
				append(tr("files:")).append(QString::fromLatin1("\n\t\t")).
				append(files.join(QString::fromLatin1("\n\t\t"))).
				append(QString::fromLatin1("\n")).
				append(tr("it will free ").append(Tools::humanReadableSize(entries->totalSize()))),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		QString lockReason = tr("Removing...");

		for (FileSystemList::size_type i = 0, size = entries->size(); i < size; ++i)
			if ((entry = entries->at(i))->isDir())
			{
				index = m_items.indexOf(entry->fileName());
				static_cast<FolderNodeEntry*>(m_items[index].item)->lock(lockReason, entry->totalSize());
				updateRange.add(index, index);
			}

		PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, entries));
		tasks().resetTask(task.data(), entry->fileName());
		updateSecondColumn(updateRange);
		Application::instance()->taskPool().handle(task.take());
	}
	else
		if (!entries->isEmpty())
		{
			for (FileSystemList::size_type i = 0, size = entries->size(); i < size; ++i)
			{
				index = m_items.indexOf(entries->at(i)->fileName());
				static_cast<FolderNodeEntry*>(m_items[index].item)->unlock();
				updateRange.add(index, index);
			}

			tasks().removeAll(entries->at(0)->fileName());
			updateBothColumns(updateRange);
		}
}

void FolderNode::removeCompleteEvent(const ModelEvent *e)
{
	typedef PerformRemoveTask::Event * NotConstEvent;
	typedef const PerformRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	PScopedPointer<FileSystemList> entries(const_cast<NotConstEvent>(event)->entries.take());
	RangeIntersection updateRange;
	Values::size_type index;
	FileSystemItem *entry;

	for (FileSystemList::size_type i = 0, size = entries->size(); i < size; ++i)
		if ((entry = entries->at(i))->shouldRemove())
			removeEntry(m_items.indexOf(entry->fileName()));
		else
		{
			index = m_items.indexOf(entry->fileName());
			static_cast<FolderNodeEntry*>(m_items[index].item)->clearTotalSize();
			static_cast<FolderNodeEntry*>(m_items[index].item)->unlock();
			updateRange.add(index, index);
		}

	tasks().removeAll(entries->at(0)->fileName());
	updateBothColumns(updateRange);
}

void FolderNode::scanForSize(const ProcessedList &entries)
{
	FolderNodeEntry *entry;
	Values::size_type index;
	RangeIntersection updateRange;
	ScanFilesForSizeTask::EntryList list;

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		if ((entry = static_cast<FolderNodeEntry*>(entries.at(i).second))->isDir())
		{
			index = entries.at(i).first;
			entry->lock(tr("Scanning folder for size..."));
			updateRange.add(index, index);
			list.push_back(entry->fileName());
		}

	if (!list.isEmpty())
	{
		PScopedPointer<ScanFilesForSizeTask> task(new ScanFilesForSizeTask(this, m_info, list));
		tasks().add(task.data(), list);
		updateFirstColumn(updateRange);
		Application::instance()->taskPool().handle(task.take());
	}
}

void FolderNode::scanForSizeEvent(const ModelEvent *e)
{
	typedef ScanFilesForSizeTask::Event * NotConstEvent;
	typedef const ScanFilesForSizeTask::Event * Event;
	Event event = static_cast<Event>(e);

	PScopedPointer<FileSystemList> entries(const_cast<NotConstEvent>(event)->entries.take());
	RangeIntersection updateRange;
	Values::size_type index;
	FolderNodeEntry *entry;

	if (event->canceled)
		for (FileSystemList::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			entry = static_cast<FolderNodeEntry*>(m_items[index = m_items.indexOf(entries->at(i)->fileName())].item);
			entry->clearTotalSize();
			entry->unlock();
			updateRange.add(index, index);
		}
	else
		for (FileSystemList::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			entry = static_cast<FolderNodeEntry*>(m_items[index = m_items.indexOf(entries->at(i)->fileName())].item);
			entry->setTotalSize(static_cast<FileSystemList*>(entries->at(i))->totalSize());
			entry->unlock();
			updateRange.add(index, index);
		}

	tasks().removeAll(entries->at(0)->fileName());
	updateBothColumns(updateRange);
}

void FolderNode::scanForCopy(const ProcessedList &entries, INode *destination, bool move)
{
	FolderNodeEntry *entry;
	Values::size_type index;
	RangeIntersection updateRange;
	QString fileLockReason = move ? tr("Moving...") : tr("Copying...");
	QString folderLockReason = move ? tr("Scanning folder for move...") : tr("Scanning folder for copy...");
	ScanFilesForCopyTask::EntryList list;

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
	{
		index = entries.at(i).first;

		if ((entry = static_cast<FolderNodeEntry*>(entries.at(i).second))->isDir())
			entry->lock(folderLockReason);
		else
			entry->lock(fileLockReason);

		updateRange.add(index, index);
		list.push_back(entry->fileName());
	}

	PScopedPointer<IFileControl> control(destination->createControl());
	PScopedPointer<ScanFilesForCopyTask> task(new ScanFilesForCopyTask(this, m_info, list, control, false));
	tasks().add(task.data(), list);
	updateFirstColumn(updateRange);
	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::scanForCopyEvent(const ModelEvent *e)
{
	typedef ScanFilesForCopyTask::Event * NotConstEvent;
	typedef const ScanFilesForCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	PScopedPointer<FileSystemList> entries(const_cast<NotConstEvent>(event)->entries.take());
	RangeIntersection updateRange;
	Values::size_type index;

	if (event->canceled)
	{
		for (FileSystemList::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			index = m_items.indexOf(entries->at(i)->fileName());
			static_cast<FolderNodeEntry*>(m_items[index].item)->unlock();
			updateRange.add(index, index);
		}

		tasks().removeAll(entries->at(0)->fileName());
		updateBothColumns(updateRange);
	}
	else
	{
		QString lockReason = event->move ? tr("Moving...") : tr("Copying...");
		FileSystemItem *entry;

		for (FileSystemList::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			index = m_items.indexOf((entry = entries->at(i))->fileName());
			static_cast<FolderNodeEntry*>(m_items[index].item)->lock(lockReason, entry->totalSize());
			updateRange.add(index, index);
		}

		PScopedPointer<PerformCopyTask> task(new PerformCopyTask(this, entries, const_cast<NotConstEvent>(event)->destination, event->move));
		tasks().resetTask(task.data(), entry->fileName());
		updateSecondColumn(updateRange);
		Application::instance()->taskPool().handle(task.take());
	}
}

void FolderNode::copyCompleteEvent(const ModelEvent *e)
{
	typedef PerformCopyTask::Event * NotConstEvent;
	typedef const PerformCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	PScopedPointer<FileSystemList> entries(const_cast<NotConstEvent>(event)->entries.take());
	RangeIntersection updateRange;
	Values::size_type index;
	FileSystemItem *entry;

	if (!event->canceled && event->move)
	{
		QString lockReason = tr("Removing...");

		for (FileSystemList::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			index = m_items.indexOf((entry = entries->at(i))->fileName());

			if (entry->isDir())
				static_cast<FolderNodeEntry*>(m_items[index].item)->lock(lockReason, entry->totalSize());
			else
				static_cast<FolderNodeEntry*>(m_items[index].item)->lock(lockReason);

			updateRange.add(index, index);
		}

		PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, entries));
		tasks().resetTask(task.data(), entry->fileName());
		updateSecondColumn(updateRange);
		Application::instance()->taskPool().handle(task.take());
	}
	else
	{
		for (FileSystemList::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			index = m_items.indexOf((entry = entries->at(i))->fileName());
			static_cast<FolderNodeEntry*>(m_items[index].item)->unlock();
			updateRange.add(index, index);
		}

		tasks().removeAll(entries->at(0)->fileName());
		updateBothColumns(updateRange);
	}
}

void FolderNode::questionAnswerEvent(const ModelEvent *e)
{
	typedef BaseTask::QuestionAnswerEvent * NotConstEvent;
	typedef const BaseTask::QuestionAnswerEvent * Event;
	Event event = static_cast<Event>(e);

	event->result()->lock();
	event->result()->setAnswer(QMessageBox::question(&Application::instance()->mainWindow(), event->title(), event->question(), event->buttons()));
	event->result()->unlock();
}

void FolderNode::updateProgressEvent(const ModelEvent *e)
{
	typedef PerformTask::UpdateProgressEvent * NotConstEvent;
	typedef const PerformTask::UpdateProgressEvent * Event;
	Event event = static_cast<Event>(e);

	Values::size_type index = m_items.indexOf(event->fileName);
	FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(m_items.at(index).item);

	entry->setDoneSize(event->progress);
	entry->setTimeElapsed(event->timeElapsed);
	updateSecondColumn(index, entry);
}

void FolderNode::completedProgressEvent(const ModelEvent *e)
{
	typedef PerformTask::CompletedProgressEvent * NotConstEvent;
	typedef const PerformTask::CompletedProgressEvent * Event;
	Event event = static_cast<Event>(e);

	Values::size_type index = m_items.indexOf(event->fileName);
	FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(m_items.at(index).item);

	entry->setDoneSize(entry->totalSize().toULongLong());
	entry->setTimeElapsed(event->timeElapsed);
	updateSecondColumn(index, entry);
}

QModelIndex FolderNode::index(int column, FolderNodeItem *item) const
{
	int index = m_items.indexOf(item);

	if (index != Values::InvalidIndex)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

Node *FolderNode::createNode(const Info &info, PluginsManager *plugins) const
{
	if (Node *res = plugins->node(&info, (FolderNode*)this))
		return res;
	else
		if (info.isDir())
			return new FolderNode(info, (FolderNode*)this);
		else
			return 0;
}

QModelIndex FolderNode::indexForFile(FolderNodeItem *item) const
{
	Q_ASSERT(m_items.indexOf(item) != Values::InvalidIndex);
	Values::size_type index = m_items.indexOf(item);
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

QModelIndex FolderNode::indexForFile(FolderNodeItem *item, Values::size_type index) const
{
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

void FolderNode::updateFirstColumn(FolderNodeItem *entry)
{
	updateFirstColumn(m_items.indexOf(entry), entry);
}

void FolderNode::updateFirstColumn(const RangeIntersection &range)
{
	for (RangeIntersection::RangeList::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0), createIndex(range.at(i).bottom(), 0));
}

void FolderNode::updateFirstColumn(Values::size_type index, FolderNodeItem *entry)
{
	QModelIndex idx = createIndex(index, 0, entry);
	emit dataChanged(idx, idx);
}

void FolderNode::updateSecondColumn(FolderNodeItem *entry)
{
	updateSecondColumn(m_items.indexOf(entry), entry);
}

void FolderNode::updateSecondColumn(const RangeIntersection &range)
{
	for (RangeIntersection::RangeList::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 1), createIndex(range.at(i).bottom(), 1));
}

void FolderNode::updateSecondColumn(Values::size_type index, FolderNodeItem *entry)
{
	QModelIndex idx = createIndex(index, 1, entry);
	emit dataChanged(idx, idx);
}

void FolderNode::updateBothColumns(FolderNodeItem *entry)
{
	updateBothColumns(m_items.indexOf(entry), entry);
}

void FolderNode::updateBothColumns(const RangeIntersection &range)
{
	for (RangeIntersection::RangeList::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0), createIndex(range.at(i).bottom(), 1));
}

void FolderNode::updateBothColumns(Values::size_type index, FolderNodeItem *entry)
{
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, 1, entry));
}

void FolderNode::removeEntry(Values::size_type index)
{
	beginRemoveRows(QModelIndex(), index, index);

	if (Node *node = m_items.at(index).node)
		node->switchTo(this, rootIndex());

	m_items.remove(index);

	endRemoveRows();
}

void FolderNode::removeEntry(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());

	if (Node *node = m_items.at(index.row()).node)
		node->switchTo(this, rootIndex());

	m_items.remove(index.row());

	endRemoveRows();
}

FILE_SYSTEM_NS_END
