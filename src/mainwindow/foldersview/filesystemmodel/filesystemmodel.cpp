#include "filesystemmodel.h"
#include "items/filesystemtree.h"
#include "items/filesystementry.h"
#include "events/filesystemmodelevents.h"
#include "tasks/scan/listfilestask.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "tasks/perform/performremoveentrytask.h"
#include "tasks/perform/performremovetreetask.h"
#include "tasks/perform/performcopyentrytask.h"
#include "tasks/perform/performcopytreetask.h"
#include "visitor/filesystemlockedentryvisitor.h"
#include "../directoryview.h"
#include "../../../tools/rangeintersection.h"
#include "../../../application.h"
#include <QtCore/QSet>
#include <QtCore/QThreadPool>
#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>


FileSystemModel::FileSystemModel(const QString &currentDirectory, DirectoryView *parent) :
	QAbstractItemModel(parent),
	m_currentFsTree(new FileSystemTree(currentDirectory))
{
	list(m_currentFsTree);
}

FileSystemModel::FileSystemModel(const QFileInfo &fileInfo, QObject *parent) :
	QAbstractItemModel(parent),
	m_currentFsTree(new FileSystemTree(fileInfo))
{
	list(m_currentFsTree);
}

FileSystemModel::~FileSystemModel()
{
	while (m_currentFsTree->parent() != 0)
		m_currentFsTree = m_currentFsTree->parent();

	delete m_currentFsTree;
}

