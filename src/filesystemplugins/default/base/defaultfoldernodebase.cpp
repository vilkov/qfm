#include "defaultfoldernodebase.h"
#include "defaultfoldernodebase_p.h"
#include "../tasks/scan/defaultscanfilestask.h"
#include "../tasks/perform/defaultperformcopytask.h"
#include "../tasks/perform/defaultperformmovetask.h"
#include "../tasks/perform/defaultperformremovetask.h"
#include "../actions/defaultfoldercopyaction.h"
#include "../actions/defaultfoldercutaction.h"
#include "../actions/defaultfolderpasteaction.h"
#include "../actions/defaultfolderpasteintofolderaction.h"
#include "../actions/defaultfolderpropertiesaction.h"
#include "../actions/defaultfolderpasteclipboardaction.h"
#include "../../../filesystem/tasks/filesystemperformactiontask.h"
#include "../../../filesystem/tools/filesystemcommontools.h"
#include "../../../tools/widgets/stringdialog/stringdialog.h"
#include "../../../application.h"

#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>


DEFAULT_PLUGIN_NS_BEGIN

struct GlobalActions
{
	GlobalActions() :
		copyAction(NULL),
		cutAction(NULL),
		pasteAction(NULL),
		pasteIntoFolderAction(NULL),
		propertiesAction(NULL),
		pasteClipboardAction(NULL),
		m_ref(0)
	{}

	void addRef()
	{
		if (m_ref++ == 0)
		{
			copyAction = new FolderCopyAction();
			cutAction = new FolderCutAction();
			pasteAction = new FolderPasteAction();
			pasteIntoFolderAction = new FolderPasteIntoFolderAction();
			propertiesAction = new FolderPropertiesAction();
			pasteClipboardAction = new FolderPasteClipboardAction();
		}
	}

	void release()
	{
		if (--m_ref == 0)
		{
			delete copyAction;
			delete cutAction;
			delete pasteAction;
			delete pasteIntoFolderAction;
			delete propertiesAction;
			delete pasteClipboardAction;
		}
	}

	FolderCopyAction *copyAction;
	FolderCutAction *cutAction;
	FolderPasteAction *pasteAction;
	FolderPasteIntoFolderAction *pasteIntoFolderAction;
	FolderPropertiesAction *propertiesAction;
	FolderPasteClipboardAction *pasteClipboardAction;

private:
	int m_ref;
};

static GlobalActions globalActions;


