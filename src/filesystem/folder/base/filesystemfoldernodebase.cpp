#include "filesystemfoldernodebase.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "tasks/perform/performcopytask.h"
#include "tasks/perform/performmovetask.h"
#include "tasks/perform/performremovetask.h"
#include "../../interfaces/imp/filesystemcopycontrol.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderNodeBase::FolderNodeBase(IFileContainer::Holder &container, const NodeModelContainer &items, Node *parent) :
	TasksNode(items, parent),
	m_updating(false),
	m_container(container.take())
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
	return new CopyControl(const_cast<FolderNodeBase *>(this), m_container->location());
}

void FolderNodeBase::scanForSize(const TasksItemList &entries)
{
	PScopedPointer<ScanFilesForSizeTask> task(new ScanFilesForSizeTask(this, m_container.data(), entries));
	addTask(task.take(), entries);
}

void FolderNodeBase::scanForCopy(const TasksItemList &entries, ICopyControl::Holder &destination, bool move)
{
	PScopedPointer<ScanFilesForCopyTask> task(new ScanFilesForCopyTask(this, destination, entries, move));
	addTask(task.take(), entries);
}

void FolderNodeBase::scanForRemove(const TasksItemList &entries)
{
	PScopedPointer<ScanFilesForRemoveTask> task(new ScanFilesForRemoveTask(this, m_container.data(), entries));
	addTask(task.take(), entries);
}

void FolderNodeBase::performCopy(BaseTask *oldTask, const Snapshot &snapshot, ICopyControl::Holder &destination, bool move)
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

void FolderNodeBase::performRemove(BaseTask *oldTask, const Snapshot &snapshot)
{
	PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, snapshot));
	resetTask(task.take(), oldTask);
}

void FolderNodeBase::updateFiles()
{
	if (isVisible())
	{
		IFileContainer::Holder container(new FileContainer(m_container->location()));
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
		performCopy(event->task, event->snapshot, const_cast<NotConstEvent>(event)->destination, event->move);
	else
		removeAllTaskLinks(event->task);
}

void FolderNodeBase::scanForRemove(const BaseTask::Event *e)
{
	typedef ScanFilesForRemoveTask::Event * NotConstEvent;
	typedef const ScanFilesForRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (scanForRemoveEvent(event->canceled, event->snapshot))
		performRemove(event->task, event->snapshot);
	else
		removeAllTaskLinks(event->task);
}

void FolderNodeBase::performCopy(const BaseTask::Event *e)
{
	typedef PerformCopyTask::Event * NotConstEvent;
	typedef const PerformCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (performCopyEvent(event->canceled, event->snapshot, event->move))
		performRemove(event->task, event->snapshot);
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
