#include "filesystemfoldernodebase.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "tasks/perform/performcopytask.h"
#include "tasks/perform/performmovetask.h"
#include "tasks/perform/performremovetask.h"
#include "../../info/filesystemcopycontrol.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderNodeBase::FolderNodeBase(const Info &info, const ModelContainer &conteiner, Node *parent) :
	TasksNode(conteiner, parent),
	FileContainer(info),
	m_updating(false)
{}

bool FolderNodeBase::event(QEvent *e)
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

FileTypeId FolderNodeBase::id() const
{
	return info().id();
}

QIcon FolderNodeBase::icon() const
{
	return info().icon();
}

QString FolderNodeBase::name() const
{
	return info().name();
}

QString FolderNodeBase::description() const
{
	return info().description();
}

bool FolderNodeBase::isDir() const
{
	return true;
}

bool FolderNodeBase::isFile() const
{
	return false;
}

bool FolderNodeBase::isLink() const
{
	return info().isLink();
}

bool FolderNodeBase::exists() const
{
	return info().exists();
}

FolderNodeBase::size_type FolderNodeBase::fileSize() const
{
	return info().fileSize();
}

QString FolderNodeBase::fileName() const
{
	if (info().isRoot())
		return info().absoluteFilePath();
	else
		return info().fileName();
}

QString FolderNodeBase::absolutePath() const
{
	return info().absolutePath();
}

QString FolderNodeBase::absoluteFilePath() const
{
	return info().absoluteFilePath();
}

QString FolderNodeBase::absoluteFilePath(const QString &fileName) const
{
	return info().absoluteFilePath(fileName);
}

QDateTime FolderNodeBase::lastModified() const
{
	return info().lastModified();
}

int FolderNodeBase::permissions() const
{
	return info().permissions();
}

void FolderNodeBase::refresh()
{
	if (isUpdating())
		return;

	if (info().isRoot())
		updateFiles();
	else
		if (exists())
			updateFiles();
//		else
//			viewCloseAll();
}

ICopyControl *FolderNodeBase::createControl(INodeView *view) const
{
	return new CopyControl(info());
}

void FolderNodeBase::scanForSize(const TasksItemList &entries)
{
	IFileContainer::Holder container(new FileContainer(*this));
	PScopedPointer<ScanFilesForSizeTask> task(new ScanFilesForSizeTask(this, container, entries));
	addTask(task.take(), entries);
}

void FolderNodeBase::scanForCopy(const TasksItemList &entries, ICopyControl::Holder &destination, bool move)
{
	IFileContainer::Holder container(new FileContainer(*this));
	PScopedPointer<ScanFilesForCopyTask> task(new ScanFilesForCopyTask(this, container, entries, destination, move));
	addTask(task.take(), entries);
}

void FolderNodeBase::scanForRemove(const TasksItemList &entries)
{
	IFileContainer::Holder container(new FileContainer(*this));
	PScopedPointer<ScanFilesForRemoveTask> task(new ScanFilesForRemoveTask(this, container, entries));
	addTask(task.take(), entries);
}

void FolderNodeBase::performCopy(BaseTask *oldTask, const Snapshot &snapshot, IFileContainer::Holder &destination, bool move)
{
	if (destination->isPhysical() && move)
	{
		PScopedPointer<PerformMoveTask> task(new PerformMoveTask(this, snapshot, destination));
		resetTask(task.take(), oldTask);
	}
	else
	{
		PScopedPointer<PerformCopyTask> task(new PerformCopyTask(this, snapshot, destination, move));
		resetTask(task.take(), oldTask);
	}
}

void FolderNodeBase::performRemove(BaseTask *oldTask, const Snapshot &snapshot)
{
	PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, snapshot));
	resetTask(task.take(), oldTask);
}

void FolderNodeBase::updateFiles()
{
	if (isVisible())
	{
		IFileContainer::Holder container(new FileContainer(*this));
		PScopedPointer<UpdateFilesTask> task(new UpdateFilesTask(this, container, updateFilesMap()));
		setUpdating(true);
		handleTask(task.take());
	}
}

void FolderNodeBase::updateFiles(const BaseTask::Event *e)
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

void FolderNodeBase::scanForSize(const BaseTask::Event *e)
{
	typedef ScanFilesForSizeTask::Event * NotConstEvent;
	typedef const ScanFilesForSizeTask::Event * Event;
	Event event = static_cast<Event>(e);

	scanForSizeEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

void FolderNodeBase::scanForCopy(const BaseTask::Event *e)
{
	typedef ScanFilesForCopyTask::Event * NotConstEvent;
	typedef const ScanFilesForCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (scanForCopyEvent(event->canceled, event->snapshot, event->destination.data(), event->move))
		performCopy(event->task, const_cast<NotConstEvent>(event)->files, const_cast<NotConstEvent>(event)->control, event->move);
	else
		removeAllTaskLinks(event->task);
}

void FolderNodeBase::scanForRemove(const BaseTask::Event *e)
{
	typedef ScanFilesForRemoveTask::Event * NotConstEvent;
	typedef const ScanFilesForRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (scanForRemoveEvent(event->canceled, event->snapshot))
		performRemove(event->task, const_cast<NotConstEvent>(event)->files);
	else
		removeAllTaskLinks(event->task);
}

void FolderNodeBase::performCopy(const BaseTask::Event *e)
{
	typedef PerformCopyTask::Event * NotConstEvent;
	typedef const PerformCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (performCopyEvent(event->canceled, event->snapshot, event->move))
		performRemove(event->task, const_cast<NotConstEvent>(event)->files);
	else
		removeAllTaskLinks(event->task);
}

void FolderNodeBase::performRemove(const BaseTask::Event *e)
{
	typedef PerformRemoveTask::Event * NotConstEvent;
	typedef const PerformRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	performRemoveEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

FILE_SYSTEM_NS_END