FolderNodeBase::FolderNodeBase(IFileContainer::Holder &container, Node *parent) :
	TasksNode(m_items, parent),
	m_container(container.take()),
	m_updating(false),
	m_proxy(this),
	m_delegate(&m_proxy)
{
	globalActions.addRef();

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

FolderNodeBase::~FolderNodeBase()
{
	globalActions.release();
}

int FolderNodeBase::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant FolderNodeBase::headerData(int section, Qt::Orientation orientation, int role) const
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

bool FolderNodeBase::event(QEvent *e)
{
	switch (static_cast<FilesBaseTask::Event::Type>(e->type()))
	{
		case FilesBaseTask::Event::UpdateFiles:
		{
			e->accept();
			updateFiles(static_cast<BaseTask::Event*>(e));
			return true;
		}
		case FilesBaseTask::Event::ScanFilesForRemove:
		{
			e->accept();
			scanForRemove(static_cast<BaseTask::Event*>(e));
			return true;
		}
		case FilesBaseTask::Event::RemoveFiles:
		{
			e->accept();
			performRemove(static_cast<BaseTask::Event*>(e));
			return true;
		}
		case FilesBaseTask::Event::ScanFilesForSize:
		{
			e->accept();
			scanForSize(static_cast<BaseTask::Event*>(e));
			return true;
		}
		case FilesBaseTask::Event::ScanFilesForCopy:
		{
			e->accept();
			scanForCopy(static_cast<BaseTask::Event*>(e));
			return true;
		}
		case FilesBaseTask::Event::CopyFiles:
		{
			e->accept();
			performCopy(static_cast<BaseTask::Event*>(e));
			return true;
		}
		default:
			break;
	}

	return TasksNode::event(e);
}

void FolderNodeBase::refresh()
{
	if (!isUpdating())
		updateFiles();
}

QString FolderNodeBase::title() const
{
	QString res = m_container->location();
	return res.mid(res.lastIndexOf(QChar('/')) + 1);
}

FolderNodeBase::Sorting FolderNodeBase::sorting() const
{
	return Sorting(0, Qt::AscendingOrder);
}

FolderNodeBase::Geometry FolderNodeBase::geometry() const
{
	return Geometry() << 300 << 80 << 50;
}

QAbstractItemModel *FolderNodeBase::model() const
{
	return const_cast<FolderProxyModel *>(&m_proxy);
}

QAbstractItemDelegate *FolderNodeBase::delegate() const
{
	return const_cast<FolderDelegate *>(&m_delegate);
}

const INodeView::MenuActionList &FolderNodeBase::actions() const
{
	return m_menuActions;
}

::History::Entry *FolderNodeBase::menuAction(QAction *action, INodeView *view)
{
	return NULL;
}

QString FolderNodeBase::location() const
{
	return m_container->location();
}

QString FolderNodeBase::location(const QString &fileName) const
{
	return m_container->location(fileName);
}

ICopyControl *FolderNodeBase::createControl(INodeView *view) const
{
	return m_container->createControl(view);
}

void FolderNodeBase::contextMenu(const QModelIndexList &list, INodeView *view)
{
	typedef QSet<NodeItem::Holder>                                    ItemsSet;
	typedef QList<NodeItem::Holder>                                   ItemsList;
	typedef QMap<const FileAction *, FileAction::FilesList>           ActionsMap;
	typedef QMap<NodeItem::Holder, ::Tools::Containers::Dot>          ItemsIndexMap;
	typedef ::DesktopEnvironment::ContextMenuFactory::FileActionsList FileActionsList;

	QMenu menu;
	ItemsSet set;
	ActionsMap map;
	ItemsList items;
	QModelIndex index;
	ItemsIndexMap itemsIndex;
	NodeItem::Holder item;
	FileActionsList actions;

	for (ItemsList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!(item = m_items[(index = m_proxy.mapToSource(list.at(i))).row()]).as<DefaultNodeItem>()->isRootItem() && !set.contains(item))
		{
			set.insert(item);
			itemsIndex[item] = index.row();
		}

	items = set.toList();

	menu.addAction(const_cast<QAction*>(globalActions.copyAction->action()));
	menu.addAction(const_cast<QAction*>(globalActions.cutAction->action()));

	if (items.isEmpty())
	{
		menu.addAction(const_cast<QAction*>(globalActions.pasteClipboardAction->action()));
	}
	else
	{
		if (items.size() == 1)
		{
			if ((item = items.at(0)).as<DefaultNodeItem>()->info()->isDir())
			{
				menu.addAction(const_cast<QAction*>(globalActions.pasteIntoFolderAction->action()));

				actions = Application::globalMenu()->actions(::DesktopEnvironment::ContextMenuFactory::SingleFolder);

				for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
					map[actions.at(i)].push_back(FileAction::FilesList::value_type(item, item.as<DefaultNodeItem>()->info().data()));
			}
			else
			{
				menu.addAction(const_cast<QAction*>(globalActions.pasteAction->action()));

				actions = Application::globalMenu()->actions(::DesktopEnvironment::ContextMenuFactory::SingleFile);

				for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
					map[actions.at(i)].push_back(FileAction::FilesList::value_type(item, item.as<DefaultNodeItem>()->info().data()));
			}
		}
		else
		{
			menu.addAction(const_cast<QAction*>(globalActions.pasteAction->action()));

			actions = Application::globalMenu()->actions(::DesktopEnvironment::ContextMenuFactory::MultipleFilesOrFolders);

			for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
			{
				FileAction::FilesList &files = map[actions.at(i)];

				for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
				{
					item = items.at(i);
					files.push_back(FileAction::FilesList::value_type(item, item.as<DefaultNodeItem>()->info().data()));
				}
			}
		}

		for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
		{
			actions = Application::globalMenu()->actions((item = items.at(i)).as<DefaultNodeItem>()->info()->fileType()->id());

			for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
				map[actions.at(i)].push_back(FileAction::FilesList::value_type(item, item.as<DefaultNodeItem>()->info().data()));
		}
	}

	actions = Application::globalMenu()->actions(::DesktopEnvironment::ContextMenuFactory::AnyFilesOrFolders);

	for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
	{
		FileAction::FilesList &files = map[actions.at(i)];

		for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
		{
			item = items.at(i);
			files.push_back(FileAction::FilesList::value_type(item, item.as<DefaultNodeItem>()->info().data()));
		}
	}

	menu.addSeparator();

	if (!map.isEmpty())
	{
		for (ActionsMap::const_iterator it = map.begin(), end = map.end(); it != end; ++it)
			menu.addAction(const_cast<QAction*>(it.key()->action()));

		menu.addSeparator();
	}

	menu.addAction(const_cast<QAction*>(globalActions.propertiesAction->action()));

	if (FileAction *action = FileAction::fromQAction(menu.exec(QCursor::pos())))
	{
		FileAction::FilesList files = map.value(action);

		if (files.isEmpty())
			for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
			{
				item = items.at(i);
				files.push_back(FileAction::FilesList::value_type(item, item.as<DefaultNodeItem>()->info().data()));
			}

		if (action->isAsynchronous())
		{
			PScopedPointer<PerformActionTask> task;

			if (task = static_cast<AsyncFileAction *>(action)->process(this, m_container.data(), files))
			{
				Union update;
				Snapshot::Files list(m_container.data());

				for (FileAction::FilesList::size_type i = 0, size = files.size(); i < size; ++i)
				{
					item = files.at(i).first;

					item->lock(static_cast<AsyncFileAction *>(action)->lockReason());
					update.add(itemsIndex.value(item));
					list.add(item.as<DefaultNodeItem>()->info()->fileName(), item);
				}

				addTask(task.take(), list);
				updateFirstColumn(update);
			}
		}
		else
			static_cast<SyncFileAction *>(action)->process(m_container.data(), files);
	}

	menu.clear();
}

void FolderNodeBase::createFile(const QModelIndex &index, INodeView *view)
{

}

void FolderNodeBase::createDirectory(const QModelIndex &index, INodeView *view)
{
	QString name;

	if (index.isValid())
	{
		DefaultNodeItem *item = m_items[m_proxy.mapToSource(index).row()].as<DefaultNodeItem>();

		if (!item->isRootItem())
			name = item->info()->fileName();
	}

	StringDialog dialog(tr("Enter name for new directory"), tr("Name"), name, Application::mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{
		QString error;
		IFileContainer::Holder folder(m_container->open(dialog.value(), true, error));

		if (folder)
		{
			DefaultNodeItem *item;
			IFileInfo::Holder info(m_container->scanner()->info(dialog.value(), error));

			beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
			m_items.add(NodeItem::Holder(item = new DefaultNodeItem(info)));
			endInsertRows();

			view->select(indexForFile(item, m_items.size() - 1));
		}
		else
			QMessageBox::critical(Application::mainWindow(), tr("Failed to create directory..."), error);
	}
}

void FolderNodeBase::rename(const QModelIndex &index, INodeView *view)
{
	DefaultNodeItem *entry = static_cast<DefaultNodeItem *>(m_proxy.mapToSource(index).internalPointer());

	if (!entry->isRootItem() && !static_cast<DefaultNodeItem *>(entry)->isLocked())
	{
		RenameFunctor functor(m_container.data(), m_items);
		functor(index.row(), entry);
	}
}

void FolderNodeBase::rename(const QModelIndexList &list, INodeView *view)
{
	RenameFunctor functor(m_container.data(), m_items);
	processIndexList(list, functor);
}

void FolderNodeBase::remove(const QModelIndexList &list, INodeView *view)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForRemove(entries);
}

void FolderNodeBase::cancel(const QModelIndexList &list, INodeView *view)
{
	CancelFunctor cancelFunctor(this, tr("Canceling..."));
	processLockedIndexList(list, cancelFunctor);
	updateFirstColumn(cancelFunctor.updateUnion());
}

void FolderNodeBase::calculateSize(const QModelIndexList &list, INodeView *view)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForSize(entries);
}

