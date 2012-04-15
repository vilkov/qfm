#include "filesystemrootfoldernode.h"
#include "filesystemfoldernode.h"
#include "items/filesystemfolderitem.h"
#include "actions/filesystemfoldercopyaction.h"
#include "actions/filesystemfoldercutaction.h"
#include "actions/filesystemfolderpasteaction.h"
#include "actions/filesystemfolderpasteintofolderaction.h"
#include "actions/filesystemfolderpropertiesaction.h"
#include "actions/filesystemfolderpasteclipboardaction.h"
#include "../filesystempluginsmanager.h"
#include "../tools/filesystemcommontools.h"
#include "../tasks/concrete/perform/performactiontask.h"
#include "../../tools/widgets/stringdialog/stringdialog.h"
#include "../../application.h"

#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

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


RootFolderNode::RootFolderNode(IFileContainer::Holder &container, Node *parent) :
	FolderNodeBase(container, m_items, parent),
	m_proxy(this),
	m_delegate(&m_proxy)
{
	globalActions.addRef();

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

RootFolderNode::~RootFolderNode()
{
	globalActions.release();
}

int RootFolderNode::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant RootFolderNode::headerData(int section, Qt::Orientation orientation, int role) const
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

void RootFolderNode::contextMenu(const QModelIndexList &list, INodeView *view)
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

void RootFolderNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void RootFolderNode::createDirectory(const QModelIndex &index, INodeView *view)
{
	QModelIndex idx = m_proxy.mapToSource(index);

	StringDialog dialog(
			tr("Enter name for the new directory"),
			tr("Name"),
			idx.isValid() ? m_items[idx.row()]->info().fileName() : QString(),
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

void RootFolderNode::rename(const QModelIndex &index, INodeView *view)
{
	FolderBaseItem *entry = static_cast<FolderBaseItem*>(m_proxy.mapToSource(index).internalPointer());

	if (!entry->isRootItem() && !static_cast<FolderItem*>(entry)->isLocked())
	{
//		RenameFunctor functor(this, m_items);
//		functor(index.row(), entry);
	}
}

void RootFolderNode::rename(const QModelIndexList &list, INodeView *view)
{
//	RenameFunctor functor(this, m_items);
//	processIndexList(list, functor);
}

void RootFolderNode::remove(const QModelIndexList &list, INodeView *view)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForRemove(entries);
}

void RootFolderNode::cancel(const QModelIndexList &list, INodeView *view)
{
	CancelFunctor cancelFunctor(this, tr("Canceling..."));
	processLockedIndexList(list, cancelFunctor);
	updateFirstColumn(cancelFunctor.updateUnion());
}

void RootFolderNode::calculateSize(const QModelIndexList &list, INodeView *view)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForSize(entries);
}

void RootFolderNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{
	AbsoluteFilePathList pathList(container());
	processIndexList(list, pathList);
	Application::instance()->clipboard()->setText(pathList.join(QChar('\r')));
}

void RootFolderNode::copy(const INodeView *source, INodeView *destination)
{
	ProcessedList entries;
	processIndexList(source->selectedIndexes(), entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, false);
}

void RootFolderNode::move(const INodeView *source, INodeView *destination)
{
	ProcessedList entries;
	processIndexList(source->selectedIndexes(), entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, true);
}

void RootFolderNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

QString RootFolderNode::location(const QModelIndex &index) const
{
	QModelIndex idx = m_proxy.mapToSource(index);

	if (idx.isValid())
		return container()->location(static_cast<FolderBaseItem *>(idx.internalPointer())->info().fileName());
	else
		return QString();
}

QAbstractItemModel *RootFolderNode::model() const
{
	return const_cast<FolderProxyModel *>(&m_proxy);
}

QAbstractItemDelegate *RootFolderNode::delegate() const
{
	return const_cast<FolderDelegate *>(&m_delegate);
}

const INodeView::MenuActionList &RootFolderNode::actions() const
{
	return m_menuActions;
}

::History::Entry *RootFolderNode::menuAction(QAction *action, INodeView *view)
{
	return NULL;
}

QModelIndex RootFolderNode::rootIndex() const
{
	return QModelIndex();
}

Node *RootFolderNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<FolderBaseItem *>(index.internalPointer())->isRootItem())
		return parentNode();
	else
		if (!static_cast<FolderItem *>(index.internalPointer())->isLocked())
		{
			FolderItem *entry = static_cast<FolderItem *>(index.internalPointer());

			if (entry->node())
				entry->node()->setParentEntryIndex(idx);
			else
				if (Node *node = createNode(&entry->info(), plugins))
				{
					entry->setNode(node);
					node->setParentEntryIndex(idx);
				}

			if (entry->node())
				return entry->node();
			else
				if (entry->info().isFile())
					Application::desktopService()->open(container(), &entry->info());
		}

	return NULL;
}

