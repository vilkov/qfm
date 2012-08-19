#include "defaultbasenode.h"
#include "defaultbasenode_p.h"
#include "../search/defaultsearchnode.h"
#include "../../search/dialog/defaultsearchdialog.h"
#include "../../tasks/scan/defaultscanfilestask.h"
#include "../../tasks/perform/defaultperformcopytask.h"
#include "../../tasks/perform/defaultperformmovetask.h"
#include "../../tasks/perform/defaultperformremovetask.h"
#include "../../actions/defaultcopyaction.h"
#include "../../actions/defaultcutaction.h"
#include "../../actions/defaultpasteaction.h"
#include "../../actions/defaultpasteintofolderaction.h"
#include "../../actions/defaultpropertiesaction.h"
#include "../../actions/defaultpasteclipboardaction.h"
#include "../../actions/defaultopenwithaction.h"
#include "../../../../tools/widgets/stringdialog/stringdialog.h"
#include "../../../../application.h"

#include <vfs/filters/vfs_filters.h>
#include <vfs/tasks/vfs_performactiontask.h>
#include <vfs/tools/vfs_commontools.h>

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
			copyAction = new CopyAction();
			cutAction = new CutAction();
			pasteAction = new PasteAction();
			pasteIntoFolderAction = new PasteIntoFolderAction();
			propertiesAction = new PropertiesAction();
			pasteClipboardAction = new PasteClipboardAction();
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

	CopyAction *copyAction;
	CutAction *cutAction;
	PasteAction *pasteAction;
	PasteIntoFolderAction *pasteIntoFolderAction;
	PropertiesAction *propertiesAction;
	PasteClipboardAction *pasteClipboardAction;

private:
	int m_ref;
};

static GlobalActions globalActions;


BaseNode::BaseNode(IFileContainer::Holder &container, Node *parent) :
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

BaseNode::~BaseNode()
{
	globalActions.release();
}

int BaseNode::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant BaseNode::headerData(int section, Qt::Orientation orientation, int role) const
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

bool BaseNode::event(QEvent *e)
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
			return TasksNode::event(e);
	}
}

void BaseNode::refresh()
{
	if (!isUpdating())
		updateFiles();
}

QString BaseNode::title() const
{
	QString res = m_container->location();
	return res.mid(res.lastIndexOf(QChar(L'/')) + 1);
}

QString BaseNode::location() const
{
	return m_container->location();
}

QAbstractItemModel *BaseNode::model() const
{
	return const_cast<ProxyModel *>(&m_proxy);
}

QAbstractItemDelegate *BaseNode::delegate() const
{
	return const_cast<Delegate *>(&m_delegate);
}

const INodeView::MenuActionList &BaseNode::actions() const
{
	return m_menuActions;
}

::History::Entry *BaseNode::menuAction(QAction *action, INodeView *view)
{
	return NULL;
}

ICopyControl *BaseNode::createControl(INodeView *view) const
{
	return m_container->createControl(view);
}