void FolderNodeBase::pathToClipboard(const QModelIndexList &list, INodeView *view)
{
	AbsoluteFilePathList pathList(m_container.data());
	processIndexList(list, pathList);
	Application::instance()->clipboard()->setText(pathList.join(QChar('\r')));
}

void FolderNodeBase::copy(const INodeView *source, INodeView *destination)
{
	ProcessedList entries;
	processIndexList(source->selectedIndexes(), entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, false);
}

void FolderNodeBase::move(const INodeView *source, INodeView *destination)
{
	ProcessedList entries;
	processIndexList(source->selectedIndexes(), entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, true);
}

void FolderNodeBase::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

QModelIndex FolderNodeBase::rootIndex() const
{
	return QModelIndex();
}

Node *FolderNodeBase::viewChild(const QModelIndex &idx, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<DefaultNodeItem *>(index.internalPointer())->isRootItem())
		return parentNode();
	else
		if (!static_cast<DefaultNodeItem *>(index.internalPointer())->isLocked())
		{
			DefaultNodeItem *entry = static_cast<DefaultNodeItem *>(index.internalPointer());

			if (entry->node())
				entry->node()->setParentEntryIndex(idx);
			else
				if (Node *node = createNode(entry->info().data()))
				{
					entry->setNode(node);
					node->setParentEntryIndex(idx);
				}

			if (entry->node())
				return entry->node();
			else
				if (entry->info()->isFile())
					Application::desktopService()->open(m_container.data(), entry->info().data());
		}

	return NULL;
}

