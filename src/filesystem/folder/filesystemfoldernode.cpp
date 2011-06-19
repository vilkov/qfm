#include "filesystemfoldernode.h"
#include "items/filesystemfoldernoderoot.h"
#include "items/filesystemfoldernodeentry.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "tasks/perform/performremovetreetask.h"
#include "../filesystempluginsmanager.h"
#include "../../application.h"
#include <QtGui/QMessageBox>


#define START_PROCESS_EVENT(Type, Var)                                             \
	struct LocalValueCast                                                          \
	{                                                                              \
		LocalValueCast(Values::Value &value) :                                     \
			m_value(value)                                                         \
		{}                                                                         \
		                                                                           \
		inline Node *node()                                                        \
		{ return m_value.node; }                                                   \
                                                                                   \
		inline FolderNodeEntry *entry()                                            \
		{ return static_cast<FolderNodeEntry*>(m_value.item); }                    \
                                                                                   \
	private:                                                                       \
		Values::Value &m_value;                                                    \
	};                                                                             \
	                                                                               \
	Type *params = (Type *)Var;                                                    \
	Values::size_type index = m_items.indexOf(params->snapshot.entry->fileName()); \
                                                                                   \
	if (index != Values::InvalidIndex)                                             \
	{                                                                              \
		LocalValueCast value(m_items[index]);

#define END_PROCESS_EVENT }


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

bool FolderNode::exists(IFileInfo *info) const
{
	return m_info.exists(info);
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

IFile *FolderNode::open(IFile::OpenMode mode, QString &error) const
{
	return m_info.open(mode, error);
}

void FolderNode::close(IFile *file) const
{
	m_info.close(file);
}

IFileInfo *FolderNode::create(IFileInfo *info, QString &error) const
{
	return m_info.create(info, error);
}

IFileInfo *FolderNode::create(const QString &fileName, FileType type, QString &error) const
{
	return m_info.create(fileName, type, error);
}

void FolderNode::close(IFileInfo *info) const
{
	m_info.close(info);
}

void FolderNode::remove(const QModelIndexList &list)
{
	ProcessedList entries = processIndexList(list);

	if (!entries.isEmpty())
	{

	}
}

void FolderNode::calculateSize(const QModelIndexList &list)
{
	ProcessedList entries = processIndexList(list);

	if (!entries.isEmpty())
	{

	}
}

void FolderNode::copy(const QModelIndexList &list, INode *destination)
{
	ProcessedList entries = processIndexList(list);

	if (!entries.isEmpty())
	{

	}
}

void FolderNode::move(const QModelIndexList &list, INode *destination)
{
	ProcessedList entries = processIndexList(list);

	if (!entries.isEmpty())
	{

	}
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
				viewThis(nodeView, indexForFile(value.item, index));
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
	Node *parent = static_cast<Node*>(Node::parent());

	while (!parent->exists())
		parent = static_cast<Node*>(parent->Node::parent());

	switchTo(parent, QModelIndex());
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

FolderNode::ProcessedList FolderNode::processIndexList(const QModelIndexList &list)
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
					res.push_back(ProcessedValue(index.row(), entry));
				else
					removeEntry(index);
			}
		}

	return res;
}

//void FolderNode::addFunctor(Functors::ProcessedList &list, Values::size_type index, FolderNodeItem *entry)
//{
//	list.push_back(Functors::ProcessedValue(index, entry));
//}
//
//void FolderNode::removeFunctor(Functors::ProcessedList &list, Values::size_type index, FolderNodeItem *entry)
//{
//	if (entry->isDir())
//		scanForRemove(entry);
//	else
//		removeEntry(entry);
//}
//
//void FolderNode::calculateSizeFunctor(Functors::ProcessedList &list, Values::size_type index, FolderNodeItem *entry)
//{
//	if (entry->isDir())
//		scanForSize(entry);
//	else
//		updateSecondColumn(entry);
//}
//
//void FolderNode::copyFunctor(Functors::ProcessedList &list, Values::size_type index, FolderNodeItem *entry, INode *destination)
//{
//	if (entry->isDir())
//		scanForCopy(entry, destination, false);
//	else
//		if (entry->isFile())
//			copyEntry(entry, destination, false);
//}
//
//void FolderNode::moveFunctor(Functors::ProcessedList &list, Values::size_type index, FolderNodeItem *entry, INode *destination)
//{
//	if (entry->isDir())
//		scanForCopy(entry, destination, true);
//	else
//		if (entry->isFile())
//			copyEntry(entry, destination, true);
//}

