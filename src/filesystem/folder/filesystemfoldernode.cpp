#include "filesystemfoldernode.h"
#include "items/filesystemrootitem.h"
#include "items/filesystementryitem.h"
#include "base/tasks/basetask.h"
#include "../filesystempluginsmanager.h"
#include "../tools/filesystemcommontools.h"
#include "../../tools/widgets/stringdialog/stringdialog.h"
#include "../../application.h"

#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

FolderNode::FolderNode(const Info &info, Node *parent) :
	FolderNodeBase(info, m_items, parent),
	m_proxy(this),
	m_delegate(&m_proxy)
{
	if (!isRoot())
		m_items.add(new FilesystemRootItem());

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

int FolderNode::columnCount(const QModelIndex &parent) const
{
	return 3;
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

IFileInfo *FolderNode::info(const QModelIndex &idx) const
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<FileSystemBaseItem*>(index.internalPointer())->isRootItem())
		return Node::parent();
	else
		return &m_items[index.row()]->info();
}

void FolderNode::menuAction(QAction *action)
{

}

void FolderNode::createFile(const QModelIndex &index)
{

}

void FolderNode::createDirectory(const QModelIndex &index)
{
	QModelIndex idx = m_proxy.mapToSource(index);

	StringDialog dialog(
			tr("Enter name for the new directory"),
			tr("Name"),
			idx.isValid() ? m_items[idx.row()]->info().fileName() : QString(),
			&Application::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{
		QString error;
		PScopedPointer<IFileControl> folder(create(dialog.value(), IFileControl::Folder, error));

		if (!folder)
			QMessageBox::critical(&Application::instance()->mainWindow(), tr("Failed to create directory..."), error);
	}
}

void FolderNode::rename(const QModelIndexList &list)
{
	RenameFunctor functor(this, m_items);
	processIndexList(list, functor);
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

QModelIndex FolderNode::rootIndex() const
{
	if (isRoot())
		return QModelIndex();
	else
		return m_proxy.mapFromSource(createIndex(0, 0, m_items[0]));
}

Node *FolderNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<FileSystemBaseItem*>(index.internalPointer())->isRootItem())
		return Node::parent();
	else
		if (!static_cast<FileSystemEntryItem*>(index.internalPointer())->isLocked())
		{
			FileSystemEntryItem *entry = static_cast<FileSystemEntryItem*>(index.internalPointer());
			entry->info().refresh();

			if (entry->info().exists())
			{
				if (entry->node())
					entry->node()->setParentEntryIndex(idx);
				else
					if (Node *node = createNode(entry->info(), plugins))
					{
						entry->setNode(node);
						node->setParentEntryIndex(idx);
					}

				return entry->node();
			}
			else
				removeEntry(index);
		}

	return 0;
}