Node *FolderNodeBase::viewChild(const QString &fileName, QModelIndex &selected)
{
	Container::size_type index = m_items.indexOf(fileName);

	if (index == Container::InvalidIndex)
	{
		QString error;
		IFileInfo::Holder info;

		if (info = m_container->scanner()->info(fileName, error))
			if (Node *node = createNode(info.data()))
			{
				beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
				m_items.add(NodeItem::Holder(new DefaultNodeItem(info, node)));
				endInsertRows();

				return node;
			}
			else
			{
				if (info->isFile())
				{
					DefaultNodeItem *item;

					beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
					m_items.add(NodeItem::Holder(item = new DefaultNodeItem(info)));
					endInsertRows();

					selected = indexForFile(item, m_items.lastIndex());
				}

				return this;
			}
	}
	else
	{
		DefaultNodeItem *item = m_items[index].as<DefaultNodeItem>();

		if (item->node())
			return item->node();
		else
			if (Node *node = createNode(item->info().data()))
			{
				item->setNode(node);
				return node;
			}
			else
			{
				if (item->info()->isFile())
					selected = indexForFile(item, index);

				return this;
			}
	}

	return NULL;
}

void FolderNodeBase::nodeRemoved(Node *node)
{
	Container::size_type index = m_items.indexOf(node);

	if (index != Container::InvalidIndex)
		m_items[index].as<DefaultNodeItem>()->setNode(NULL);
}

void FolderNodeBase::cleanup(Snapshot &snapshot)
{
	Container::size_type index;

	for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
		if (snapshot.isRemoved(i))
		{
			if ((index = m_items.indexOf((*i).first.as<DefaultNodeItem>()->info()->fileName())) != Container::InvalidIndex)
				removeEntry(index);

			i = snapshot.erase(i);
		}
		else
			++i;
}

void FolderNodeBase::processScanEventSnapshot(Snapshot &snapshot, EventFunctor &functor)
{
	DefaultNodeItem *entry;
	Container::size_type index;

	cleanup(snapshot);

	for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
		if ((index = m_items.indexOf((entry = (*i).first.as<DefaultNodeItem>())->info()->fileName())) != Container::InvalidIndex)
		{
			functor(index, entry, (*i).second);
			++i;
		}
		else
			i = snapshot.erase(i);
}

void FolderNodeBase::processPerformEventSnapshot(Snapshot &snapshot, EventFunctor &functor)
{
	DefaultNodeItem *entry;
	Container::size_type index;

	for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
		if ((index = m_items.indexOf((entry = (*i).first.as<DefaultNodeItem>())->info()->fileName())) != Container::InvalidIndex)
		{
			functor(index, entry, (*i).second);
			++i;
		}
		else
			i = snapshot.erase(i);
}

Snapshot FolderNodeBase::updateFilesList() const
{
	Snapshot::Files files(m_container.data());

	for (Container::size_type i = 0, size = m_items.size(); i < size; ++i)
		files.add(m_items[i].as<DefaultNodeItem>()->info()->fileName(), m_items[i]);

	return files;
}