Node *RootFolderNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	ItemsContainer::size_type index = m_items.indexOf(fileName);

	if (index == ItemsContainer::InvalidIndex)
	{
		Info info(location(fileName), Info::Refresh());

		if (Node *node = createNode(&info, plugins))
		{
			beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
			m_items.add(new FolderItem(info, node));
			endInsertRows();

			return node;
		}
		else
		{
			if (info.isFile())
			{
				FolderBaseItem *item;

				beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
				m_items.add(item = new FolderItem(info));
				endInsertRows();

				selected = indexForFile(item, m_items.lastIndex());
			}

			return this;
		}
	}
	else
	{
		FolderItem *item = static_cast<FolderItem*>(m_items[index]);

		if (item->node())
			return item->node();
		else
			if (Node *node = createNode(&item->info(), plugins))
			{
				item->setNode(node);
				return node;
			}
			else
			{
				if (item->info().isFile())
					selected = indexForFile(item, index);

				return this;
			}
	}

	return 0;
}

void RootFolderNode::nodeRemoved(Node *node)
{
	ItemsContainer::size_type index = m_items.indexOf(node);

	if (index != ItemsContainer::InvalidIndex)
		static_cast<FolderItem*>(m_items[index])->setNode(NULL);
}

void RootFolderNode::doesNotExistAnyMore()
{
	if (m_items.size() > 1)
	{
		beginRemoveRows(QModelIndex(), 1, m_items.size() - 1);
		m_items.clear();
		endRemoveRows();
	}
}

UpdatesList::Changes RootFolderNode::updateFilesMap() const
{
	UpdatesList::Changes changes;

	for (ItemsContainer::size_type i = 0, size = m_items.size(); i < size; ++i)
		changes.insert(m_items[i]->info().fileName(), m_items[i]->info());

	return changes;
}

void RootFolderNode::updateFilesEvent(const UpdatesList &updates)
{
	Union updateRange;
	UpdatesList updatesLocal(updates);
	ItemsContainer::size_type index;

	for (UpdatesList::iterator update = updatesLocal.begin(), end = updatesLocal.end(); update != end;)
		switch (update.value().type())
		{
			case UpdatesList::Added:
			case UpdatesList::Updated:
			{
				if ((index = m_items.indexOf(update.key())) != ItemsContainer::InvalidIndex)
				{
					m_items[index]->setInfo(update.value().info());
					updateRange.add(index);

					update = updatesLocal.erase(update);
				}
				else
					++update;

				break;
			}
			case UpdatesList::Deleted:
			{
				if ((index = m_items.indexOf(update.key())) != ItemsContainer::InvalidIndex)
				{
					if (!static_cast<FolderItem*>(m_items[index])->isLocked())
						removeEntry(index);
				}

				update = updatesLocal.erase(update);
				break;
			}
			default:
			{
				++update;
				break;
			}
		}

	updateColumns(updateRange, columnCount(QModelIndex()) - 1);

	if (!updatesLocal.isEmpty())
	{
		UpdatesList::Values added = updatesLocal.values();

		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + added.size() - 1);
		for (UpdatesList::Values::size_type i = 0, size = added.size(); i < size; ++i)
			m_items.add(new FolderItem(added.at(i).info()));
		endInsertRows();
	}
}

void RootFolderNode::scanForSizeEvent(bool canceled, const Snapshot &snapshot)
{
	Union updateRange;
	ItemsContainer::size_type index;
	FolderItem *entry;
	Snapshot::List list(snapshot);

	if (canceled)
		for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
		{
			entry = static_cast<FolderItem*>(m_items[index = m_items.indexOf(list.at(i).second->fileName())]);
			entry->clearTotalSize();
			entry->unlock();
			updateRange.add(index);
		}
	else
		for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
		{
			entry = static_cast<FolderItem*>(m_items[index = m_items.indexOf(list.at(i).second->fileName())]);
			entry->setTotalSize(list.at(i).second->totalSize());
			entry->unlock();
			updateRange.add(index);
		}

	updateBothColumns(updateRange);
}