void BaseNode::contextMenu(const QModelIndexList &list, INodeView *view)
{
	typedef QSet<NodeItem::Holder>                           ItemsSet;
	typedef QList<NodeItem::Holder>                          ItemsList;
	typedef QMap<const FileAction *, FileAction::FilesList>  ActionsMap;
	typedef QMap<NodeItem::Holder, ::Tools::Containers::Dot> ItemsIndexMap;
	typedef ::Desktop::ContextMenuFactory::FileActionsList   FileActionsList;
	typedef QMap<const IApplication *, FileAction *>         OpenWithActionsMap;

	QMenu menu;
	QMenu openWithMenu(tr("Open with"));
	ItemsSet set;
	ActionsMap mapGlobalActions;
	ActionsMap mapOpenWithActions;
	ItemsList items;
	QModelIndex index;
	ItemsIndexMap itemsIndex;
	NodeItem::Holder item;
	FileActionsList actions;
	OpenWithActionsMap openWithActions;
	IApplications::LinkedList applications;

	for (ItemsList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!(item = m_items[(index = m_proxy.mapToSource(list.at(i))).row()]).as<NodeItem>()->isRootItem() && !set.contains(item))
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
			if ((item = items.at(0)).as<NodeItem>()->info()->isDir())
			{
				menu.addAction(const_cast<QAction*>(globalActions.pasteIntoFolderAction->action()));

				actions = Application::globalMenu()->actions(::Desktop::ContextMenuFactory::SingleFolder);

				for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
					mapGlobalActions[actions.at(i)].push_back(FileAction::FilesList::value_type(item, item.as<NodeItem>()->info()));
			}
			else
			{
				menu.addAction(const_cast<QAction*>(globalActions.pasteAction->action()));

				actions = Application::globalMenu()->actions(::Desktop::ContextMenuFactory::SingleFile);

				for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
					mapGlobalActions[actions.at(i)].push_back(FileAction::FilesList::value_type(item, item.as<NodeItem>()->info()));
			}
		}
		else
		{
			menu.addAction(const_cast<QAction*>(globalActions.pasteAction->action()));

			actions = Application::globalMenu()->actions(::Desktop::ContextMenuFactory::MultipleFilesOrFolders);

			for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
			{
				FileAction::FilesList &files = mapGlobalActions[actions.at(i)];

				for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
				{
					item = items.at(i);
					files.push_back(FileAction::FilesList::value_type(item, item.as<NodeItem>()->info()));
				}
			}
		}

		for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
		{
			item = items.at(i);
			applications = m_container->applications()->user(item.as<NodeItem>()->info()->fileType());

			if (applications.isEmpty())
				applications = m_container->applications()->system(item.as<NodeItem>()->info()->fileType());

			for (IApplications::LinkedList::const_iterator i = applications.begin(), end = applications.end(); i != end; ++i)
				if (FileAction *&action = openWithActions[*i])
					mapOpenWithActions[action].push_back(FileAction::FilesList::value_type(item, item.as<NodeItem>()->info()));
				else
				{
					action = new OpenWithAction(*i);
					mapOpenWithActions[action].push_back(FileAction::FilesList::value_type(item, item.as<NodeItem>()->info()));
				}
		}

		if (!mapOpenWithActions.isEmpty())
		{
			menu.addSeparator();

			for (ActionsMap::const_iterator it = mapOpenWithActions.begin(), end = mapOpenWithActions.end(); it != end; ++it)
				openWithMenu.addAction(const_cast<QAction *>(it.key()->action()));

			menu.addMenu(&openWithMenu);
			menu.addSeparator();
		}

		for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
		{
			actions = Application::globalMenu()->actions((item = items.at(i)).as<NodeItem>()->info()->fileType()->id());

			for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
				mapGlobalActions[actions.at(i)].push_back(FileAction::FilesList::value_type(item, item.as<NodeItem>()->info()));
		}
	}

	actions = Application::globalMenu()->actions(::Desktop::ContextMenuFactory::AnyFilesOrFolders);

	for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
	{
		FileAction::FilesList &files = mapGlobalActions[actions.at(i)];

		for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
		{
			item = items.at(i);
			files.push_back(FileAction::FilesList::value_type(item, item.as<NodeItem>()->info()));
		}
	}

	menu.addSeparator();

	if (!mapGlobalActions.isEmpty())
	{
		for (ActionsMap::const_iterator it = mapGlobalActions.begin(), end = mapGlobalActions.end(); it != end; ++it)
			menu.addAction(const_cast<QAction *>(it.key()->action()));

		menu.addSeparator();
	}

	menu.addAction(const_cast<QAction*>(globalActions.propertiesAction->action()));

	if (FileAction *action = FileAction::fromQAction(menu.exec(QCursor::pos())))
	{
		FileAction::FilesList files = mapGlobalActions.value(action);

		if (files.isEmpty())
			for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
			{
				item = items.at(i);
				files.push_back(FileAction::FilesList::value_type(item, item.as<NodeItem>()->info()));
			}

		if (action->isAsynchronous())
		{
			PScopedPointer<PerformActionTask> task;

			if (task = static_cast<AsyncFileAction *>(action)->process(this, m_container, files))
			{
				Union update;
				Snapshot::Files list(m_container);

				for (FileAction::FilesList::size_type i = 0, size = files.size(); i < size; ++i)
				{
					item = files.at(i).first;

					item->lock(static_cast<AsyncFileAction *>(action)->lockReason());
					update.add(itemsIndex.value(item));
					list.add(item.as<NodeItem>()->info()->fileName(), item);
				}

				addTask(task.take(), list);
				updateFirstColumn(update);
			}
		}
		else
			static_cast<SyncFileAction *>(action)->process(m_container, files);
	}

	qDeleteAll(openWithActions);
	openWithMenu.clear();
	menu.clear();
}