void FolderNodeBase::updateFilesEvent(Snapshot &updates)
{
	Union updateRange;
	Container::size_type index;

	for (Snapshot::iterator update = updates.begin(), end = updates.end(); update != end;)
		if (updates.isAdded(update) || updates.isUpdated(update))
		{
			if ((index = m_items.indexOf((*update).second->info()->fileName())) != Container::InvalidIndex)
			{
				m_items[index].as<DefaultNodeItem>()->update((*update).second->info());
				updateRange.add(index);

				update = updates.erase(update);
			}
			else
				++update;
		}
		else
			if (updates.isRemoved(update))
			{
				if ((index = m_items.indexOf((*update).first.as<DefaultNodeItem>()->info()->fileName())) != Container::InvalidIndex)
				{
					if (!m_items[index].as<DefaultNodeItem>()->isLocked())
						removeEntry(index);
				}

				update = updates.erase(update);
			}
			else
				update = updates.erase(update);

	updateColumns(updateRange, columnCount(QModelIndex()) - 1);

	if (!updates.isEmpty())
	{
		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + updates.size() - 1);
		for (Snapshot::iterator update = updates.begin(), end = updates.end(); update != end; update = updates.erase(update))
			m_items.add(NodeItem::Holder(new DefaultNodeItem((*update).second->info())));
		endInsertRows();
	}
}

void FolderNodeBase::scanForSizeEvent(bool canceled, Snapshot &snapshot)
{
	if (canceled)
	{
		ScanForSizeEventFunctor_canceled functor;
		processScanEventSnapshot(snapshot, functor);
		updateBothColumns(functor.updateRange);
	}
	else
	{
		ScanForSizeEventFunctor functor;
		processScanEventSnapshot(snapshot, functor);
		updateBothColumns(functor.updateRange);
	}
}

bool FolderNodeBase::scanForCopyEvent(bool canceled, Snapshot &snapshot, ICopyControl *control, bool move)
{
	if (canceled)
	{
		ScanForCopyEventFunctor_canceled functor;
		processScanEventSnapshot(snapshot, functor);

		control->canceled();
		updateBothColumns(functor.updateRange);
	}
	else
	{
		ScanForCopyEventFunctor functor(move ? tr("Moving...") : tr("Copying..."));
		processScanEventSnapshot(snapshot, functor);

		if (control->start(snapshot, move))
		{
			updateBothColumns(functor.updateRange);
			return true;
		}
		else
			control->done(false);

		for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
			(*i).first.as<DefaultNodeItem>()->unlock();

		updateBothColumns(functor.updateRange);
	}

	return false;
}

bool FolderNodeBase::scanForRemoveEvent(bool canceled, Snapshot &snapshot)
{
	if (canceled)
	{
		ScanForRemoveEventFunctor_canceled functor;
		processScanEventSnapshot(snapshot, functor);
		updateBothColumns(functor.updateRange);
	}
	else
	{
		ScanForRemoveEventFunctor functor(tr("Removing..."));
		processScanEventSnapshot(snapshot, functor);

		if (QMessageBox::question(
				Application::mainWindow(),
				tr("Remove..."),
				tr("Would you like to remove").
					append(QString::fromLatin1("\n\t")).
					append(tr("directories:")).append(QString::fromLatin1("\n\t\t")).
					append(functor.folders.join(QString::fromLatin1("\n\t\t"))).
					append(QString::fromLatin1("\n\t")).
					append(tr("files:")).append(QString::fromLatin1("\n\t\t")).
					append(functor.files.join(QString::fromLatin1("\n\t\t"))).
					append(QString::fromLatin1("\n")).
					append(tr("it will free ").append(Tools::humanReadableSize(snapshot.totalSize()))),
				QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			updateSecondColumn(functor.updateRange);
			return true;
		}
		else
			for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
				(*i).first.as<DefaultNodeItem>()->unlock();

		updateBothColumns(functor.updateRange);
	}

	return false;
}

bool FolderNodeBase::performCopyEvent(bool canceled, Snapshot &snapshot, bool move)
{
	if (!canceled && move)
	{
		PerformCopyEventFunctor functor(tr("Removing..."));
		processPerformEventSnapshot(snapshot, functor);
		updateSecondColumn(functor.updateRange);

		return true;
	}
	else
	{
		PerformCopyEventFunctor_canceled functor;
		processPerformEventSnapshot(snapshot, functor);
		updateBothColumns(functor.updateRange);
	}

	return false;
}

void FolderNodeBase::performRemoveEvent(bool canceled, Snapshot &snapshot)
{
	Union updateRange;
	PerformRemoveEventFunctor functor(this, &FolderNodeBase::removeEntry);

	processPerformEventSnapshot(snapshot, functor);

	for (QStringList::size_type i = 0, size = functor.list.size(); i < size; ++i)
		updateRange.add(m_items.indexOf(functor.list.at(i)));

	updateBothColumns(updateRange);
}

void FolderNodeBase::updateProgressEvent(const NodeItem::Holder &item, quint64 progress, quint64 timeElapsed)
{
	item.as<DefaultNodeItem>()->updateProgress(progress, timeElapsed);
	updateSecondColumn(m_items.indexOf(item.as<DefaultNodeItem>()->info()->fileName()), item.as<DefaultNodeItem>());
}

void FolderNodeBase::completedProgressEvent(const NodeItem::Holder &item, quint64 timeElapsed)
{
	item.as<DefaultNodeItem>()->updateProgress(item.as<DefaultNodeItem>()->total(), timeElapsed);
	updateSecondColumn(m_items.indexOf(item.as<DefaultNodeItem>()->info()->fileName()), item.as<DefaultNodeItem>());
}

void FolderNodeBase::performActionEvent(const AsyncFileAction::FilesList &files, const QString &error)
{
	Union update;
	NodeItem::Holder item;

	for (FileAction::FilesList::size_type i = 0, size = files.size(); i < size; ++i)
	{
		item = files.at(i).first;

		item->unlock();
		update.add(m_items.indexOf(item.data()));
	}

	if (!error.isEmpty())
		QMessageBox::critical(Application::mainWindow(), tr("Error..."), error);

	updateFirstColumn(update);
}

Node *FolderNodeBase::createNode(const IFileInfo *file) const
{
	return Application::rootNode()->open(m_container.data(), file, const_cast<FolderNodeBase *>(this));
}

void FolderNodeBase::updateFiles()
{
	if (isVisible())
	{
		PScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::UpdateFiles, this, updateFilesList()));
		setUpdating(true);
		handleTask(task.take());
	}
}