bool RootFolderNode::scanForCopyEvent(bool canceled, const Snapshot &snapshot, ICopyControl *control, bool move)
{
	Union updateRange;
	ItemsContainer::size_type index;
	Snapshot::Files files(snapshot);

	if (canceled)
		control->canceled();
	else
		if (control->start(files, move))
		{
			QString lockReason = move ? tr("Moving...") : tr("Copying...");
			const InfoItem *entry;

			for (Snapshot::Files::size_type i = 0, size = files.size(); i < size; ++i)
			{
				index = m_items.indexOf((entry = files.at(i))->fileName());
				static_cast<FolderItem*>(m_items[index])->lock(lockReason, entry->totalSize());
				updateRange.add(index);
			}

			updateSecondColumn(updateRange);
			return true;
		}
		else
			control->done(false);

	for (Snapshot::Files::size_type i = 0, size = files.size(); i < size; ++i)
	{
		index = m_items.indexOf(files.at(i)->fileName());
		static_cast<FolderItem*>(m_items[index])->setTotalSize(files.at(i)->totalSize());
		static_cast<FolderItem*>(m_items[index])->unlock();
		updateRange.add(index);
	}

	updateBothColumns(updateRange);
	return false;
}

bool RootFolderNode::scanForRemoveEvent(bool canceled, const Snapshot &snapshot)
{
	Union updateRange;
	ItemsContainer::size_type index;
	QStringList folders;
	QStringList files;
	InfoItem *entry;
	Snapshot::List list(snapshot);

	for (Snapshot::List::size_type i = 0; i < list.size(); ++i)
		if ((entry = list.at(i).second)->isDir())
			folders.push_back(entry->fileName());
		else
			files.push_back(entry->fileName());

	if (!canceled &&
		QMessageBox::question(
			Application::mainWindow(),
			tr("Remove..."),
			tr("Would you like to remove").
				append(QString::fromLatin1("\n\t")).
				append(tr("directories:")).append(QString::fromLatin1("\n\t\t")).
				append(folders.join(QString::fromLatin1("\n\t\t"))).
				append(QString::fromLatin1("\n\t")).
				append(tr("files:")).append(QString::fromLatin1("\n\t\t")).
				append(files.join(QString::fromLatin1("\n\t\t"))).
				append(QString::fromLatin1("\n")).
				append(tr("it will free ").append(Tools::humanReadableSize(snapshot.totalSize()))),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		QString lockReason = tr("Removing...");

		for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
			if ((entry = list.at(i).second)->isDir())
			{
				index = m_items.indexOf(entry->fileName());
				static_cast<FolderItem*>(m_items[index])->lock(lockReason, entry->totalSize());
				updateRange.add(index);
			}

		updateSecondColumn(updateRange);
		return true;
	}
	else
	{
		for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
		{
			index = m_items.indexOf(list.at(i).second->fileName());
			static_cast<FolderItem*>(m_items[index])->setTotalSize(list.at(i).second->totalSize());
			static_cast<FolderItem*>(m_items[index])->unlock();
			updateRange.add(index);
		}

		updateBothColumns(updateRange);
	}

	return false;
}

bool RootFolderNode::performCopyEvent(bool canceled, const Snapshot &snapshot, bool move)
{
	Union updateRange;
	ItemsContainer::size_type index;
	InfoItem *entry;
	Snapshot::List list(snapshot);

	if (!canceled && move)
	{
		QString lockReason = tr("Removing...");

		for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
		{
			index = m_items.indexOf((entry = list.at(i).second)->fileName());

			if (entry->isDir())
				static_cast<FolderItem*>(m_items[index])->lock(lockReason, entry->totalSize());
			else
				static_cast<FolderItem*>(m_items[index])->lock(lockReason);

			updateRange.add(index);
		}

		updateSecondColumn(updateRange);

		return true;
	}
	else
	{
		for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
		{
			index = m_items.indexOf((entry = list.at(i).second)->fileName());
			static_cast<FolderItem*>(m_items[index])->unlock();
			updateRange.add(index);
		}

		updateBothColumns(updateRange);
	}

	return false;
}

void RootFolderNode::performRemoveEvent(bool canceled, const Snapshot &snapshot)
{
	Union updateRange;
	ItemsContainer::size_type index;
	InfoItem *entry;
	Snapshot::List list(snapshot);

	for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
		if ((entry = list.at(i).second)->isRemoved())
			removeEntry(m_items.indexOf(entry->fileName()));
		else
		{
			index = m_items.indexOf(entry->fileName());
			static_cast<FolderItem*>(m_items[index])->clearTotalSize();
			static_cast<FolderItem*>(m_items[index])->unlock();
			updateRange.add(index);
		}

	updateBothColumns(updateRange);
}

void RootFolderNode::updateProgressEvent(const NodeItem *item, quint64 progress, quint64 timeElapsed)
{
	ItemsContainer::size_type index = m_items.indexOf(static_cast<ItemsContainer::value_type>(const_cast<NodeItem *>(item))->info().fileName());
	FolderItem *entry = static_cast<FolderItem*>(m_items[index]);

	entry->updateProgress(progress, timeElapsed);
	updateSecondColumn(index, entry);
}

void RootFolderNode::completedProgressEvent(const NodeItem *item, quint64 timeElapsed)
{
	ItemsContainer::size_type index = m_items.indexOf(static_cast<ItemsContainer::value_type>(const_cast<NodeItem *>(item))->info().fileName());
	FolderItem *entry = static_cast<FolderItem*>(m_items[index]);

	entry->updateProgress(entry->total(), timeElapsed);
	updateSecondColumn(index, entry);
}

void RootFolderNode::performActionEvent(const AsyncFileAction::FilesList &files)
{
	Union update;
	TaskNodeItem *item;

	for (FileAction::FilesList::size_type i = 0, size = files.size(); i < size; ++i)
	{
		item = const_cast<TaskNodeItem *>(static_cast<const TaskNodeItem *>(files.at(i).first));

		item->unlock();
		update.add(m_items.indexOf(item));
	}

	updateFirstColumn(update);
}

Node *RootFolderNode::createNode(const IFileInfo *file, PluginsManager *plugins) const
{
	if (Node *res = plugins->node(container(), file, const_cast<RootFolderNode *>(this)))
		return res;
	else
		if (file->isDir())
		{
			QString error;
			IFileContainer::Holder folder(container()->open(file->fileName(), false, error));

			if (folder)
				return new FolderNode(folder, const_cast<RootFolderNode *>(this));
			else
				QMessageBox::critical(Application::mainWindow(), tr("Error"), error);
		}

	return NULL;
}

void RootFolderNode::CancelFunctor::call(ItemsContainer::size_type index, FolderBaseItem *entry)
{
	m_items = m_node->cancelTaskAndTakeItems(entry);

	for (TasksItemList::size_type i = 0, size = m_items.size(); i < size; ++i)
	{
		m_union.add(index);
		static_cast<TaskNodeItem *>(m_items.at(i))->cancel(m_reason);
	}
}

void RootFolderNode::RenameFunctor::call(ItemsContainer::size_type index, FolderBaseItem *entry)
{
	StringDialog dialog(
			entry->info().isDir() ?
					tr("Enter new name for directory \"%1\"").arg(entry->info().fileName()) :
					tr("Enter new name for file \"%1\"").arg(entry->info().fileName()),
			tr("Name"),
			entry->info().fileName(),
			Application::mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{
		QString error;

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
			QMessageBox::critical(Application::mainWindow(),
						entry->info().isDir() ?
							tr("Failed to rename directory \"%1\"").arg(entry->info().fileName()) :
							tr("Failed to rename file \"%1\"").arg(entry->info().fileName()),
						error);
	}
}

void RootFolderNode::processIndexList(const QModelIndexList &list, Functors::Functor &functor)
{
	QModelIndex index;
	FolderBaseItem *entry;
	QSet<FolderBaseItem*> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<FolderBaseItem*>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && !static_cast<FolderItem*>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void RootFolderNode::processLockedIndexList(const QModelIndexList &list, Functors::Functor &functor)
{
	QModelIndex index;
	ProcessedList res;
	FolderBaseItem *entry;
	QSet<FolderBaseItem*> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<FolderBaseItem*>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && static_cast<FolderItem*>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void RootFolderNode::scanForRemove(const ProcessedList &entries)
{
	Union updateRange;
	TasksItemList list;
	FolderItem *entry;
	ItemsContainer::size_type index;

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
	{
		index = entries.at(i).first;

		if ((entry = static_cast<FolderItem*>(entries.at(i).second))->info().isDir())
			entry->lock(tr("Scanning folder for remove..."));
		else
			entry->lock(tr("Removing..."));

		updateRange.add(index);
		list.push_back(entry);
	}

	updateFirstColumn(updateRange);
	FolderNodeBase::scanForRemove(list);
}

void RootFolderNode::scanForSize(const ProcessedList &entries)
{
	Union updateRange;
	TasksItemList list;
	FolderItem *entry;
	ItemsContainer::size_type index;

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		if ((entry = static_cast<FolderItem*>(entries.at(i).second))->info().isDir())
		{
			index = entries.at(i).first;
			entry->lock(tr("Scanning folder for size..."));
			updateRange.add(index);
			list.push_back(entry);
		}

	if (!list.isEmpty())
	{
		updateFirstColumn(updateRange);
		FolderNodeBase::scanForSize(list);
	}
}

void RootFolderNode::scanForCopy(const ProcessedList &entries, INodeView *destination, bool move)
{
	ICopyControl::Holder control(destination->node()->createControl(destination));

	if (control)
	{
		Union updateRange;
		TasksItemList list;
		FolderItem *entry;
		ItemsContainer::size_type index;
		QString fileLockReason = move ? tr("Moving...") : tr("Copying...");
		QString folderLockReason = move ? tr("Scanning folder for move...") : tr("Scanning folder for copy...");

		list.reserve(entries.size());

		for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		{
			index = entries.at(i).first;

			if ((entry = static_cast<FolderItem*>(entries.at(i).second))->info().isDir())
				entry->lock(folderLockReason);
			else
				entry->lock(fileLockReason);

			updateRange.add(index);
			list.push_back(entry);
		}

		updateFirstColumn(updateRange);
		FolderNodeBase::scanForCopy(list, control, move);
	}
}

QModelIndex RootFolderNode::index(int column, FolderBaseItem *item) const
{
	ItemsContainer::size_type index = m_items.indexOf(item);

	if (index != ItemsContainer::InvalidIndex)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

QModelIndex RootFolderNode::indexForFile(FolderBaseItem *item) const
{
	Q_ASSERT(m_items.indexOf(item) != ItemsContainer::InvalidIndex);
	ItemsContainer::size_type index = m_items.indexOf(item);
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

QModelIndex RootFolderNode::indexForFile(FolderBaseItem *item, ItemsContainer::size_type index) const
{
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

void RootFolderNode::updateFirstColumn(FolderBaseItem *entry)
{
	updateFirstColumn(m_items.indexOf(entry), entry);
}

void RootFolderNode::updateFirstColumn(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()]),
						 createIndex(range.at(i).bottom(), 0, m_items[range.at(i).bottom()]));
}

void RootFolderNode::updateFirstColumn(ItemsContainer::size_type index, FolderBaseItem *entry)
{
	QModelIndex idx = createIndex(index, 0, entry);
	emit dataChanged(idx, idx);
}

void RootFolderNode::updateSecondColumn(FolderBaseItem *entry)
{
	updateSecondColumn(m_items.indexOf(entry), entry);
}

void RootFolderNode::updateSecondColumn(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 1, m_items[range.at(i).top()]),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()]));
}

void RootFolderNode::updateSecondColumn(ItemsContainer::size_type index, FolderBaseItem *entry)
{
	QModelIndex idx = createIndex(index, 1, entry);
	emit dataChanged(idx, idx);
}

void RootFolderNode::updateBothColumns(FolderBaseItem *entry)
{
	updateBothColumns(m_items.indexOf(entry), entry);
}

void RootFolderNode::updateBothColumns(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()]),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()]));
}

void RootFolderNode::updateBothColumns(ItemsContainer::size_type index, FolderBaseItem *entry)
{
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, 1, entry));
}

void RootFolderNode::updateColumns(const Union &range, int lastColumn)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()]),
						 createIndex(range.at(i).bottom(), lastColumn, m_items[range.at(i).bottom()]));
}

void RootFolderNode::removeEntry(ItemsContainer::size_type index)
{
	beginRemoveRows(QModelIndex(), index, index);
	m_items.remove(index);
	endRemoveRows();
}

void RootFolderNode::removeEntry(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	m_items.remove(index.row());
	endRemoveRows();
}

FILE_SYSTEM_NS_END
