#include "filesystemfoldernode.h"
#include "items/filesystemfoldernoderoot.h"
#include "items/filesystemfoldernodeentry.h"
#include "items/filesystemfoldernodeitemlist.h"
#include "events/filesystemmodelevents.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "tasks/perform/performremovetreetask.h"
#include "../filesystempluginsmanager.h"
#include "../../tools/rangeintersection.h"
#include "../../application.h"
#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

FolderNode::FolderNode(const Info &info, Node *parent) :
	Node(parent),
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
		case ModelEvent::ListFiles:
		{
			e->accept();
			listEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::UpdateFiles:
		{
			e->accept();
			updateFilesEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForRemove:
		{
			e->accept();
			scanForRemoveEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::RemoveFilesCompleted:
		{
			e->accept();
			removeCompleteEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::RemoveFilesCanceled:
		{
			e->accept();
			removeCanceledEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForSize:
		{
			e->accept();
			scanForSizeEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForCopy:
		{
			e->accept();
			scanForCopyEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::CopyFilesCompleted:
		{
			e->accept();
			copyCompleteEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::CopyFilesCanceled:
		{
			e->accept();
			copyCanceledEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForMove:
		{
			e->accept();
			scanForMoveEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::QuestionAnswer:
		{
			e->accept();
			questionAnswerEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::UpdatePerformProgress:
		{
			e->accept();
			updateProgressEvent(static_cast<ModelEvent*>(e)->parameters());
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

int FolderNode::columnCount() const
{
	return columnCount(QModelIndex());
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

IFile *FolderNode::open(IFile::OpenMode mode, QString &error) const
{
	return 0;
}

IFileInfo *FolderNode::create(const QString &fileName, FileType type, QString &error) const
{
	return 0;
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
			static_cast<Node*>(Node::parent())->removeEntry(this);
}

void FolderNode::remove(const QModelIndexList &list)
{
	processIndexList(list, FolderNodeFunctors::callTo(this, &FolderNode::removeFunctor));
}

void FolderNode::calculateSize(const QModelIndexList &list)
{
	processIndexList(list, FolderNodeFunctors::callTo(this, &FolderNode::calculateSizeFunctor));
}

void FolderNode::copy(const QModelIndexList &list, INode *destination)
{
	processIndexList(list, FolderNodeFunctors::callTo(this, &FolderNode::copyFunctor, destination));
}

void FolderNode::move(const QModelIndexList &list, INode *destination)
{
	processIndexList(list, FolderNodeFunctors::callTo(this, &FolderNode::moveFunctor, destination));
}

void FolderNode::viewParent(INodeView *nodeView)
{
	if (!m_info.isRoot())
		if (exists())
			switchTo(static_cast<Node*>(Node::parent()), nodeView, m_parentEntryIndex);
		else
			static_cast<Node*>(Node::parent())->removeEntry(this);
}

void FolderNode::viewThis(INodeView *nodeView, const QModelIndex &selected)
{
	addView(nodeView);
	nodeView->setNode(this, &m_proxy, &m_delegate);

	if (selected.isValid())
		nodeView->select(selected);
	else
		nodeView->select(rootIndex());
}

void FolderNode::viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<FolderNodeItem*>(index.internalPointer())->isRootItem())
	{
		removeView(nodeView);
		static_cast<Node*>(Node::parent())->viewThis(nodeView, m_parentEntryIndex);
		static_cast<Node*>(Node::parent())->refresh();
	}
	else
		if (!static_cast<FolderNodeEntry*>(index.internalPointer())->isLocked())
		{
			FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(index.internalPointer());
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

				if (value.node)
				{
					removeView(nodeView);
					value.node->viewThis(nodeView, QModelIndex());
					value.node->refresh();
				}
			}
			else
				removeEntry(index);
		}
}

void FolderNode::viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins)
{
	Values::size_type index = m_items.indexOf(*path);

	if (index == Values::InvalidIndex)
	{
		Info info(absoluteFilePath(*path));

		if (Node *node = createNode(info, plugins))
		{
			removeView(nodeView);
			m_items.add(Values::Value(new FolderNodeEntry(info), node));

			if ((++path).atEnd())
				node->viewThis(nodeView, QModelIndex());
			else
				node->viewChild(nodeView, path, plugins);
		}
		else
			if (info.isFile())
			{
				FolderNodeItem *item;

				m_items.add(Values::Value(item = new FolderNodeEntry(info)));
				viewThis(nodeView, indexForFile(item, m_items.lastIndex()));
			}
			else
				viewThis(nodeView, QModelIndex());
	}
	else
	{
		Values::Value &value = m_items[index];

		if (value.node || (value.node = createNode(*value.item, plugins)))
		{
			removeView(nodeView);

			if ((++path).atEnd())
				value.node->viewThis(nodeView, QModelIndex());
			else
				value.node->viewChild(nodeView, path, plugins);
		}
		else
			if (value.item->isFile())
				viewThis(nodeView, indexForFile(value.item, m_items.lastIndex()));
			else
				viewThis(nodeView, QModelIndex());
	}
}

void FolderNode::viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins)
{
	Path path(absoluteFilePath);

	if (path.isAbsolute())
	{
		Node *node = this;
		removeView(nodeView);

		while (node->parent())
			node = static_cast<Node*>(node->parent());

		node->viewChild(nodeView, path.begin(), plugins);
	}
	else
		viewChild(nodeView, path.begin(), plugins);
}

void FolderNode::removeThis()
{
	if (!m_info.isRoot())
	{
		Node *parent = static_cast<Node*>(Node::parent());

		while (!parent->exists())
			parent = static_cast<Node*>(parent->Node::parent());

		switchTo(parent, QModelIndex());
	}
}

void FolderNode::switchTo(Node *node, const QModelIndex &selected)
{
	Node *child;

	for (SetView::iterator it = m_view.begin(), end = m_view.end(); it != end; it = m_view.erase(it))
		node->viewThis(*it, selected);

	for (Values::size_type i = 0, size = m_items.size(); i < size; ++i)
		if (child = m_items.at(i).node)
			child->switchTo(node, selected);
}

void FolderNode::removeEntry(Node *entry)
{
	removeEntry(m_items.indexOf(entry));
}

void FolderNode::processIndexList(const QModelIndexList &list, const FolderNodeFunctors::Functor &functor)
{
	QModelIndex index;
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
					functor(entry);
				else
					removeEntry(index);
			}
		}
}

void FolderNode::removeFunctor(FolderNodeItem *entry)
{
	if (entry->isDir())
		scanForRemove(entry);
	else
		removeEntry(entry);
}

void FolderNode::calculateSizeFunctor(FolderNodeItem *entry)
{
	if (entry->isDir())
		scanForSize(entry);
	else
		updateSecondColumn(entry);
}

void FolderNode::copyFunctor(FolderNodeItem *entry, INode *destination)
{
	if (entry->isDir())
		scanForCopy(entry, destination);
	else
		if (entry->isFile())
			copyEntry(entry, destination);
}

void FolderNode::moveFunctor(FolderNodeItem *entry, INode *destination)
{
	if (entry->isDir())
		scanForMove(entry, destination);
	else
		if (entry->isFile())
			moveEntry(entry, destination);
}

void FolderNode::list(FolderNodeItem *fileSystemTree)
{

}

void FolderNode::listEvent(const ModelEvent::Params *p)
{

}

void FolderNode::updateFiles()
{
	UpdatesList::Map changes;
	QScopedPointer<UpdateFilesTask> task(new UpdateFilesTask());

	for (Values::size_type i = m_info.isRoot() ? 0 : 1, size = m_items.size(); i < size; ++i)
		changes.insert(m_items.at(i).item->fileName(), *m_items.at(i).item);

	task->parameters()->node = this;
	task->parameters()->updates = changes;

	setUpdating(true);
	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::updateFilesEvent(const ModelEvent::Params *p)
{
	typedef const UpdateFilesTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	UpdatesList updates = params->updates;
	RangeIntersection updateRange(1);
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
						 createIndex(updateRange.at(i).bottom(), columnCount() - 1));

	if (!updates.isEmpty())
	{
		UpdatesList::Values added = updates.values();

		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + added.size() - 1);
		for (UpdatesList::Values::size_type i = 0, size = added.size(); i < size; ++i)
			m_items.add(new FolderNodeEntry(added.at(i).info()));
		endInsertRows();
	}

	if (params->isLastEvent)
		setUpdating(false);
}

void FolderNode::removeEntry(FolderNodeItem *entry)
{
	if (QMessageBox::question(
			&Application::instance()->mainWindow(),
			tr("Remove file..."),
			tr("Would you like to remove \"%1\" file?").arg(static_cast<FolderNodeEntry*>(entry)->absoluteFilePath()),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
//		QScopedPointer<PerformRemoveEntryTask::Params> params(new PerformRemoveEntryTask::Params());
//		params->source.object = (QObject*)this;
//		params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
//		params->source.entry = static_cast<FileSystemEntry*>(entry);
//
//		static_cast<FileSystemEntry*>(entry)->lock(tr("Removing..."));
//		updateFirstColumn(fileSystemTree, entry);
//
//		Application::instance()->taskPool().handle(new PerformRemoveEntryTask(params.take()));
	}
}

void FolderNode::scanForRemove(FolderNodeItem *entry)
{
	QScopedPointer<ScanFilesForRemoveTask> task(new ScanFilesForRemoveTask());
	task->parameters()->source.node = this;
	task->parameters()->source.entry = static_cast<FolderNodeEntry*>(entry);

	static_cast<FolderNodeEntry*>(entry)->lock(tr("Scanning folder for remove..."));
	updateFirstColumn(entry);

	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::scanForRemoveEvent(const ModelEvent::Params *p)
{
	typedef ScanFilesForRemoveTask::Event::Params *ParamsType;
	ParamsType params = (ParamsType)p;

	if (QMessageBox::question(
			&Application::instance()->mainWindow(),
			tr("Remove directory..."),
			tr("Would you like to remove \"%1\" directory (%2)?").
				arg(params->snapshot.entry->absoluteFilePath()).
				arg(FolderNodeEntry::humanReadableSize(params->size)),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		params->snapshot.entry->lock(tr("Removing..."), params->size);
		updateSecondColumn(params->snapshot.entry);
		Application::instance()->taskPool().handle(new PerformRemoveTreeTask(this, *params));
	}
	else
	{
		params->snapshot.entry->setTotalSize(params->size);
		params->snapshot.entry->unlock();
		updateBothColumns(params->snapshot.entry);
	}
}

void FolderNode::removeCompleteEvent(const ModelEvent::Params *p)
{
	typedef const PerformRemoveEntryTask::CompletedEvent::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	if (params->removeParentEntry && !isUpdating())
		removeEntry(m_items.indexOf(params->snapshot.entry));
	else
	{
		params->snapshot.entry->clearTotalSize();
		params->snapshot.entry->unlock();
		updateBothColumns(params->snapshot.entry);
	}
}

void FolderNode::removeCanceledEvent(const ModelEvent::Params *p)
{
	typedef const PerformRemoveEntryTask::CanceledEvent::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	params->snapshot.entry->clearTotalSize();
	params->snapshot.entry->unlock();
	updateBothColumns(params->snapshot.entry);
}

void FolderNode::scanForSize(FolderNodeItem *entry)
{
	QScopedPointer<ScanFilesForSizeTask> task(new ScanFilesForSizeTask());
	task->parameters()->source.node = this;
	task->parameters()->source.entry = static_cast<FolderNodeEntry*>(entry);

	static_cast<FolderNodeEntry*>(entry)->lock(tr("Scanning folder for size..."));
	updateFirstColumn(entry);

	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::scanForSizeEvent(const ModelEvent::Params *p)
{
	typedef const ScanFilesForSizeTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	Values::size_type index = m_items.indexOf(params->snapshot.entry);

	if (index != Values::InvalidIndex)
	{
//		m_items[index].node = params->subnode;
		params->snapshot.entry->setTotalSize(params->size);
		params->snapshot.entry->unlock();
		updateBothColumns(params->snapshot.entry);
	}
//	else
//		delete params->subnode;
}

void FolderNode::copyEntry(FolderNodeItem *entry, INode *destination)
{
//	QScopedPointer<PerformCopyEntryTask::Params> params(new PerformCopyEntryTask::Params());
//	params->source.object = (QObject*)this;
//	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
//	params->source.entry = static_cast<FileSystemEntry*>(entry);
//	params->destination.object = destination;
//	params->destination.fileSystemTree = static_cast<FileSystemTree*>(destination->m_currentFsTree);
//	params->removeSource = false;
//
//	static_cast<FileSystemEntry*>(entry)->lock(tr("Copying..."), params->source.entry->fileInfo().size());
//	updateFirstColumn(fileSystemTree, entry);
//
//	Application::instance()->taskPool().handle(new PerformCopyEntryTask(params.take()));
}

void FolderNode::scanForCopy(FolderNodeItem *entry, INode *destination)
{
	QScopedPointer<ScanFilesForCopyTask> task(new ScanFilesForCopyTask());
	task->parameters()->source.node = this;
	task->parameters()->source.entry = static_cast<FolderNodeEntry*>(entry);
//	task->parameters()->destination.node = destination;

	static_cast<FolderNodeEntry*>(entry)->lock(tr("Scanning folder for copy..."));
	updateFirstColumn(entry);

	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::moveEntry(FolderNodeItem *entry, INode *destination)
{
//	QScopedPointer<PerformCopyEntryTask::Params> params(new PerformCopyEntryTask::Params());
//	params->source.object = (QObject*)this;
//	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
//	params->source.entry = static_cast<FileSystemEntry*>(entry);
//	params->destination.object = destination;
//	params->destination.fileSystemTree = static_cast<FileSystemTree*>(destination->m_currentFsTree);
//	params->removeSource = true;
//
//	static_cast<FileSystemEntry*>(entry)->lock(tr("Moving..."), params->source.entry->fileInfo().size());
//	updateFirstColumn(fileSystemTree, entry);
//
//	Application::instance()->taskPool().handle(new PerformCopyEntryTask(params.take()));
}

void FolderNode::scanForMove(FolderNodeItem *entry, INode *destination)
{
	QScopedPointer<ScanFilesForMoveTask> task(new ScanFilesForMoveTask());
	task->parameters()->source.node = this;
	task->parameters()->source.entry = static_cast<FolderNodeEntry*>(entry);
//	task->parameters()->destination.node = destination;

	static_cast<FolderNodeEntry*>(entry)->lock(tr("Scanning folder for move..."));
	updateFirstColumn(entry);

	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::scanForCopyEvent(const ModelEvent::Params *p)
{
	typedef const ScanFilesForCopyTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	Values::size_type index = m_items.indexOf(params->snapshot.entry);

	if (index != Values::InvalidIndex)
	{
//		m_items[index].node = params->subnode;
		params->snapshot.entry->lock(tr("Copying..."), params->size);
		updateSecondColumn(params->snapshot.entry);

//		Application::instance()->taskPool().handle(new PerformCopyTreeTask(new PerformCopyTreeTask::Params((QObject*)this, *params, false)));
	}
//	else
//		delete params->subnode;
}

void FolderNode::scanForMoveEvent(const ModelEvent::Params *p)
{
	typedef const ScanFilesForMoveTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	Values::size_type index = m_items.indexOf(params->snapshot.entry);

	if (index != Values::InvalidIndex)
	{
//		m_items[index].node = params->subnode;
		params->snapshot.entry->lock(tr("Moving..."), params->size);
		updateSecondColumn(params->snapshot.entry);

//		Application::instance()->taskPool().handle(new PerformCopyTreeTask(new PerformCopyTreeTask::Params((QObject*)this, *params, true)));
	}
//	else
//		delete params->subnode;
}

void FolderNode::copyCompleteEvent(const ModelEvent::Params *p)
{

}

void FolderNode::copyCanceledEvent(const ModelEvent::Params *p)
{

}

void FolderNode::questionAnswerEvent(const ModelEvent::Params *p)
{
	typedef ModelEvents::QuestionAnswerParams * ParamsType;
	ParamsType params = (ParamsType)p;

	params->result->lock();
	params->result->setAnswer(QMessageBox::question(&Application::instance()->mainWindow(), params->title, params->question, params->buttons));
	params->result->unlock();
}

void FolderNode::updateProgressEvent(const ModelEvent::Params *p)
{
	typedef const ModelEvents::UpdatePerformProgressEvent::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);

	params->snapshot.entry->setDoneSize(params->progress);
	params->snapshot.entry->setTimeElapsed(params->timeElapsed);
	updateSecondColumn(params->snapshot.entry);
}

QModelIndex FolderNode::index(int column, FolderNodeItem *item) const
{
	int index = m_items.indexOf(item);

	if (index != -1)
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

QModelIndex FolderNode::rootIndex() const
{
	if (m_info.isRoot())
		return QModelIndex();
	else
		return m_proxy.mapFromSource(createIndex(0, 0, m_items.at(0).item));
}

void FolderNode::updateFirstColumn(FolderNodeItem *entry)
{
	QModelIndex index = createIndex(m_items.indexOf(entry), 0, entry);
	emit dataChanged(index, index);
}

void FolderNode::updateSecondColumn(FolderNodeItem *entry)
{
	QModelIndex index = createIndex(m_items.indexOf(entry), 1, entry);
	emit dataChanged(index, index);
}

void FolderNode::updateBothColumns(FolderNodeItem *entry)
{
	Values::size_type index = m_items.indexOf(entry);
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, 1, entry));
}

void FolderNode::removeEntry(Values::size_type index)
{
	beginRemoveRows(QModelIndex(), index, index);

	if (Node *node = m_items.at(index).node)
		node->removeThis();

	m_items.remove(index);

	endRemoveRows();
}

void FolderNode::removeEntry(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());

	if (Node *node = m_items.at(index.row()).node)
		node->removeThis();

	m_items.remove(index.row());

	endRemoveRows();
}

void FolderNode::switchTo(Node *node, INodeView *nodeView, const QModelIndex &selected)
{
	removeView(nodeView);
	node->viewThis(nodeView, selected);
	node->refresh();
}

void FolderNode::addView(INodeView *view)
{
	m_view.insert(view);
}

void FolderNode::removeView(INodeView *view)
{
	m_view.remove(view);
}

FILE_SYSTEM_NS_END