void FolderNodeBase::scanForSize(const Snapshot &snapshot)
{
	PScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::ScanFilesForSize, this, snapshot));
	addTask(task.take(), snapshot);
}

void FolderNodeBase::scanForCopy(const Snapshot &snapshot, ICopyControl::Holder &destination, bool move)
{
	PScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::ScanFilesForCopy, this, destination, snapshot, move));
	addTask(task.data(), const_cast<const ScanFilesTask *>(task.data())->destination().data(), snapshot);
	task.take();
}

void FolderNodeBase::scanForRemove(const Snapshot &snapshot)
{
	PScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::ScanFilesForRemove, this, snapshot));
	addTask(task.take(), snapshot);
}

void FolderNodeBase::performCopy(BaseTask *oldTask, const Snapshot &snapshot, ICopyControl::Holder &destination, bool move)
{
	if (destination->isPhysical() && move)
	{
		PScopedPointer<PerformMoveTask> task(new PerformMoveTask(this, destination, snapshot));
		resetTask(task.data(), oldTask);
		task.take();
	}
	else
	{
		PScopedPointer<PerformCopyTask> task(new PerformCopyTask(this, destination, snapshot, move));
		resetTask(task.data(), oldTask);
		task.take();
	}
}

void FolderNodeBase::performRemove(BaseTask *oldTask, const Snapshot &snapshot)
{
	PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, snapshot));
	resetTask(task.take(), oldTask);
}

void FolderNodeBase::performRemove(BaseTask *oldTask, const ICopyControl *destination, const Snapshot &snapshot)
{
	PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, snapshot));
	resetTask(task.take(), oldTask, destination);
}

void FolderNodeBase::updateFiles(const BaseTask::Event *e)
{
	typedef ScanFilesTask::UpdatesEvent * NotConstEvent;
	NotConstEvent event = static_cast<NotConstEvent>(const_cast<BaseTask::Event *>(e));

	if (!event->canceled)
		updateFilesEvent(event->snapshot);

	if (event->isLastEvent)
	{
		setUpdating(false);
		taskHandled(event->task);
	}
}