void FolderNode::list(FolderNodeItem *fileSystemTree)
{

}

void FolderNode::listEvent(const ModelEvent::Params *p)
{

}

void FolderNode::updateFiles()
{
	if (isVisible())
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
}

void FolderNode::updateFilesEvent(const ModelEvent::Params *p)
{
	typedef const UpdateFilesTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	UpdatesList updates = params->updates;
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
		static_cast<FolderNodeEntry*>(entry)->lock(tr("Removing..."));
		updateFirstColumn(entry);

		Application::instance()->taskPool().handle(new PerformRemoveEntryTask(this, static_cast<FolderNodeEntry*>(entry)));
	}
}

void FolderNode::scanForRemove(const ProcessedList &entries)
{
	QStringList list;
	FolderNodeEntry *entry;
	RangeIntersection updateRange;

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
	{
		if ((entry = static_cast<FolderNodeEntry*>(entries.at(i).second))->isDir())
			entry->lock(tr("Scanning folder for remove..."));
		else
			entry->lock(tr("Removing..."));

		updateRange.add(entries.at(i).first, entries.at(i).first);
		list.push_back(entry->fileName());
	}

	QScopedPointer<ScanFilesForRemoveTask> task(new ScanFilesForRemoveTask(this, m_info, list));
	updateFirstColumn(updateRange);
	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::scanForRemoveEvent(const ModelEvent::Params *p)
{
	START_PROCESS_EVENT(ScanFilesForRemoveTask::Event::Params, p);
		if (QMessageBox::question(
				&Application::instance()->mainWindow(),
				tr("Remove directory..."),
				tr("Would you like to remove \"%1\" directory (%2)?").
					arg(params->snapshot.entry->absoluteFilePath()).
					arg(FolderNodeEntry::humanReadableSize(params->size)),
				QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			value.entry()->lock(tr("Removing..."), params->size);
			updateSecondColumn(index, value.entry());
			Application::instance()->taskPool().handle(new PerformRemoveTreeTask(this, *params));
		}
		else
		{
			value.entry()->setTotalSize(params->size);
			value.entry()->unlock();
			updateBothColumns(index, value.entry());
		}
	END_PROCESS_EVENT;
}

void FolderNode::removeCompleteEvent(const ModelEvent::Params *p)
{
	START_PROCESS_EVENT(const PerformRemoveEntryTask::CompletedEvent::Params, p);
		if (params->removeParentEntry)
			removeEntry(index);
		else
		{
			value.entry()->clearTotalSize();
			value.entry()->unlock();
			updateBothColumns(index, value.entry());
		}
	END_PROCESS_EVENT;
}

void FolderNode::removeCanceledEvent(const ModelEvent::Params *p)
{
	START_PROCESS_EVENT(const PerformRemoveEntryTask::CanceledEvent::Params, p);
		value.entry()->clearTotalSize();
		value.entry()->unlock();
		updateBothColumns(index, value.entry());
	END_PROCESS_EVENT;
}

void FolderNode::scanForSize(const ProcessedList &entries)
{
	QStringList list;
	FolderNodeEntry *entry;
	RangeIntersection updateRange;

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		if ((entry = static_cast<FolderNodeEntry*>(entries.at(i).second))->isDir())
		{
			entry->lock(tr("Scanning folder for size..."));
			updateRange.add(entries.at(i).first, entries.at(i).first);
			list.push_back(entry->fileName());
		}

	QScopedPointer<ScanFilesForSizeTask> task(new ScanFilesForSizeTask(this, m_info, list));
	updateFirstColumn(updateRange);
	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::scanForSizeEvent(const ModelEvent::Params *p)
{
	START_PROCESS_EVENT(const ScanFilesForSizeTask::Event::Params, p);
		value.entry()->setTotalSize(params->size);
		value.entry()->unlock();
		updateBothColumns(index, value.entry());
	END_PROCESS_EVENT;
}

void FolderNode::copyEntry(FolderNodeItem *entry, INode *destination, bool move)
{
	if (move)
		static_cast<FolderNodeEntry*>(entry)->lock(tr("Moving..."), entry->size());
	else
		static_cast<FolderNodeEntry*>(entry)->lock(tr("Copying..."), entry->size());

	updateFirstColumn(entry);

	Application::instance()->taskPool().handle(new PerformCopyEntryTask(this, static_cast<FolderNodeEntry*>(entry), destination, move));
}

void FolderNode::scanForCopy(const ProcessedList &entries, INode *destination, bool move)
{
	QStringList list;
	FolderNodeEntry *entry;
	RangeIntersection updateRange;
	QString fileLockReason = move ? tr("Moving...") : tr("Copying...");
	QString folderLockReason = move ? tr("Scanning folder for move...") : tr("Scanning folder for copy...");

	list.reserve(entries.size());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
	{
		if ((entry = static_cast<FolderNodeEntry*>(entries.at(i).second))->isDir())
			entry->lock(folderLockReason);
		else
			entry->lock(fileLockReason);

		updateRange.add(entries.at(i).first, entries.at(i).first);
		list.push_back(entry->fileName());
	}

	QScopedPointer<ScanFilesForCopyTask> task(new ScanFilesForCopyTask(this, m_info, list, destination, false));
	updateFirstColumn(updateRange);
	Application::instance()->taskPool().handle(task.take());
}

void FolderNode::scanForCopyEvent(const ModelEvent::Params *p)
{
	START_PROCESS_EVENT(ScanFilesForCopyTask::Event::Params, p);
		if (params->move)
			value.entry()->lock(tr("Moving..."), params->size);
		else
			value.entry()->lock(tr("Copying..."), params->size);

		updateSecondColumn(index, value.entry());

		Application::instance()->taskPool().handle(new PerformCopyTreeTask(this, *params, params->move));
	END_PROCESS_EVENT;
}

void FolderNode::copyCompleteEvent(const ModelEvent::Params *p)
{
	START_PROCESS_EVENT(const PerformCopyEntryTask::CompletedEvent::Params, p);
		if (params->canceled)
		{
			value.entry()->unlock();
			updateBothColumns(index, value.entry());
		}
		else
		{
			if (params->removeSource)
			{
				if (value.entry()->exists())
					if (value.entry()->isDir())
					{
						typedef PerformCopyTreeTask::CompletedEvent::Params *ParamsType;
						ParamsType params = (ParamsType)p;

						value.entry()->lock(tr("Removing..."));
						Application::instance()->taskPool().handle(new PerformRemoveTreeTask(this, *params));
					}
					else
					{
						value.entry()->lock(tr("Removing..."));
						Application::instance()->taskPool().handle(new PerformRemoveEntryTask(this, value.entry()));
					}
				else
					removeEntry(index);
			}
			else
			{
				value.entry()->unlock();
				updateBothColumns(index, value.entry());
			}

			params->destination->refresh();
		}
	END_PROCESS_EVENT;
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
	START_PROCESS_EVENT(const ModelEvents::UpdatePerformProgressEvent::Params, p);
		value.entry()->setDoneSize(params->progress);
		value.entry()->setTimeElapsed(params->timeElapsed);
		updateSecondColumn(index, value.entry());
	END_PROCESS_EVENT;
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
	updateFirstColumn(m_items.indexOf(entry), entry);
}

void FolderNode::updateFirstColumn(const RangeIntersection &range)
{
	for (RangeIntersection::RangeList::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0),
						 createIndex(range.at(i).bottom(), columnCount() - 1));
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

void FolderNode::updateSecondColumn(Values::size_type index, FolderNodeItem *entry)
{
	QModelIndex idx = createIndex(index, 1, entry);
	emit dataChanged(idx, idx);
}

void FolderNode::updateBothColumns(FolderNodeItem *entry)
{
	updateBothColumns(m_items.indexOf(entry), entry);
}

void FolderNode::updateBothColumns(Values::size_type index, FolderNodeItem *entry)
{
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

bool FolderNode::isVisible() const
{
	return !m_view.isEmpty();
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