void BaseNode::createFile(const QModelIndex &index, INodeView *view)
{
	QString name;

	if (index.isValid())
	{
		NodeItem *item = m_items[m_proxy.mapToSource(index).row()].as<NodeItem>();

		if (!item->isRootItem())
			name = item->info()->fileName().as<QString>();
	}

	StringDialog dialog(tr("Enter name for new file"), tr("Name"), name, Application::mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{

	}
}

void BaseNode::createDirectory(const QModelIndex &index, INodeView *view)
{
	QString name;

	if (index.isValid())
	{
		NodeItem *item = m_items[m_proxy.mapToSource(index).row()].as<NodeItem>();

		if (!item->isRootItem())
			name = item->info()->fileName().as<QString>();
	}

	StringDialog dialog(tr("Enter name for new directory"), tr("Name"), name, Application::mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{
		QString error;
		IFileContainer::Holder folder(m_container->create(dialog.value(), error));

		if (folder)
		{
			NodeItem *item;
			IFileInfo::Holder info(m_container->info(dialog.value(), error));

			beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
			m_items.add(NodeItem::Holder(item = new NodeItem(info)));
			endInsertRows();

			view->select(indexForFile(item, m_items.size() - 1));
		}
		else
			QMessageBox::critical(Application::mainWindow(), tr("Failed to create directory..."), error);
	}
}

void BaseNode::rename(const QModelIndex &index, INodeView *view)
{
	NodeItem *entry = static_cast<NodeItem *>(m_proxy.mapToSource(index).internalPointer());

	if (!entry->isRootItem() && !static_cast<NodeItem *>(entry)->isLocked())
		renameFunctor(index.row(), entry);
}

void BaseNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void BaseNode::remove(const QModelIndexList &list, INodeView *view)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForRemove(entries);
}

void BaseNode::cancel(const QModelIndexList &list, INodeView *view)
{
	CancelFunctor cancelFunctor(this, tr("Canceling..."));
	processLockedIndexList(list, cancelFunctor);
	updateFirstColumn(cancelFunctor.updateUnion());
}

void BaseNode::calculateSize(const QModelIndexList &list, INodeView *view)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForSize(entries);
}

void BaseNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{
	AbsoluteFilePathList pathList(m_container.data());
	processIndexList(list, pathList);
	Application::instance()->clipboard()->setText(pathList.join(QChar(L'\r')));
}

void BaseNode::copy(const INodeView *source, INodeView *destination)
{
	ProcessedList entries;
	processIndexList(source->selectedIndexes(), entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, false);
}

void BaseNode::move(const INodeView *source, INodeView *destination)
{
	ProcessedList entries;
	processIndexList(source->selectedIndexes(), entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, true);
}

void BaseNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