void FolderNodeBase::scanForSize(const BaseTask::Event *e)
{
	typedef ScanFilesTask::Event * NotConstEvent;
	typedef const ScanFilesTask::Event * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	scanForSizeEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

void FolderNodeBase::scanForCopy(const BaseTask::Event *e)
{
	typedef ScanFilesTask::CopyEvent * NotConstEvent;
	typedef const ScanFilesTask::CopyEvent * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	if (scanForCopyEvent(event->canceled, event->snapshot, event->destination.data(), event->move))
		performCopy(event->task, event->snapshot, const_cast<NotConstEvent>(event)->destination, event->move);
	else
		removeAllTaskLinks(event->task, event->destination.data());
}

void FolderNodeBase::scanForRemove(const BaseTask::Event *e)
{
	typedef ScanFilesTask::Event * NotConstEvent;
	typedef const ScanFilesTask::Event * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	if (scanForRemoveEvent(event->canceled, event->snapshot))
		performRemove(event->task, event->snapshot);
	else
		removeAllTaskLinks(event->task);
}

void FolderNodeBase::performCopy(const BaseTask::Event *e)
{
	typedef PerformCopyTask::ExtendedEvent * NotConstEvent;
	typedef const PerformCopyTask::ExtendedEvent * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	event->destination->node()->refresh();

	if (performCopyEvent(event->canceled, event->snapshot, event->move))
		performRemove(event->task, event->destination.data(), event->snapshot);
	else
		removeAllTaskLinks(event->task, event->destination.data());
}

void FolderNodeBase::performRemove(const BaseTask::Event *e)
{
	typedef PerformRemoveTask::Event * NotConstEvent;
	typedef const PerformRemoveTask::Event * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	performRemoveEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

FolderNodeBase::Container::Container()
{}

FolderNodeBase::Container::size_type FolderNodeBase::Container::size() const
{
	return m_container.size();
}

FolderNodeBase::Container::Item *FolderNodeBase::Container::at(size_type index) const
{
	return m_container.at(index).data();
}

FolderNodeBase::Container::size_type FolderNodeBase::Container::indexOf(Item *item) const
{
	for (List::size_type i = 0, size = m_container.size(); i < size; ++i)
		if (static_cast<Item *>(m_container.at(i).data()) == item)
			return i;

	return InvalidIndex;
}

void FolderNodeBase::CancelFunctor::call(Container::size_type index, DefaultNodeItem *item)
{
	m_items = m_node->cancelTaskAndTakeItems(NodeItem::Holder(item));
	m_union.add(index);

	for (TasksMap::List::size_type i = 0, size = m_items.size(); i < size; ++i)
		m_items.at(i).as<TasksNodeItem>()->cancel(m_reason);
}

void FolderNodeBase::RenameFunctor::call(Container::size_type index, DefaultNodeItem *item)
{
	StringDialog dialog(
			item->info()->isDir() ?
					tr("Enter new name for directory \"%1\"").arg(item->info()->fileName()) :
					tr("Enter new name for file \"%1\"").arg(item->info()->fileName()),
			tr("Name"),
			item->info()->fileName(),
			Application::mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{
//		QString error;
//
//		if (entry->info().rename(dialog.value(), error))
//		{
//			Info info(m_info->absoluteFilePath(dialog.value()));
//
//			m_items.replace(index, entry->info().fileName(), info.fileName());
//			static_cast<FileSystemEntryItem*>(entry)->setInfo(info);
//
//			if (static_cast<FileSystemEntryItem*>(entry)->node())
//			{
////				static_cast<FileSystemEntryItem*>(entry)->node()->viewCloseAll();
//				static_cast<FileSystemEntryItem*>(entry)->setNode(0);
//			}
//		}
//		else
//			QMessageBox::critical(Application::mainWindow(),
//						entry->info().isDir() ?
//							tr("Failed to rename directory \"%1\"").arg(entry->info().fileName()) :
//							tr("Failed to rename file \"%1\"").arg(entry->info().fileName()),
//						error);
	}
}

void FolderNodeBase::processIndexList(const QModelIndexList &list, Functor &functor)
{
	QModelIndex index;
	DefaultNodeItem *entry;
	QSet<DefaultNodeItem *> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<DefaultNodeItem *>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && !static_cast<DefaultNodeItem *>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void FolderNodeBase::processLockedIndexList(const QModelIndexList &list, Functor &functor)
{
	QModelIndex index;
	ProcessedList res;
	DefaultNodeItem *entry;
	QSet<DefaultNodeItem *> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<DefaultNodeItem *>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && static_cast<DefaultNodeItem *>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void FolderNodeBase::scanForRemove(const ProcessedList &entries)
{
	Union updateRange;
	DefaultNodeItem *entry;
	Container::size_type index;
	Snapshot::Files files(m_container.data());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
	{
		index = entries.at(i).first;

		if ((entry = entries.at(i).second)->info()->isDir())
			entry->lock(tr("Scanning folder for remove..."));
		else
			entry->lock(tr("Removing..."));

		updateRange.add(index);
		files.add(entry->info()->fileName(), NodeItem::Holder(entry));
	}

	updateFirstColumn(updateRange);
	scanForRemove(files);
}

void FolderNodeBase::scanForSize(const ProcessedList &entries)
{
	Union updateRange;
	DefaultNodeItem *entry;
	Container::size_type index;
	Snapshot::Files files(m_container.data());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		if ((entry = static_cast<DefaultNodeItem*>(entries.at(i).second))->info()->isDir())
		{
			index = entries.at(i).first;
			entry->lock(tr("Scanning folder for size..."));
			updateRange.add(index);
			files.add(entry->info()->fileName(), NodeItem::Holder(entry));
		}

	updateFirstColumn(updateRange);
	scanForSize(files);
}

void FolderNodeBase::scanForCopy(const ProcessedList &entries, INodeView *destination, bool move)
{
	ICopyControl::Holder control(destination->node()->createControl(destination));

	if (control)
	{
		Union updateRange;
		DefaultNodeItem *entry;
		Container::size_type index;
		Snapshot::Files files(m_container.data());
		QString fileLockReason = move ? tr("Moving...") : tr("Copying...");
		QString folderLockReason = move ? tr("Scanning folder for move...") : tr("Scanning folder for copy...");

		for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		{
			index = entries.at(i).first;

			if ((entry = static_cast<DefaultNodeItem*>(entries.at(i).second))->info()->isDir())
				entry->lock(folderLockReason);
			else
				entry->lock(fileLockReason);

			updateRange.add(index);
			files.add(entry->info()->fileName(), NodeItem::Holder(entry));
		}

		updateFirstColumn(updateRange);
		scanForCopy(files, control, move);
	}
}

QModelIndex FolderNodeBase::index(int column, DefaultNodeItem *item) const
{
	Container::size_type index = m_items.indexOf(item);

	if (index != Container::InvalidIndex)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

QModelIndex FolderNodeBase::indexForFile(DefaultNodeItem *item) const
{
	Q_ASSERT(m_items.indexOf(item) != Container::InvalidIndex);
	Container::size_type index = m_items.indexOf(item);
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

QModelIndex FolderNodeBase::indexForFile(DefaultNodeItem *item, Container::size_type index) const
{
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

void FolderNodeBase::updateFirstColumn(DefaultNodeItem *entry)
{
	updateFirstColumn(m_items.indexOf(entry), entry);
}

void FolderNodeBase::updateFirstColumn(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), 0, m_items[range.at(i).bottom()].data()));
}

void FolderNodeBase::updateFirstColumn(Container::size_type index, DefaultNodeItem *entry)
{
	QModelIndex idx = createIndex(index, 0, entry);
	emit dataChanged(idx, idx);
}

void FolderNodeBase::updateSecondColumn(DefaultNodeItem *entry)
{
	updateSecondColumn(m_items.indexOf(entry), entry);
}

void FolderNodeBase::updateSecondColumn(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 1, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()].data()));
}

void FolderNodeBase::updateSecondColumn(Container::size_type index, DefaultNodeItem *entry)
{
	QModelIndex idx = createIndex(index, 1, entry);
	emit dataChanged(idx, idx);
}

void FolderNodeBase::updateBothColumns(DefaultNodeItem *entry)
{
	updateBothColumns(m_items.indexOf(entry), entry);
}

void FolderNodeBase::updateBothColumns(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()].data()));
}

void FolderNodeBase::updateBothColumns(Container::size_type index, DefaultNodeItem *entry)
{
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, 1, entry));
}

void FolderNodeBase::updateColumns(const Union &range, int lastColumn)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), lastColumn, m_items[range.at(i).bottom()].data()));
}

void FolderNodeBase::removeEntry(Container::size_type index)
{
	beginRemoveRows(QModelIndex(), index, index);
	m_items.remove(index);
	endRemoveRows();
}

void FolderNodeBase::removeEntry(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	m_items.remove(index.row());
	endRemoveRows();
}

DEFAULT_PLUGIN_NS_END