bool FileSystemModel::event(QEvent *e)
{
	switch (static_cast<FileSystemModelEvent::Type>(e->type()))
	{
		case FileSystemModelEvent::ListFiles:
		{
			e->accept();
			listEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::UpdateFiles:
		{
			e->accept();
			updateEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::ScanFilesForRemove:
		{
			e->accept();
			scanForRemoveEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::RemoveFilesCompleted:
		{
			e->accept();
			removeCompleteEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::RemoveFilesCanceled:
		{
			e->accept();
			removeCanceledEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::ScanFilesForSize:
		{
			e->accept();
			scanForSizeEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::ScanFilesForCopy:
		{
			e->accept();
			scanForCopyEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::CopyFilesCompleted:
		{
			e->accept();
			copyCompleteEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::CopyFilesCanceled:
		{
			e->accept();
			copyCanceledEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::ScanFilesForMove:
		{
			e->accept();
			scanForMoveEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		case FileSystemModelEvent::QuestionAnswer:
		{
			e->accept();
			questionAnswerEvent(static_cast<FileSystemModelEvent*>(e)->parameters());
			return true;
		}
		default:
			break;
	}

	return QAbstractItemModel::event(e);
}

int FileSystemModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
        return static_cast<FileSystemItem*>(parent.internalPointer())->size();
	else
    	return m_currentFsTree->size();
}

int FileSystemModel::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<FileSystemItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_currentFsTree->child(index.row())->data(index.column(), role);
}

Qt::ItemFlags FileSystemModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
    	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::NoItemFlags;
}

QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex FileSystemModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid())
			return index(row, column, static_cast<FileSystemItem*>(parent.internalPointer()));
		else
			return createIndex(row, column, m_currentFsTree->child(row));
    else
        return QModelIndex();
}

QModelIndex FileSystemModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (FileSystemItem *parentItem = static_cast<FileSystemItem*>(child.internalPointer())->parent())
			if (parentItem != m_currentFsTree)
				if (parentItem->parent())
					return createIndex(parentItem->parent()->indexOf(parentItem), 0, parentItem);
				else
					return createIndex(m_currentFsTree->indexOf(parentItem), 0, parentItem);

    return QModelIndex();
}

bool FileSystemModel::isLocked() const
{
	return !lockedEntries().isEmpty();
}

QStringList FileSystemModel::lockedEntries() const
{
	FileSystemLockedEntryVisitor visitor;
	m_currentFsTree->accept(&visitor);
	return visitor.directories();
}

QModelIndex FileSystemModel::rootIndex() const
{
	if (m_currentFsTree->size() > 0 && m_currentFsTree->child(0)->isRoot())
		return createIndex(0, 0, m_currentFsTree->child(0));
	else
		return QModelIndex();
}

QModelIndex FileSystemModel::parentEntryIndex() const
{
	if (m_currentFsTree->parent() != 0)
	{
		FileSystemTree::size_type index = m_currentFsTree->parent()->indexOf(static_cast<FileSystemTree*>(m_currentFsTree)->parentEntry());

		if (index != FileSystemTree::InvalidIndex)
			return createIndex(index, 0, static_cast<FileSystemTree*>(m_currentFsTree)->parentEntry());
	}

	return QModelIndex();
}

const QFileInfo &FileSystemModel::currentDirectoryInfo() const
{
	return static_cast<FileSystemTree*>(m_currentFsTree)->fileInfo();
}

const QFileInfo &FileSystemModel::fileInfo(const QModelIndex &index) const
{
	if (m_currentFsTree->child(index.row())->isRoot())
		return static_cast<FileSystemTree*>(m_currentFsTree)->fileInfo();
	else
		return static_cast<FileSystemEntry*>(m_currentFsTree->child(index.row()))->fileInfo();
}

void FileSystemModel::pathToClipboard(const QModelIndexList &list) const
{
	QString text;
	QSet<FileSystemItem*> set;
	FileSystemItem *item;
#ifdef Q_OS_WIN32
	QString endl = QString::fromLatin1("\r\n");
#else
	QString endl = QString::fromLatin1("\n");
#endif

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!set.contains(item = static_cast<FileSystemItem*>(list.at(i).internalPointer())))
		{
			set.insert(item);

			if (!item->isRoot())
				text.append(static_cast<FileSystemEntry*>(item)->fileInfo().absoluteFilePath()).append(endl);
		}

	text.chop(endl.size());
	Application::clipboard()->setText(text);
}

QModelIndex FileSystemModel::find(const QString &fileName) const
{
	for (FileSystemItem::size_type i = 0, size = m_currentFsTree->size(); i < size; ++i)
		if (!static_cast<FileSystemItem*>(m_currentFsTree->child(i))->isRoot() &&
			static_cast<FileSystemEntry*>(m_currentFsTree->child(i))->fileInfo().fileName() == fileName)
			return createIndex(i, 0, m_currentFsTree->child(i));

	return QModelIndex();
}

void FileSystemModel::refresh()
{
	if (!isLocked())
		doRefresh();
}

void FileSystemModel::refreshSize(const QModelIndex &index)
{
	if (!static_cast<FileSystemItem*>(index.internalPointer())->isRoot() &&
		!static_cast<FileSystemEntry*>(index.internalPointer())->isLocked())
	{
		FileSystemEntry *entry = static_cast<FileSystemEntry*>(index.internalPointer());
		entry->fileInfo().refresh();

		if (entry->fileInfo().exists())
			if (entry->fileInfo().isDir())
				scanForSize(m_currentFsTree, entry);
			else
				updateSecondColumn(m_currentFsTree, entry);
		else
			removeEntry(index);
	}
}

void FileSystemModel::activated(const QModelIndex &index)
{
	if (static_cast<FileSystemItem*>(index.internalPointer())->isRoot())
		if (FileSystemTree *parent = static_cast<FileSystemTree*>(static_cast<FileSystemTree*>(m_currentFsTree)->parent()))
		{
			beginRemoveRows(QModelIndex(), 0, m_currentFsTree->size() - 1);
			if (!parent->isUpdating())
				update(parent);
			endRemoveRows();

			beginInsertRows(QModelIndex(), 0, parent->size() - 1);
			m_currentFsTree = parent;
			endInsertRows();
		}
		else
		{
			beginRemoveRows(QModelIndex(), 0, m_currentFsTree->size() - 1);
			static_cast<FileSystemTree*>(m_currentFsTree)->fileInfo().refresh();

			if (static_cast<FileSystemTree*>(m_currentFsTree)->fileInfo().exists())
			{
				parent = new FileSystemTree(static_cast<FileSystemTree*>(m_currentFsTree)->fileInfo().absolutePath(), 0);
				parent->add<FileSystemEntry>(FilesTask::info(static_cast<FileSystemTree*>(m_currentFsTree)->fileInfo()));
				parent->setSubtree(static_cast<FileSystemTree*>(m_currentFsTree));
				static_cast<FileSystemTree*>(m_currentFsTree)->setParentEntry(parent->last());
			}
			else
			{
				QDir dir = static_cast<FileSystemTree*>(m_currentFsTree)->fileInfo().absoluteDir();
				while (!dir.exists())
					dir.cdUp();

				parent = new FileSystemTree(dir.absolutePath(), 0);
				delete m_currentFsTree;
			}
			endRemoveRows();

			beginInsertRows(QModelIndex(), 0, parent->size() - 1);
			update(parent);
			m_currentFsTree = parent;
			endInsertRows();
		}
	else
		if (!static_cast<FileSystemEntry*>(index.internalPointer())->isLocked())
		{
			FileSystemEntry *entry = static_cast<FileSystemEntry*>(index.internalPointer());
			entry->fileInfo().refresh();

			if (entry->fileInfo().exists())
				if (entry->fileInfo().isDir())
					if (FileSystemTree *tree = static_cast<FileSystemTree*>(static_cast<FileSystemTree*>(m_currentFsTree)->subtree(entry)))
					{
						beginRemoveRows(QModelIndex(), 0, m_currentFsTree->size() - 1);
						if (!tree->isUpdating())
							update(tree);
						endRemoveRows();

						beginInsertRows(QModelIndex(), 0, tree->size() - 1);
						m_currentFsTree = tree;
						endInsertRows();
					}
					else
					{
						beginRemoveRows(QModelIndex(), 0, m_currentFsTree->size() - 1);
						tree = static_cast<FileSystemTree*>(m_currentFsTree);
						tree->setSubtree(entry, m_currentFsTree = new FileSystemTree(entry->fileInfo().absoluteFilePath(), tree));
						static_cast<FileSystemTree*>(m_currentFsTree)->setParentEntry(entry);
						endRemoveRows();

						beginInsertRows(QModelIndex(), 0, m_currentFsTree->size() - 1);
						list(m_currentFsTree);
						endInsertRows();
					}
				else
				{

				}
			else
			{
				FileSystemEntry::size_type index = m_currentFsTree->indexOf(entry);
				beginRemoveRows(QModelIndex(), index, index);
				static_cast<FileSystemTree*>(m_currentFsTree)->remove(index);
				endRemoveRows();
			}
		}
}

QModelIndex FileSystemModel::setCurrentDirectory(const QString &filePath)
{
	QModelIndex res;
	FileSystemInfo info(filePath);

	if (info.exists())
		res = setCurrentDirectory(info);

	return res;
}

QModelIndex FileSystemModel::setCurrentDirectory(const QFileInfo &info)
{
	if (isLocked())
		return QModelIndex();

	beginRemoveRows(QModelIndex(), 0, m_currentFsTree->size() - 1);

	while (m_currentFsTree->parent() != 0)
		m_currentFsTree = m_currentFsTree->parent();
	delete m_currentFsTree;

	FileSystemTree *tree;

	if (info.isFile())
	{
		tree = new FileSystemTree(info.absolutePath(), 0);
		tree->add<FileSystemEntry>(FilesTask::info(info));
		update(tree);
	}
	else
	{
		tree = new FileSystemTree(info, 0);
		list(tree);
	}

	endRemoveRows();

	beginInsertRows(QModelIndex(), 0, tree->size() - 1);
	m_currentFsTree = tree;
	endInsertRows();

	return createIndex(tree->size() - 1, 0, tree->child(tree->size() - 1));
}

void FileSystemModel::rename(const QModelIndex &index, const QString &newFileName)
{
	if (!static_cast<FileSystemItem*>(index.internalPointer())->isRoot() &&
		!static_cast<FileSystemEntry*>(index.internalPointer())->isLocked())
	{
		FileSystemInfo &info = static_cast<FileSystemEntry*>(index.internalPointer())->fileInfo();
		info.refresh();

		if (info.exists())
		{
			QDir dir = info.absoluteDir();

			if (dir.rename(info.fileName(), newFileName))
			{
				static_cast<FileSystemEntry*>(index.internalPointer())->update(FilesTask::info(FileSystemInfo(dir.absoluteFilePath(newFileName))));
				emit dataChanged(index, index);
			}
		}
		else
			removeEntry(index);
	}
}

void FileSystemModel::createDirectory(const QString &dirName)
{
	const QFileInfo &info = static_cast<FileSystemTree*>(m_currentFsTree)->fileInfo();

	if (info.exists())
	{
		QDir dir = info.absoluteDir();

		if (dir.mkdir(dirName))
		{
			beginInsertRows(QModelIndex(), m_currentFsTree->size(), m_currentFsTree->size());
			static_cast<FileSystemTree*>(m_currentFsTree)->add<FileSystemEntry>(FilesTask::info(FileSystemInfo(dir.absoluteFilePath(dirName))));
			endInsertRows();
		}
	}
}

void FileSystemModel::remove(const QModelIndex &index)
{
	if (!static_cast<FileSystemItem*>(index.internalPointer())->isRoot() &&
		!static_cast<FileSystemEntry*>(index.internalPointer())->isLocked())
	{
		FileSystemInfo &info = static_cast<FileSystemEntry*>(index.internalPointer())->fileInfo();
		info.refresh();

		if (info.exists())
			if (info.isDir())
				scanForRemove(static_cast<FileSystemTree*>(m_currentFsTree), static_cast<FileSystemEntry*>(index.internalPointer()));
			else
				removeEntry(static_cast<FileSystemTree*>(m_currentFsTree), static_cast<FileSystemEntry*>(index.internalPointer()));
		else
			removeEntry(index);
	}
}

void FileSystemModel::copy(const QModelIndex &index, FileSystemModel *destination)
{
	if (!static_cast<FileSystemItem*>(index.internalPointer())->isRoot() &&
		!static_cast<FileSystemEntry*>(index.internalPointer())->isLocked())
	{
		FileSystemInfo &info = static_cast<FileSystemEntry*>(index.internalPointer())->fileInfo();
		info.refresh();

		if (info.exists())
			if (info.isDir())
				scanForCopy(m_currentFsTree, static_cast<FileSystemEntry*>(index.internalPointer()), destination);
			else
				copyEntry(m_currentFsTree, static_cast<FileSystemEntry*>(index.internalPointer()), destination);
		else
			removeEntry(index);
	}
}

void FileSystemModel::move(const QModelIndex &index, FileSystemModel *destination)
{
	if (!static_cast<FileSystemItem*>(index.internalPointer())->isRoot() &&
		!static_cast<FileSystemEntry*>(index.internalPointer())->isLocked())
	{
		FileSystemInfo &info = static_cast<FileSystemEntry*>(index.internalPointer())->fileInfo();
		info.refresh();

		if (info.exists())
			if (info.isDir())
				scanForMove(m_currentFsTree, static_cast<FileSystemEntry*>(index.internalPointer()), destination);
			else
				moveEntry(m_currentFsTree, static_cast<FileSystemEntry*>(index.internalPointer()), destination);
		else
			removeEntry(index);
	}
}

void FileSystemModel::list(FileSystemItem *fileSystemTree)
{
	QScopedPointer<ListFilesTask::Params> params(new ListFilesTask::Params());
	params->object = (QObject*)this;
	params->fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);

	static_cast<FileSystemTree*>(fileSystemTree)->setUpdating(true);
	Application::instance()->taskPool().handle(new ListFilesTask(params.take()));
}

void FileSystemModel::listEvent(const FileSystemModelEvent::Params *p)
{
	typedef const ListFilesTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	if (!params->updates.isEmpty())
		if (params->fileSystemTree == m_currentFsTree)
		{
			beginInsertRows(QModelIndex(), m_currentFsTree->size(), m_currentFsTree->size() + params->updates.size() - 1);
			static_cast<FileSystemTree*>(m_currentFsTree)->add<FileSystemEntry>(params->updates);
			endInsertRows();
		}
		else
			params->fileSystemTree->add<FileSystemEntry>(params->updates);

	if (params->isLastEvent)
		params->fileSystemTree->setUpdating(false);
}

void FileSystemModel::update(FileSystemItem *fileSystemTree)
{
	QScopedPointer<UpdateFilesTask::Params> params(new UpdateFilesTask::Params());
	params->object = (QObject*)this;
	params->fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
	params->list = static_cast<FileSystemTree*>(fileSystemTree)->makeChangeSet();

	static_cast<FileSystemTree*>(fileSystemTree)->setUpdating(true);
	Application::instance()->taskPool().handle(new UpdateFilesTask(params.take()));
}

void FileSystemModel::updateEvent(const FileSystemModelEvent::Params *p)
{
	typedef const UpdateFilesTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	ChangesList list = params->updates;

	if (params->fileSystemTree == m_currentFsTree)
	{
		RangeIntersection updateRange(1);

		for (ChangesList::size_type i = 0; i < list.size();)
			if (list.at(i).type() == Change::Updated)
			{
				FileSystemEntry *entry = static_cast<FileSystemEntry*>(list.at(i).entry());
				FileSystemEntry::size_type index = m_currentFsTree->indexOf(entry);
				entry->update(list.at(i).info());
				updateRange.add(index, index);
				list.removeAt(i);
			}
			else
				if (list.at(i).type() == Change::Deleted)
				{
					removeEntry(m_currentFsTree->indexOf(list.at(i).entry()));
					list.removeAt(i);
				}
				else
					++i;

		for (RangeIntersection::RangeList::size_type i = 0, size = updateRange.size(); i < size; ++i)
			emit dataChanged(createIndex(updateRange.at(i).top(), 0, m_currentFsTree),
							 createIndex(updateRange.at(i).bottom(), columnCount(), m_currentFsTree));

		if (!list.isEmpty())
		{
			beginInsertRows(QModelIndex(), m_currentFsTree->size(), m_currentFsTree->size() + list.size() - 1);
			static_cast<FileSystemTree*>(m_currentFsTree)->add<FileSystemEntry>(list);
			endInsertRows();
		}
	}
	else
	{
		for (ChangesList::size_type i = 0; i < list.size();)
			if (list.at(i).type() == Change::Updated)
			{
				static_cast<FileSystemEntry*>(list.at(i).entry())->update(list.at(i).info());
				list.removeAt(i);
			}
			else
				if (list.at(i).type() == Change::Deleted)
				{
					params->fileSystemTree->remove(params->fileSystemTree->indexOf(list.at(i).entry()));
					list.removeAt(i);
				}
				else
					++i;

		params->fileSystemTree->add<FileSystemEntry>(list);
	}

	if (params->isLastEvent)
		static_cast<FileSystemTree*>(params->fileSystemTree)->setUpdating(false);
}

void FileSystemModel::removeEntry(FileSystemItem *fileSystemTree, FileSystemItem *entry)
{
	if (QMessageBox::question(
			&Application::instance()->mainWindow(),
			tr("Remove file..."),
			tr("Would you like to remove \"%1\" file?").arg(static_cast<FileSystemEntry*>(entry)->fileInfo().absoluteFilePath()),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		QScopedPointer<PerformRemoveEntryTask::Params> params(new PerformRemoveEntryTask::Params());
		params->source.object = (QObject*)this;
		params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
		params->source.entry = static_cast<FileSystemEntry*>(entry);

		static_cast<FileSystemEntry*>(entry)->lock(tr("Removing..."));
		updateFirstColumn(fileSystemTree, entry);

		Application::instance()->taskPool().handle(new PerformRemoveEntryTask(params.take()));
	}
}

void FileSystemModel::scanForRemove(FileSystemItem *fileSystemTree, FileSystemItem *entry)
{
	QScopedPointer<ScanFilesForRemoveTask::Params> params(new ScanFilesForRemoveTask::Params());
	params->source.object = (QObject*)this;
	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
	params->source.entry = static_cast<FileSystemEntry*>(entry);

	static_cast<FileSystemEntry*>(entry)->lock(tr("Scanning folder for remove..."));
	updateFirstColumn(fileSystemTree, entry);

	Application::instance()->taskPool().handle(new ScanFilesForRemoveTask(params.take()));
}

void FileSystemModel::scanForRemoveEvent(const FileSystemModelEvent::Params *p)
{
	typedef const ScanFilesForRemoveTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	params->snapshot.fileSystemTree->setSubtree(params->snapshot.entry, params->subtree);
	params->snapshot.entry->setFileSize(params->size);

	if (QMessageBox::question(
			&Application::instance()->mainWindow(),
			tr("Remove directory..."),
			tr("Would you like to remove \"%1\" directory?").arg(static_cast<FileSystemEntry*>(params->snapshot.entry)->fileInfo().absoluteFilePath()),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		params->snapshot.entry->lock(tr("Removing..."));

		if (params->snapshot.fileSystemTree == m_currentFsTree)
			updateSecondColumn(m_currentFsTree, params->snapshot.entry);

		Application::instance()->taskPool().handle(new PerformRemoveTreeTask(new PerformRemoveTreeTask::Params((QObject*)this, *params)));
	}
	else
	{
		params->snapshot.entry->unlock();

		if (params->snapshot.fileSystemTree == m_currentFsTree)
			updateBothColumns(m_currentFsTree, params->snapshot.entry);
	}
}

void FileSystemModel::removeCompleteEvent(const FileSystemModelEvent::Params *p)
{
	typedef const PerformRemoveEntryTask::CompletedEvent::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	if (params->shoulRemoveEntry)
		if (params->snapshot.fileSystemTree == m_currentFsTree)
			removeEntry(m_currentFsTree->indexOf(params->snapshot.entry));
		else
			params->snapshot.fileSystemTree->remove(params->snapshot.fileSystemTree->indexOf(params->snapshot.entry));
	else
	{
		params->snapshot.fileSystemTree->setSubtree(params->snapshot.entry, 0);
		params->snapshot.entry->unlock();

		if (params->snapshot.fileSystemTree == m_currentFsTree)
			updateFirstColumn(m_currentFsTree, params->snapshot.entry);
	}
}

void FileSystemModel::removeCanceledEvent(const FileSystemModelEvent::Params *p)
{
	typedef const PerformRemoveEntryTask::CanceledEvent::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	params->snapshot.fileSystemTree->setSubtree(params->snapshot.entry, 0);
	params->snapshot.entry->setFileSize(QVariant());
	params->snapshot.entry->unlock();

	if (params->snapshot.fileSystemTree == m_currentFsTree)
		updateBothColumns(m_currentFsTree, params->snapshot.entry);
}

void FileSystemModel::scanForSize(FileSystemItem *fileSystemTree, FileSystemItem *entry)
{
	QScopedPointer<ScanFilesForSizeTask::Params> params(new ScanFilesForSizeTask::Params());
	params->source.object = (QObject*)this;
	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
	params->source.entry = static_cast<FileSystemEntry*>(entry);

	static_cast<FileSystemEntry*>(entry)->lock(tr("Scanning folder for size..."));
	updateFirstColumn(fileSystemTree, entry);

	Application::instance()->taskPool().handle(new ScanFilesForSizeTask(params.take()));
}

void FileSystemModel::scanForSizeEvent(const FileSystemModelEvent::Params *p)
{
	typedef const ScanFilesForSizeTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	static_cast<FileSystemEntry*>(params->snapshot.entry)->setFileSize(params->size);
	params->snapshot.entry->unlock();

	if (m_currentFsTree == params->snapshot.fileSystemTree)
		updateBothColumns(m_currentFsTree, params->snapshot.entry);
}

void FileSystemModel::copyEntry(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination)
{
	QScopedPointer<PerformCopyEntryTask::Params> params(new PerformCopyEntryTask::Params());
	params->source.object = (QObject*)this;
	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
	params->source.entry = static_cast<FileSystemEntry*>(entry);
	params->destination.object = destination;
	params->destination.fileSystemTree = static_cast<FileSystemTree*>(destination->m_currentFsTree);
	params->removeSource = false;

	static_cast<FileSystemEntry*>(entry)->lock(tr("Copying..."));
	updateFirstColumn(fileSystemTree, entry);

	Application::instance()->taskPool().handle(new PerformCopyEntryTask(params.take()));
}

void FileSystemModel::scanForCopy(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination)
{
	QScopedPointer<ScanFilesForCopyTask::Params> params(new ScanFilesForCopyTask::Params());
	params->source.object = (QObject*)this;
	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
	params->source.entry = static_cast<FileSystemEntry*>(entry);
	params->destination.object = destination;
	params->destination.fileSystemTree = static_cast<FileSystemTree*>(destination->m_currentFsTree);

	static_cast<FileSystemEntry*>(entry)->lock(tr("Scanning folder for copy..."));
	updateFirstColumn(fileSystemTree, entry);

	Application::instance()->taskPool().handle(new ScanFilesForCopyTask(params.take()));
}

void FileSystemModel::moveEntry(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination)
{
	QScopedPointer<PerformCopyEntryTask::Params> params(new PerformCopyEntryTask::Params());
	params->source.object = (QObject*)this;
	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
	params->source.entry = static_cast<FileSystemEntry*>(entry);
	params->destination.object = destination;
	params->destination.fileSystemTree = static_cast<FileSystemTree*>(destination->m_currentFsTree);
	params->removeSource = true;

	static_cast<FileSystemEntry*>(entry)->lock(tr("Moving..."));
	updateFirstColumn(fileSystemTree, entry);

	Application::instance()->taskPool().handle(new PerformCopyEntryTask(params.take()));
}

void FileSystemModel::scanForMove(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination)
{
	QScopedPointer<ScanFilesForMoveTask::Params> params(new ScanFilesForMoveTask::Params());
	params->source.object = (QObject*)this;
	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
	params->source.entry = static_cast<FileSystemEntry*>(entry);
	params->destination.object = destination;
	params->destination.fileSystemTree = static_cast<FileSystemTree*>(destination->m_currentFsTree);

	static_cast<FileSystemEntry*>(entry)->lock(tr("Scanning folder for move..."));
	updateFirstColumn(fileSystemTree, entry);

	Application::instance()->taskPool().handle(new ScanFilesForMoveTask(params.take()));
}

void FileSystemModel::scanForCopyEvent(const FileSystemModelEvent::Params *p)
{
	typedef const ScanFilesForCopyTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	params->snapshot.fileSystemTree->setSubtree(params->snapshot.entry, params->subtree);
	params->snapshot.entry->setFileSize(params->size);
	params->snapshot.entry->lock(tr("Copying..."));

	if (m_currentFsTree == params->snapshot.fileSystemTree)
		updateSecondColumn(m_currentFsTree, params->snapshot.entry);

	Application::instance()->taskPool().handle(new PerformCopyTreeTask(new PerformCopyTreeTask::Params((QObject*)this, *params, false)));
}

void FileSystemModel::scanForMoveEvent(const FileSystemModelEvent::Params *p)
{
	typedef const ScanFilesForMoveTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	params->snapshot.fileSystemTree->setSubtree(params->snapshot.entry, params->subtree);
	params->snapshot.entry->setFileSize(params->size);
	params->snapshot.entry->lock(tr("Moving..."));

	if (m_currentFsTree == params->snapshot.fileSystemTree)
		updateSecondColumn(m_currentFsTree, params->snapshot.entry);

	Application::instance()->taskPool().handle(new PerformCopyTreeTask(new PerformCopyTreeTask::Params((QObject*)this, *params, true)));
}

void FileSystemModel::copyCompleteEvent(const FileSystemModelEvent::Params *p)
{
	typedef const PerformCopyEntryTask::CompletedEvent::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	if (params->removeSource)
	{
		FileSystemInfo &info = params->snapshot.entry->fileInfo();
		info.refresh();

		if (info.exists())
			if (info.isDir())
			{
				typedef const PerformCopyTreeTask::CompletedEvent::Params *ParamsType;
				ParamsType params = static_cast<ParamsType>(p);

				params->snapshot.entry->lock(tr("Removing..."));
				Application::instance()->taskPool().handle(new PerformRemoveTreeTask(new PerformRemoveTreeTask::Params((QObject*)this, *params)));
			}
			else
			{
				QScopedPointer<PerformRemoveEntryTask::Params> newParams(new PerformRemoveEntryTask::Params());
				newParams->source.object = (QObject*)this;
				newParams->source.fileSystemTree = params->snapshot.fileSystemTree;
				newParams->source.entry = params->snapshot.entry;

				params->snapshot.entry->lock(tr("Removing..."));
				Application::instance()->taskPool().handle(new PerformRemoveEntryTask(newParams.take()));
			}
		else
			if (params->snapshot.fileSystemTree == m_currentFsTree)
				removeEntry(m_currentFsTree->indexOf(params->snapshot.entry));
			else
				params->snapshot.fileSystemTree->remove(params->snapshot.fileSystemTree->indexOf(params->snapshot.entry));
	}
	else
	{
		params->snapshot.entry->unlock();

		if (params->snapshot.fileSystemTree == m_currentFsTree)
			updateBothColumns(m_currentFsTree, params->snapshot.entry);
	}

	if (params->destination.object)
		static_cast<FileSystemModel*>(params->destination.object)->refresh(params->destination.fileSystemTree);
}

void FileSystemModel::copyCanceledEvent(const FileSystemModelEvent::Params *p)
{
	copyCompleteEvent(p);
}

void FileSystemModel::questionAnswerEvent(const FileSystemModelEvent::Params *p)
{
	typedef FileSystemModelEvents::QuestionAnswerParams * ParamsType;
	ParamsType params = (ParamsType)p;

	params->result->lock();
	params->result->setAnswer(QMessageBox::question(&Application::instance()->mainWindow(), params->title, params->question, params->buttons));
	params->result->unlock();
}

void FileSystemModel::updateFirstColumn(FileSystemItem *fileSystemTree, FileSystemItem *entry)
{
	QModelIndex index = createIndex(fileSystemTree->indexOf(entry), 0, entry);
	emit dataChanged(index, index);
}

void FileSystemModel::updateSecondColumn(FileSystemItem *fileSystemTree, FileSystemItem *entry)
{
	QModelIndex index = createIndex(fileSystemTree->indexOf(entry), 1, entry);
	emit dataChanged(index, index);
}

void FileSystemModel::updateBothColumns(FileSystemItem *fileSystemTree, FileSystemItem *entry)
{
	FileSystemItem::size_type index = fileSystemTree->indexOf(entry);
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, 1, entry));
}

QModelIndex FileSystemModel::index(int column, FileSystemItem *item) const
{
	int index = m_currentFsTree->indexOf(item);

	if (index != -1)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

QModelIndex FileSystemModel::index(int row, int column, FileSystemItem *parentItem) const
{
	FileSystemItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

void FileSystemModel::removeEntry(FileSystemItem::size_type index)
{
	beginRemoveRows(QModelIndex(), index, index);
	static_cast<FileSystemTree*>(m_currentFsTree)->remove(index);
	endRemoveRows();
}

void FileSystemModel::removeEntry(const QModelIndex &index)
{
	removeEntry(index.row());
}

void FileSystemModel::refresh(FileSystemItem *fileSystemTree)
{
	if (fileSystemTree == m_currentFsTree)
		doRefresh();
}

void FileSystemModel::doRefresh()
{
	FileSystemTree *tree = static_cast<FileSystemTree*>(m_currentFsTree);

	if (tree->isUpdating())
		return;

	tree->fileInfo().refresh();
	if (tree->fileInfo().exists())
		update(tree);
	else
	{
		beginRemoveRows(QModelIndex(), 0, tree->size() - 1);

		QDir dir = tree->fileInfo().absoluteDir();
		while (!dir.exists())
			dir.cdUp();

		delete m_currentFsTree;
		m_currentFsTree = new FileSystemTree(dir.absolutePath(), 0);
		list(m_currentFsTree);

		endRemoveRows();
	}
}