::History::Entry *BaseNode::search(const QModelIndex &index, INodeView *view)
{
	SearchDialog dialog(Application::mainWindow());

	if (dialog.exec() == SearchDialog::Accepted)
	{
		IFileContainer::Holder container(m_container->open());
		FileNameFilter::Holder filter(new FileNameFilter(dialog.pattern()));
		return switchTo(new SearchNode(container, filter, this), view);
	}

	return NULL;
}

QModelIndex BaseNode::rootIndex() const
{
	return QModelIndex();
}

Node *BaseNode::viewChild(const QModelIndex &idx, QModelIndex &selected, bool newTab)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<NodeItem *>(index.internalPointer())->isRootItem())
		return parentNode();
	else
		if (!static_cast<NodeItem *>(index.internalPointer())->isLocked())
		{
			NodeItem *entry = static_cast<NodeItem *>(index.internalPointer());

			if (entry->node())
				return entry->node();
			else
				if (Node *node = createNode(entry->info()))
				{
					entry->setNode(node);
					return node;
				}
				else
					if (!newTab && entry->info()->isFile())
						Application::open(m_container, entry->info());
		}

	return NULL;
}

Node *BaseNode::viewChild(const QString &fileName, QModelIndex &selected)
{
	Container::size_type index = m_items.indexOf(fileName);

	if (index == Container::InvalidIndex)
	{
		QString error;
		IFileInfo::Holder info;

		if (info = m_container->info(fileName, error))
			if (Node *node = createNode(info))
			{
				beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
				m_items.add(NodeItem::Holder(new NodeItem(info, node)));
				endInsertRows();

				return node;
			}
			else
			{
				if (info->isFile())
				{
					NodeItem *item;

					beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
					m_items.add(NodeItem::Holder(item = new NodeItem(info)));
					endInsertRows();

					selected = indexForFile(item, m_items.lastIndex());
				}

				return this;
			}
	}
	else
	{
		NodeItem *item = m_items[index].as<NodeItem>();

		if (item->node())
			return item->node();
		else
			if (Node *node = createNode(item->info()))
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

void BaseNode::nodeRemoved(Node *node)
{
	Container::size_type index = m_items.indexOf(node);

	if (index != Container::InvalidIndex)
		m_items[index].as<NodeItem>()->setNode(NULL);
}

void BaseNode::cleanup(Snapshot &snapshot)
{
	Container::size_type index;

	for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
		if (snapshot.isRemoved(i))
		{
			if ((index = m_items.indexOf((*i).first.as<NodeItem>()->info()->fileName())) != Container::InvalidIndex)
				removeEntry(index);

			i = snapshot.erase(i);
		}
		else
			++i;
}

void BaseNode::processScanEventSnapshot(Snapshot &snapshot, EventFunctor &functor)
{
	NodeItem *entry;
	Container::size_type index;

	cleanup(snapshot);

	for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
		if ((index = m_items.indexOf((entry = (*i).first.as<NodeItem>())->info()->fileName())) != Container::InvalidIndex)
		{
			functor(index, entry, (*i).second);
			++i;
		}
		else
			i = snapshot.erase(i);
}

void BaseNode::processPerformEventSnapshot(Snapshot &snapshot, EventFunctor &functor)
{
	NodeItem *entry;
	Container::size_type index;

	for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
		if ((index = m_items.indexOf((entry = (*i).first.as<NodeItem>())->info()->fileName())) != Container::InvalidIndex)
		{
			functor(index, entry, (*i).second);
			++i;
		}
		else
			i = snapshot.erase(i);
}

Snapshot BaseNode::updateFilesList() const
{
	Snapshot::Files files(m_container.data());

	for (Container::size_type i = 0, size = m_items.size(); i < size; ++i)
		files.add(m_items[i].as<NodeItem>()->info()->fileName(), m_items[i]);

	return files;
}

void BaseNode::updateFilesEvent(Snapshot &updates)
{
	Union updateRange;
	Container::size_type index;

	for (Snapshot::iterator update = updates.begin(), end = updates.end(); update != end;)
		if (updates.isAdded(update) || updates.isUpdated(update))
		{
			if ((index = m_items.indexOf((*update).second->info()->fileName())) != Container::InvalidIndex)
			{
				m_items[index].as<NodeItem>()->update((*update).second->info());
				updateRange.add(index);

				update = updates.erase(update);
			}
			else
				++update;
		}
		else
			if (updates.isRemoved(update))
			{
				if ((index = m_items.indexOf((*update).first.as<NodeItem>()->info()->fileName())) != Container::InvalidIndex)
				{
					if (!m_items[index].as<NodeItem>()->isLocked())
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
			m_items.add(NodeItem::Holder(new NodeItem((*update).second->info())));
		endInsertRows();
	}
}

void BaseNode::scanForSizeEvent(bool canceled, Snapshot &snapshot)
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

bool BaseNode::scanForCopyEvent(bool canceled, Snapshot &snapshot, ICopyControl *control, bool move)
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
			(*i).first.as<NodeItem>()->unlock();

		updateBothColumns(functor.updateRange);
	}

	return false;
}

bool BaseNode::scanForRemoveEvent(bool canceled, Snapshot &snapshot)
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
				(*i).first.as<NodeItem>()->unlock();

		updateBothColumns(functor.updateRange);
	}

	return false;
}