Node *FolderNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	ItemsContainer::size_type index = m_items.indexOf(fileName);

	if (index == ItemsContainer::InvalidIndex)
	{
		Info info(absoluteFilePath(fileName));

		if (Node *node = createNode(info, plugins))
		{
			beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
			m_items.add(new FileSystemEntryItem(info, node));
			endInsertRows();

			return node;
		}
		else
		{
			if (info.isFile())
			{
				FileSystemBaseItem *item;

				beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
				m_items.add(item = new FileSystemEntryItem(info));
				endInsertRows();

				selected = indexForFile(item, m_items.lastIndex());
			}

			return this;
		}
	}
	else
	{
		FileSystemEntryItem *item = static_cast<FileSystemEntryItem*>(m_items[index]);

		if (item->node())
			return item->node();
		else
			if (Node *node = createNode(item->info(), plugins))
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

void FolderNode::nodeRemoved(Node *node)
{
	Q_ASSERT(m_items.indexOf(node) != ItemsContainer::InvalidIndex);
	static_cast<FileSystemEntryItem*>(m_items[m_items.indexOf(node)])->setNode(0);
}

UpdatesList::Map FolderNode::updateFilesMap() const
{
	UpdatesList::Map changes;

	for (ItemsContainer::size_type i = isRoot() ? 0 : 1, size = m_items.size(); i < size; ++i)
		changes.insert(m_items[i]->info().fileName(), m_items[i]->info());

	return changes;
}

void FolderNode::updateFilesEvent(const UpdatesList &updates)
{
	UpdatesList updatesLocal(updates);
	RangeIntersection updateRange;
	ItemsContainer::size_type index;

	for (UpdatesList::iterator update = updatesLocal.begin(), end = updatesLocal.end(); update != end;)
		switch (update.value().type())
		{
			case UpdatesList::Updated:
			{
				if ((index = m_items.indexOf(update.key())) != ItemsContainer::InvalidIndex)
				{
					m_items[index]->setInfo(update.value().info());
					updateRange.add(index, index);
				}

				update = updatesLocal.erase(update);
				break;
			}
			case UpdatesList::Deleted:
			{
				if ((index = m_items.indexOf(update.key())) != ItemsContainer::InvalidIndex)
				{
					if (m_items[index]->node())
						m_items[index]->node()->viewCloseAll();

					if (!static_cast<FileSystemEntryItem*>(m_items[index])->isLocked())
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
			m_items.add(new FileSystemEntryItem(added.at(i).info()));
		endInsertRows();
	}
}

void FolderNode::scanForSizeEvent(bool canceled, PScopedPointer<InfoListItem> &entries)
{
	RangeIntersection updateRange;
	ItemsContainer::size_type index;
	FileSystemEntryItem *entry;

	if (canceled)
		for (InfoListItem::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			entry = static_cast<FileSystemEntryItem*>(m_items[index = m_items.indexOf(entries->at(i)->fileName())]);
			entry->clearTotalSize();
			entry->unlock();
			updateRange.add(index, index);
		}
	else
		for (InfoListItem::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			entry = static_cast<FileSystemEntryItem*>(m_items[index = m_items.indexOf(entries->at(i)->fileName())]);
			entry->setTotalSize(static_cast<InfoListItem*>(entries->at(i))->totalSize());
			entry->unlock();
			updateRange.add(index, index);
		}

	tasks().removeAll(entries->at(0)->fileName());
	updateBothColumns(updateRange);
}

void FolderNode::scanForCopyEvent(bool canceled, PScopedPointer<InfoListItem> &entries, PScopedPointer<ICopyControl> &control, bool move)
{
	RangeIntersection updateRange;
	ItemsContainer::size_type index;

	if (canceled)
		control->canceled();
	else
		if (control->start(entries.data(), move))
			if (control->physicalCopyIsNecessary())
			{
				QString lockReason = move ? tr("Moving...") : tr("Copying...");
				InfoItem *entry;

				for (InfoListItem::size_type i = 0, size = entries->size(); i < size; ++i)
				{
					index = m_items.indexOf((entry = entries->at(i))->fileName());
					static_cast<FileSystemEntryItem*>(m_items[index])->lock(lockReason, entry->totalSize());
					updateRange.add(index, index);
				}

				updateSecondColumn(updateRange);
				FolderNodeBase::performCopy(entries, control, move);
				return;
			}
			else
				control->done(false);

	for (InfoListItem::size_type i = 0, size = entries->size(); i < size; ++i)
	{
		index = m_items.indexOf(entries->at(i)->fileName());
		static_cast<FileSystemEntryItem*>(m_items[index])->setTotalSize(entries->at(i)->totalSize());
		static_cast<FileSystemEntryItem*>(m_items[index])->unlock();
		updateRange.add(index, index);
	}

	tasks().removeAll(entries->at(0)->fileName());
	updateBothColumns(updateRange);
}

void FolderNode::scanForRemoveEvent(bool canceled, PScopedPointer<InfoListItem> &entries)
{
	RangeIntersection updateRange;
	ItemsContainer::size_type index;
	QStringList folders;
	QStringList files;
	InfoItem *entry;

	for (InfoListItem::size_type i = 0; i < entries->size();)
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
				entries->decTotalSize(static_cast<InfoListItem*>(entry)->totalSize());

			entries->remove(i);
		}
	}

	if (!canceled &&
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

		for (InfoListItem::size_type i = 0, size = entries->size(); i < size; ++i)
			if ((entry = entries->at(i))->isDir())
			{
				index = m_items.indexOf(entry->fileName());
				static_cast<FileSystemEntryItem*>(m_items[index])->lock(lockReason, entry->totalSize());
				updateRange.add(index, index);
			}

		updateSecondColumn(updateRange);
		FolderNodeBase::performRemove(entries);
	}
	else
		if (!entries->isEmpty())
		{
			for (InfoListItem::size_type i = 0, size = entries->size(); i < size; ++i)
			{
				index = m_items.indexOf(entries->at(i)->fileName());
				static_cast<FileSystemEntryItem*>(m_items[index])->setTotalSize(entries->at(i)->totalSize());
				static_cast<FileSystemEntryItem*>(m_items[index])->unlock();
				updateRange.add(index, index);
			}

			tasks().removeAll(entries->at(0)->fileName());
			updateBothColumns(updateRange);
		}
}

void FolderNode::performCopyEvent(bool canceled, PScopedPointer<InfoListItem> &entries, bool move)
{
	RangeIntersection updateRange;
	ItemsContainer::size_type index;
	InfoItem *entry;

	if (!canceled && move)
	{
		QString lockReason = tr("Removing...");

		for (InfoListItem::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			index = m_items.indexOf((entry = entries->at(i))->fileName());

			if (entry->isDir())
				static_cast<FileSystemEntryItem*>(m_items[index])->lock(lockReason, entry->totalSize());
			else
				static_cast<FileSystemEntryItem*>(m_items[index])->lock(lockReason);

			updateRange.add(index, index);
		}

		updateSecondColumn(updateRange);
		FolderNodeBase::performRemove(entries);
	}
	else
	{
		for (InfoListItem::size_type i = 0, size = entries->size(); i < size; ++i)
		{
			index = m_items.indexOf((entry = entries->at(i))->fileName());
			static_cast<FileSystemEntryItem*>(m_items[index])->unlock();
			updateRange.add(index, index);
		}

		tasks().removeAll(entries->at(0)->fileName());
		updateBothColumns(updateRange);
	}
}

void FolderNode::performRemoveEvent(PScopedPointer<InfoListItem> &entries)
{
	RangeIntersection updateRange;
	ItemsContainer::size_type index;
	InfoItem *entry;

	for (InfoListItem::size_type i = 0, size = entries->size(); i < size; ++i)
		if ((entry = entries->at(i))->shouldRemove())
			removeEntry(m_items.indexOf(entry->fileName()));
		else
		{
			index = m_items.indexOf(entry->fileName());
			static_cast<FileSystemEntryItem*>(m_items[index])->clearTotalSize();
			static_cast<FileSystemEntryItem*>(m_items[index])->unlock();
			updateRange.add(index, index);
		}

	tasks().removeAll(entries->at(0)->fileName());
	updateBothColumns(updateRange);
}

void FolderNode::updateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed)
{
	ItemsContainer::size_type index = m_items.indexOf(fileName);
	FileSystemEntryItem *entry = static_cast<FileSystemEntryItem*>(m_items[index]);

	entry->setDoneSize(progress);
	entry->setTimeElapsed(timeElapsed);
	updateSecondColumn(index, entry);
}

