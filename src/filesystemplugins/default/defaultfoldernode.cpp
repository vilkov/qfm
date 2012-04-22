#include "defaultfoldernode.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "tasks/perform/performcopytask.h"
#include "tasks/perform/performmovetask.h"
#include "tasks/perform/performremovetask.h"
#include "actions/defaultfoldercopyaction.h"
#include "actions/defaultfoldercutaction.h"
#include "actions/defaultfolderpasteaction.h"
#include "actions/defaultfolderpasteintofolderaction.h"
#include "actions/defaultfolderpropertiesaction.h"
#include "actions/defaultfolderpasteclipboardaction.h"
#include "../../interfaces/imp/filesystemcopycontrol.h"
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


DefaultFolderNode::DefaultFolderNode(IFileContainer::Holder &container, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_container(container.take()),
	m_items(m_itemsContainer.m_list),
	m_updating(false),
	m_proxy(this),
	m_delegate(&m_proxy)
{
	globalActions.addRef();

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

DefaultFolderNode::~DefaultFolderNode()
{
	globalActions.release();
}

int DefaultFolderNode::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant DefaultFolderNode::headerData(int section, Qt::Orientation orientation, int role) const
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

bool DefaultFolderNode::event(QEvent *e)
{
	switch (static_cast<ModelEvent::Type>(e->type()))
	{
		case ModelEvent::UpdateFiles:
		{
			e->accept();
			updateFiles(static_cast<BaseTask::Event*>(e));
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
		case ModelEvent::ScanFilesForSize:
		{
			e->accept();
			scanForSize(static_cast<BaseTask::Event*>(e));
			return true;
		}
		case ModelEvent::ScanFilesForCopy:
		{
			e->accept();
			scanForCopy(static_cast<BaseTask::Event*>(e));
			return true;
		}
		case ModelEvent::CopyFiles:
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

void DefaultFolderNode::refresh()
{
	if (!isUpdating())
		updateFiles();
}

QString DefaultFolderNode::title() const
{
	QString res = m_container->location();
	return res.mid(res.lastIndexOf(QChar('/')) + 1);
}

QAbstractItemModel *DefaultFolderNode::model() const
{
	return const_cast<DefaultFolderNode *>(&m_proxy);
}

QAbstractItemDelegate *DefaultFolderNode::delegate() const
{
	return const_cast<DefaultFolderNode *>(&m_delegate);
}

const INodeView::MenuActionList &DefaultFolderNode::actions() const
{
	return m_menuActions;
}

::History::Entry *DefaultFolderNode::menuAction(QAction *action, INodeView *view)
{
	return NULL;
}

QString DefaultFolderNode::location() const
{
	return m_container->location();
}

QString DefaultFolderNode::location(const QString &fileName) const
{
	return m_container->location(fileName);
}

ICopyControl *DefaultFolderNode::createControl(INodeView *view) const
{
	return new CopyControl(const_cast<DefaultFolderNode *>(this), m_container->location());
}

void DefaultFolderNode::contextMenu(const QModelIndexList &list, INodeView *view)
{
	typedef QSet<FolderBaseItem *>                                ItemsSet;
	typedef QList<FolderBaseItem *>                               ItemsList;
	typedef QMap<const FileAction *, FileAction::FilesList>           ActionsMap;
	typedef QMap<const TaskNodeItem *, ::Tools::Containers::Dot>      ItemsIndexMap;
	typedef ::DesktopEnvironment::ContextMenuFactory::FileActionsList FileActionsList;

	QMenu menu;
	ItemsSet set;
	ActionsMap map;
	ItemsList items;
	QModelIndex index;
	ItemsIndexMap itemsIndex;
	FolderBaseItem *item;
	FileActionsList actions;

	for (ItemsList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!(item = m_items[(index = m_proxy.mapToSource(list.at(i))).row()])->isRootItem() && !set.contains(item))
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
			if ((item = items.at(0))->info().isDir())
			{
				menu.addAction(const_cast<QAction*>(globalActions.pasteIntoFolderAction->action()));

				actions = Application::globalMenu()->actions(::DesktopEnvironment::ContextMenuFactory::SingleFolder);

				for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
					map[actions.at(i)].push_back(FileAction::FilesList::value_type(item, &item->info()));
			}
			else
			{
				menu.addAction(const_cast<QAction*>(globalActions.pasteAction->action()));

				actions = Application::globalMenu()->actions(::DesktopEnvironment::ContextMenuFactory::SingleFile);

				for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
					map[actions.at(i)].push_back(FileAction::FilesList::value_type(item, &item->info()));
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
					files.push_back(FileAction::FilesList::value_type(item, &item->info()));
				}
			}
		}

		for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
		{
			actions = Application::globalMenu()->actions((item = items.at(i))->info().id());

			for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
				map[actions.at(i)].push_back(FileAction::FilesList::value_type(item, &item->info()));
		}
	}

	actions = Application::globalMenu()->actions(::DesktopEnvironment::ContextMenuFactory::AnyFilesOrFolders);

	for (FileActionsList::size_type i = 0, size = actions.size(); i < size; ++i)
	{
		FileAction::FilesList &files = map[actions.at(i)];

		for (ItemsList::size_type i = 0, size = items.size(); i < size; ++i)
		{
			item = items.at(i);
			files.push_back(FileAction::FilesList::value_type(item, &item->info()));
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
				files.push_back(FileAction::FilesList::value_type(item, &item->info()));
			}

		if (action->isAsynchronous())
		{
			PScopedPointer<PerformActionTask> task;

			if (task = static_cast<AsyncFileAction *>(action)->process(this, container(), files))
			{
				Union update;
				TasksItemList list;
				TaskNodeItem *item;
				list.reserve(files.size());

				for (FileAction::FilesList::size_type i = 0, size = files.size(); i < size; ++i)
				{
					item = const_cast<TaskNodeItem *>(static_cast<const TaskNodeItem *>(files.at(i).first));

					item->lock(static_cast<AsyncFileAction *>(action)->lockReason());
					update.add(itemsIndex.value(item));
					list.push_back(item);
				}

				addTask(task.take(), list);
				updateFirstColumn(update);
			}
		}
		else
			static_cast<SyncFileAction *>(action)->process(container(), files);
	}

	menu.clear();
}

void DefaultFolderNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void DefaultFolderNode::createDirectory(const QModelIndex &index, INodeView *view)
{
	QModelIndex idx = m_proxy.mapToSource(index);

	StringDialog dialog(
			tr("Enter name for new directory"),
			tr("Name"),
			idx.isValid() ? m_items[idx.row()]->fileName() : QString(),
			Application::mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{
		QString error;
		IFileContainer::Holder folder(container()->open(dialog.value(), true, error));

		if (folder)
		{
			FolderItem *item;

			beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
			m_items.add(item = new FolderItem(Info(folder->location(), Info::Refresh())));
			endInsertRows();

			view->select(indexForFile(item, m_items.size() - 1));
		}
		else
			QMessageBox::critical(Application::mainWindow(), tr("Failed to create directory..."), error);
	}
}

void DefaultFolderNode::rename(const QModelIndex &index, INodeView *view)
{

}

void DefaultFolderNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void DefaultFolderNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void DefaultFolderNode::cancel(const QModelIndexList &list, INodeView *view)
{

}

void DefaultFolderNode::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void DefaultFolderNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void DefaultFolderNode::copy(const INodeView *source, INodeView *destination)
{

}

void DefaultFolderNode::move(const INodeView *source, INodeView *destination)
{

}

void DefaultFolderNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

void DefaultFolderNode::updateFiles()
{
	if (isVisible())
	{
		IFileContainer::Holder container(new FileContainer(m_container->location()));
		PScopedPointer<UpdateFilesTask> task(new UpdateFilesTask(this, container, updateFilesMap()));
		setUpdating(true);
		handleTask(task.take());
	}
}

void DefaultFolderNode::updateFiles(const BaseTask::Event *e)
{
	typedef const UpdateFilesTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (!event->canceled)
		updateFilesEvent(event->updates);

	if (event->isLastEvent)
	{
		setUpdating(false);
		taskHandled();
	}
}

void DefaultFolderNode::scanForSize(const BaseTask::Event *e)
{
	typedef ScanFilesForSizeTask::Event * NotConstEvent;
	typedef const ScanFilesForSizeTask::Event * Event;
	Event event = static_cast<Event>(e);

	scanForSizeEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

void DefaultFolderNode::scanForCopy(const BaseTask::Event *e)
{
	typedef ScanFilesForCopyTask::Event * NotConstEvent;
	typedef const ScanFilesForCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (scanForCopyEvent(event->canceled, event->snapshot, event->destination.data(), event->move))
		performCopy(event->task, event->snapshot, const_cast<NotConstEvent>(event)->destination, event->move);
	else
		removeAllTaskLinks(event->task);
}

void DefaultFolderNode::scanForRemove(const BaseTask::Event *e)
{
	typedef ScanFilesForRemoveTask::Event * NotConstEvent;
	typedef const ScanFilesForRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (scanForRemoveEvent(event->canceled, event->snapshot))
		performRemove(event->task, event->snapshot);
	else
		removeAllTaskLinks(event->task);
}

void DefaultFolderNode::performCopy(const BaseTask::Event *e)
{
	typedef PerformCopyTask::Event * NotConstEvent;
	typedef const PerformCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (performCopyEvent(event->canceled, event->snapshot, event->move))
		performRemove(event->task, event->snapshot);
	else
		removeAllTaskLinks(event->task);
}

void DefaultFolderNode::performRemove(const BaseTask::Event *e)
{
	typedef PerformRemoveTask::Event * NotConstEvent;
	typedef const PerformRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	performRemoveEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

DefaultFolderNode::Container::Container()
{}

DefaultFolderNode::Container::~Container()
{
	m_list.deleteAll();
}

DefaultFolderNode::Container::size_type DefaultFolderNode::Container::size() const
{
	return m_list.size();
}

DefaultFolderNode::Container::Item *DefaultFolderNode::Container::at(size_type index) const
{
	return m_list.at(index);
}

DefaultFolderNode::Container::size_type DefaultFolderNode::Container::indexOf(Item *item) const
{
	return m_list.indexOf(static_cast<DefaultFolderItem *>(item));
}

void DefaultFolderNode::scanForSize(const TasksItemList &entries)
{
	PScopedPointer<ScanFilesForSizeTask> task(new ScanFilesForSizeTask(this, m_container.data(), entries));
	addTask(task.take(), entries);
}

void DefaultFolderNode::scanForCopy(const TasksItemList &entries, ICopyControl::Holder &destination, bool move)
{
	PScopedPointer<ScanFilesForCopyTask> task(new ScanFilesForCopyTask(this, destination, entries, move));
	addTask(task.take(), entries);
}

void DefaultFolderNode::scanForRemove(const TasksItemList &entries)
{
	PScopedPointer<ScanFilesForRemoveTask> task(new ScanFilesForRemoveTask(this, m_container.data(), entries));
	addTask(task.take(), entries);
}

void DefaultFolderNode::performCopy(BaseTask *oldTask, const Snapshot &snapshot, ICopyControl::Holder &destination, bool move)
{
	if (destination->isPhysical() && move)
	{
		PScopedPointer<PerformMoveTask> task(new PerformMoveTask(this, destination, snapshot));
		resetTask(task.take(), oldTask);
	}
	else
	{
		PScopedPointer<PerformCopyTask> task(new PerformCopyTask(this, destination, snapshot, move));
		resetTask(task.take(), oldTask);
	}
}

void DefaultFolderNode::performRemove(BaseTask *oldTask, const Snapshot &snapshot)
{
	PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, snapshot));
	resetTask(task.take(), oldTask);
}

DEFAULT_PLUGIN_NS_END