bool BaseNode::performCopyEvent(bool canceled, Snapshot &snapshot, bool move)
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

void BaseNode::performRemoveEvent(bool canceled, Snapshot &snapshot)
{
	Union updateRange;
	PerformRemoveEventFunctor functor(this, &BaseNode::removeEntry);

	processPerformEventSnapshot(snapshot, functor);

	for (QStringList::size_type i = 0, size = functor.list.size(); i < size; ++i)
		updateRange.add(m_items.indexOf(functor.list.at(i)));

	updateBothColumns(updateRange);
}

void BaseNode::updateProgressEvent(const NodeItem::Holder &item, quint64 progress, quint64 timeElapsed)
{
	item.as<NodeItem>()->updateProgress(progress, timeElapsed);
	updateSecondColumn(m_items.indexOf(item.as<NodeItem>()->info()->fileName()), item.as<NodeItem>());
}

void BaseNode::completedProgressEvent(const NodeItem::Holder &item, quint64 timeElapsed)
{
	item.as<NodeItem>()->updateProgress(item.as<NodeItem>()->total(), timeElapsed);
	updateSecondColumn(m_items.indexOf(item.as<NodeItem>()->info()->fileName()), item.as<NodeItem>());
}

void BaseNode::performActionEvent(const AsyncFileAction::FilesList &files, const QString &error)
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

Node *BaseNode::createNode(const IFileInfo *file) const
{
	return Application::rootNode()->open(m_container.data(), file, const_cast<BaseNode *>(this));
}

void BaseNode::updateFiles()
{
	if (isVisible())
	{
		PScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::UpdateFiles, this, updateFilesList()));
		setUpdating(true);
		handleTask(task.take());
	}
}

void BaseNode::scanForSize(const Snapshot &snapshot)
{
	PScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::ScanFilesForSize, this, snapshot));
	addTask(task.take(), snapshot);
}

void BaseNode::scanForCopy(const Snapshot &snapshot, ICopyControl::Holder &destination, bool move)
{
	PScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::ScanFilesForCopy, this, destination, snapshot, move));
	addTask(task.data(), const_cast<const ScanFilesTask *>(task.data())->destination().data(), snapshot);
	task.take();
}

void BaseNode::scanForRemove(const Snapshot &snapshot)
{
	PScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::ScanFilesForRemove, this, snapshot));
	addTask(task.take(), snapshot);
}