void FolderNode::completedProgressEvent(const QString &fileName, quint64 timeElapsed)
{
	ItemsContainer::size_type index = m_items.indexOf(fileName);
	FileSystemEntryItem *entry = static_cast<FileSystemEntryItem*>(m_items[index]);

	entry->setDoneSize(entry->totalSize().toULongLong());
	entry->setTimeElapsed(timeElapsed);
	updateSecondColumn(index, entry);
}

void FolderNode::CancelFunctor::call(ItemsContainer::size_type index, FileSystemBaseItem *entry)
{
	if (TasksPool::Task *task = m_tasks.take(entry->info().fileName()))
		static_cast<BaseTask*>(task)->cancel();
}

void FolderNode::RenameFunctor::call(ItemsContainer::size_type index, FileSystemBaseItem *entry)
{
	StringDialog dialog(
			entry->info().isDir() ?
					tr("Enter new name for directory \"%1\"").arg(entry->info().fileName()) :
					tr("Enter new name for file \"%1\"").arg(entry->info().fileName()),
			tr("Name"),
			entry->info().fileName(),
			&Application::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{
		QString error;

		if (entry->info().rename(dialog.value(), error))
		{
			Info info(m_info->absoluteFilePath(dialog.value()));

			m_items.replace(index, entry->info().fileName(), info.fileName());
			static_cast<FileSystemEntryItem*>(entry)->setInfo(info);

			if (static_cast<FileSystemEntryItem*>(entry)->node())
			{
				static_cast<FileSystemEntryItem*>(entry)->node()->viewCloseAll();
				static_cast<FileSystemEntryItem*>(entry)->setNode(0);
			}
		}
		else
			QMessageBox::critical(&Application::instance()->mainWindow(),
						entry->info().isDir() ?
							tr("Failed to rename directory \"%1\"").arg(entry->info().fileName()) :
							tr("Failed to rename file \"%1\"").arg(entry->info().fileName()),
						error);
	}
}

void FolderNode::processIndexList(const QModelIndexList &list, Functors::Functor &functor)
{
	QModelIndex index;
	FileSystemBaseItem *entry;
	QSet<FileSystemBaseItem*> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<FileSystemBaseItem*>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && !static_cast<FileSystemEntryItem*>(entry)->isLocked())
			{
				entry->info().refresh();

				if (entry->info().exists())
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
	FileSystemBaseItem *entry;
	QSet<FileSystemBaseItem*> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<FileSystemBaseItem*>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && static_cast<FileSystemEntryItem*>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void FolderNode::scanForRemove(const ProcessedList &entries)
{
	QStringList list;
	FileSystemEntryItem *entry;
	ItemsContainer::size_type index;
	RangeIntersection updateRange;

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
	{
		index = entries.at(i).first;

		if ((entry = static_cast<FileSystemEntryItem*>(entries.at(i).second))->info().isDir())
			entry->lock(tr("Scanning folder for remove..."));
		else
			entry->lock(tr("Removing..."));

		updateRange.add(index, index);
		list.push_back(entry->info().fileName());
	}

	updateFirstColumn(updateRange);
	FolderNodeBase::scanForRemove(list);
}

void FolderNode::scanForSize(const ProcessedList &entries)
{
	QStringList list;
	FileSystemEntryItem *entry;
	ItemsContainer::size_type index;
	RangeIntersection updateRange;

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		if ((entry = static_cast<FileSystemEntryItem*>(entries.at(i).second))->info().isDir())
		{
			index = entries.at(i).first;
			entry->lock(tr("Scanning folder for size..."));
			updateRange.add(index, index);
			list.push_back(entry->info().fileName());
		}

	if (!list.isEmpty())
	{
		updateFirstColumn(updateRange);
		FolderNodeBase::scanForSize(list);
	}
}

void FolderNode::scanForCopy(const ProcessedList &entries, INode *destination, bool move)
{
	PScopedPointer<ICopyControl> control(destination->createControl());

	if (control)
	{
		QStringList list;
		FileSystemEntryItem *entry;
		ItemsContainer::size_type index;
		RangeIntersection updateRange;
		QString fileLockReason = move ? tr("Moving...") : tr("Copying...");
		QString folderLockReason = move ? tr("Scanning folder for move...") : tr("Scanning folder for copy...");

		list.reserve(entries.size());

		for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		{
			index = entries.at(i).first;

			if ((entry = static_cast<FileSystemEntryItem*>(entries.at(i).second))->info().isDir())
				entry->lock(folderLockReason);
			else
				entry->lock(fileLockReason);

			updateRange.add(index, index);
			list.push_back(entry->info().fileName());
		}

		updateFirstColumn(updateRange);
		FolderNodeBase::scanForCopy(list, control, move);
	}
}

QModelIndex FolderNode::index(int column, FileSystemBaseItem *item) const
{
	int index = m_items.indexOf(item);

	if (index != ItemsContainer::InvalidIndex)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

Node *FolderNode::createNode(const Info &info, PluginsManager *plugins) const
{
	if (Node *res = plugins->node(&info, const_cast<FolderNode*>(this)))
		return res;
	else
		if (info.isDir())
			return new FolderNode(info, const_cast<FolderNode*>(this));
		else
			return 0;
}

QModelIndex FolderNode::indexForFile(FileSystemBaseItem *item) const
{
	Q_ASSERT(m_items.indexOf(item) != ItemsContainer::InvalidIndex);
	ItemsContainer::size_type index = m_items.indexOf(item);
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

QModelIndex FolderNode::indexForFile(FileSystemBaseItem *item, ItemsContainer::size_type index) const
{
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

void FolderNode::updateFirstColumn(FileSystemBaseItem *entry)
{
	updateFirstColumn(m_items.indexOf(entry), entry);
}

void FolderNode::updateFirstColumn(const RangeIntersection &range)
{
	for (RangeIntersection::RangeList::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()]),
						 createIndex(range.at(i).bottom(), 0, m_items[range.at(i).bottom()]));
}

void FolderNode::updateFirstColumn(ItemsContainer::size_type index, FileSystemBaseItem *entry)
{
	QModelIndex idx = createIndex(index, 0, entry);
	emit dataChanged(idx, idx);
}

void FolderNode::updateSecondColumn(FileSystemBaseItem *entry)
{
	updateSecondColumn(m_items.indexOf(entry), entry);
}

void FolderNode::updateSecondColumn(const RangeIntersection &range)
{
	for (RangeIntersection::RangeList::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 1, m_items[range.at(i).top()]),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()]));
}

void FolderNode::updateSecondColumn(ItemsContainer::size_type index, FileSystemBaseItem *entry)
{
	QModelIndex idx = createIndex(index, 1, entry);
	emit dataChanged(idx, idx);
}

void FolderNode::updateBothColumns(FileSystemBaseItem *entry)
{
	updateBothColumns(m_items.indexOf(entry), entry);
}

void FolderNode::updateBothColumns(const RangeIntersection &range)
{
	for (RangeIntersection::RangeList::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()]),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()]));
}

void FolderNode::updateBothColumns(ItemsContainer::size_type index, FileSystemBaseItem *entry)
{
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, 1, entry));
}

void FolderNode::updateColumns(const RangeIntersection &range, int lastColumn)
{
	for (RangeIntersection::RangeList::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()]),
						 createIndex(range.at(i).bottom(), lastColumn, m_items[range.at(i).bottom()]));
}

void FolderNode::removeEntry(ItemsContainer::size_type index)
{
	beginRemoveRows(QModelIndex(), index, index);
	m_items.remove(index);
	endRemoveRows();
}

void FolderNode::removeEntry(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	m_items.remove(index.row());
	endRemoveRows();
}

FILE_SYSTEM_NS_END