void BaseNode::performCopy(BaseTask *oldTask, const Snapshot &snapshot, ICopyControl::Holder &destination, bool move)
{
	if (destination->isDefault() && move)
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

void BaseNode::performRemove(BaseTask *oldTask, const Snapshot &snapshot)
{
	PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, snapshot));
	resetTask(task.take(), oldTask);
}

void BaseNode::performRemove(BaseTask *oldTask, const ICopyControl *destination, const Snapshot &snapshot)
{
	PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, snapshot));
	resetTask(task.take(), oldTask, destination);
}

void BaseNode::updateFiles(const BaseTask::Event *e)
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

void BaseNode::scanForSize(const BaseTask::Event *e)
{
	typedef ScanFilesTask::Event * NotConstEvent;
	typedef const ScanFilesTask::Event * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	scanForSizeEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

void BaseNode::scanForCopy(const BaseTask::Event *e)
{
	typedef ScanFilesTask::CopyEvent * NotConstEvent;
	typedef const ScanFilesTask::CopyEvent * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	if (scanForCopyEvent(event->canceled, event->snapshot, event->destination.data(), event->move))
		performCopy(event->task, event->snapshot, const_cast<NotConstEvent>(event)->destination, event->move);
	else
		removeAllTaskLinks(event->task, event->destination.data());
}

void BaseNode::scanForRemove(const BaseTask::Event *e)
{
	typedef ScanFilesTask::Event * NotConstEvent;
	typedef const ScanFilesTask::Event * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	if (scanForRemoveEvent(event->canceled, event->snapshot))
		performRemove(event->task, event->snapshot);
	else
		removeAllTaskLinks(event->task);
}

void BaseNode::performCopy(const BaseTask::Event *e)
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

void BaseNode::performRemove(const BaseTask::Event *e)
{
	typedef PerformRemoveTask::Event * NotConstEvent;
	typedef const PerformRemoveTask::Event * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	performRemoveEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

BaseNode::Container::Container()
{}

BaseNode::Container::size_type BaseNode::Container::size() const
{
	return m_container.size();
}

BaseNode::Container::Item *BaseNode::Container::at(size_type index) const
{
	return m_container.at(index).data();
}

BaseNode::Container::size_type BaseNode::Container::indexOf(Item *item) const
{
	for (List::size_type i = 0, size = m_container.size(); i < size; ++i)
		if (static_cast<Item *>(m_container.at(i).data()) == item)
			return i;

	return InvalidIndex;
}

void BaseNode::CancelFunctor::call(Container::size_type index, NodeItem *item)
{
	m_items = m_node->cancelTaskAndTakeItems(NodeItem::Holder(item));

	if (!m_items.isEmpty())
	{
		m_union.add(index);

		for (TasksMap::List::size_type i = 0, size = m_items.size(); i < size; ++i)
			m_items.at(i).as<TasksNodeItem>()->cancel(m_reason);
	}
}

void BaseNode::renameFunctor(Container::size_type index, NodeItem *item)
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
		QString error;

		if (m_container->rename(item->info(), dialog.value(), error))
		{
			IFileInfo::Holder info(m_container->info(dialog.value(), error));

			if (info)
			{
				item->update(info);
				updateColumns(index, item, columnsCount());
			}
			else
				QMessageBox::critical(Application::mainWindow(),
							item->info()->isDir() ?
								tr("Failed to rename directory \"%1\"").arg(item->info()->fileName()) :
								tr("Failed to rename file \"%1\"").arg(item->info()->fileName()),
							error);
		}
		else
			QMessageBox::critical(Application::mainWindow(),
						item->info()->isDir() ?
							tr("Failed to rename directory \"%1\"").arg(item->info()->fileName()) :
							tr("Failed to rename file \"%1\"").arg(item->info()->fileName()),
						error);
	}
}

void BaseNode::processIndexList(const QModelIndexList &list, Functor &functor)
{
	QModelIndex index;
	NodeItem *entry;
	QSet<NodeItem *> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<NodeItem *>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && !static_cast<NodeItem *>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void BaseNode::processLockedIndexList(const QModelIndexList &list, Functor &functor)
{
	QModelIndex index;
	ProcessedList res;
	NodeItem *entry;
	QSet<NodeItem *> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<NodeItem *>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && static_cast<NodeItem *>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void BaseNode::scanForRemove(const ProcessedList &entries)
{
	Union updateRange;
	NodeItem *entry;
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

void BaseNode::scanForSize(const ProcessedList &entries)
{
	Union updateRange;
	NodeItem *entry;
	Container::size_type index;
	Snapshot::Files files(m_container.data());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		if ((entry = static_cast<NodeItem*>(entries.at(i).second))->info()->isDir())
		{
			index = entries.at(i).first;
			entry->lock(tr("Scanning folder for size..."));
			updateRange.add(index);
			files.add(entry->info()->fileName(), NodeItem::Holder(entry));
		}

	updateFirstColumn(updateRange);
	scanForSize(files);
}

void BaseNode::scanForCopy(const ProcessedList &entries, INodeView *destination, bool move)
{
	ICopyControl::Holder control(destination->node()->createControl(destination));

	if (control)
	{
		Union updateRange;
		NodeItem *entry;
		Container::size_type index;
		Snapshot::Files files(m_container.data());
		QString fileLockReason = move ? tr("Moving...") : tr("Copying...");
		QString folderLockReason = move ? tr("Scanning folder for move...") : tr("Scanning folder for copy...");

		for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		{
			index = entries.at(i).first;

			if ((entry = static_cast<NodeItem*>(entries.at(i).second))->info()->isDir())
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

QModelIndex BaseNode::index(int column, NodeItem *item) const
{
	Container::size_type index = m_items.indexOf(item);

	if (index != Container::InvalidIndex)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

QModelIndex BaseNode::indexForFile(NodeItem *item) const
{
	Q_ASSERT(m_items.indexOf(item) != Container::InvalidIndex);
	Container::size_type index = m_items.indexOf(item);
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

QModelIndex BaseNode::indexForFile(NodeItem *item, Container::size_type index) const
{
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

void BaseNode::updateFirstColumn(NodeItem *entry)
{
	updateFirstColumn(m_items.indexOf(entry), entry);
}

void BaseNode::updateFirstColumn(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), 0, m_items[range.at(i).bottom()].data()));
}

void BaseNode::updateFirstColumn(Container::size_type index, NodeItem *entry)
{
	QModelIndex idx = createIndex(index, 0, entry);
	emit dataChanged(idx, idx);
}

void BaseNode::updateSecondColumn(NodeItem *entry)
{
	updateSecondColumn(m_items.indexOf(entry), entry);
}

void BaseNode::updateSecondColumn(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 1, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()].data()));
}

void BaseNode::updateSecondColumn(Container::size_type index, NodeItem *entry)
{
	QModelIndex idx = createIndex(index, 1, entry);
	emit dataChanged(idx, idx);
}

void BaseNode::updateBothColumns(NodeItem *entry)
{
	updateBothColumns(m_items.indexOf(entry), entry);
}

void BaseNode::updateBothColumns(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()].data()));
}

void BaseNode::updateBothColumns(Container::size_type index, NodeItem *entry)
{
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, 1, entry));
}

void BaseNode::updateColumns(const Union &range, int lastColumn)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), lastColumn, m_items[range.at(i).bottom()].data()));
}

void BaseNode::updateColumns(Container::size_type index, NodeItem *entry, int lastColumn)
{
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, lastColumn, entry));
}

void BaseNode::removeEntry(Container::size_type index)
{
	beginRemoveRows(QModelIndex(), index, index);
	m_items.remove(index);
	endRemoveRows();
}

void BaseNode::removeEntry(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	m_items.remove(index.row());
	endRemoveRows();
}

DEFAULT_PLUGIN_NS_END
