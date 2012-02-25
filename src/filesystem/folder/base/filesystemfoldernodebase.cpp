#include "filesystemfoldernodebase.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "tasks/perform/performcopytask.h"
#include "tasks/perform/performremovetask.h"
#include "../../info/filesystemcopyinfo.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderNodeBase::FolderNodeBase(const Info &info, const ModelContainer &conteiner, Node *parent) :
	TasksNode(conteiner, parent),
	m_updating(false),
	m_info(info)
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
	return m_info.id();
}

QIcon FolderNodeBase::icon() const
{
	return m_info.icon();
}

QString FolderNodeBase::name() const
{
	return m_info.name();
}

QString FolderNodeBase::description() const
{
	return m_info.description();
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
	return m_info.isLink();
}

bool FolderNodeBase::exists() const
{
	return m_info.exists();
}

IFile::size_type FolderNodeBase::fileSize() const
{
	return m_info.fileSize();
}

QString FolderNodeBase::fileName() const
{
	if (m_info.isRoot())
		return m_info.absoluteFilePath();
	else
		return m_info.fileName();
}

QString FolderNodeBase::absolutePath() const
{
	return m_info.absolutePath();
}

QString FolderNodeBase::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString FolderNodeBase::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime FolderNodeBase::lastModified() const
{
	return m_info.lastModified();
}

int FolderNodeBase::permissions() const
{
	return m_info.permissions();
}

void FolderNodeBase::refresh()
{
	if (isUpdating())
		return;

	if (m_info.isRoot())
		updateFiles();
	else
		if (exists())
			updateFiles();
//		else
//			viewCloseAll();
}

ICopyControl *FolderNodeBase::createControl(INodeView *view) const
{
	return new CopyInfo(m_info);
}

void FolderNodeBase::scanForSize(const TasksItemList &entries)
{
	PScopedPointer<ScanFilesForSizeTask> task(new ScanFilesForSizeTask(this, entries));
	addTask(task.take(), entries);
}

void FolderNodeBase::scanForCopy(const TasksItemList &entries, PScopedPointer<ICopyControl> &control, bool move)
{
	PScopedPointer<ScanFilesForCopyTask> task(new ScanFilesForCopyTask(this, entries, control, move));
	addTask(task.take(), entries);
}

void FolderNodeBase::scanForRemove(const TasksItemList &entries)
{
	PScopedPointer<ScanFilesForRemoveTask> task(new ScanFilesForRemoveTask(this, entries));
	addTask(task.take(), entries);
}

void FolderNodeBase::performCopy(BaseTask *oldTask, const ScanedFiles &entries, PScopedPointer<ICopyControl> &control, bool move)
{
	PScopedPointer<PerformCopyTask> task(new PerformCopyTask(this, entries, control, move));
	resetTask(task.take(), oldTask);
}

void FolderNodeBase::performRemove(BaseTask *oldTask, const ScanedFiles &entries)
{
	PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, entries));
	resetTask(task.take(), oldTask);
}

void FolderNodeBase::updateFiles()
{
	if (isVisible())
	{
		PScopedPointer<UpdateFilesTask> task(new UpdateFilesTask(this, m_info, updateFilesMap()));
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

	scanForSizeEvent(event->canceled, event->files);
	removeAllTaskLinks(event->task);
}

void FolderNodeBase::scanForCopy(const BaseTask::Event *e)
{
	typedef ScanFilesForCopyTask::Event * NotConstEvent;
	typedef const ScanFilesForCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (scanForCopyEvent(event->canceled, event->files, event->control.data(), event->move))
		performCopy(event->task, const_cast<NotConstEvent>(event)->files, const_cast<NotConstEvent>(event)->control, event->move);
	else
		removeAllTaskLinks(event->task);
}

void FolderNodeBase::scanForRemove(const BaseTask::Event *e)
{
	typedef ScanFilesForRemoveTask::Event * NotConstEvent;
	typedef const ScanFilesForRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (scanForRemoveEvent(event->canceled, event->files))
		performRemove(event->task, const_cast<NotConstEvent>(event)->files);
	else
		removeAllTaskLinks(event->task);
}

void FolderNodeBase::performCopy(const BaseTask::Event *e)
{
	typedef PerformCopyTask::Event * NotConstEvent;
	typedef const PerformCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	if (performCopyEvent(event->canceled, event->entries, event->move))
		performRemove(event->task, const_cast<NotConstEvent>(event)->entries);
	else
		removeAllTaskLinks(event->task);
}

void FolderNodeBase::performRemove(const BaseTask::Event *e)
{
	typedef PerformRemoveTask::Event * NotConstEvent;
	typedef const PerformRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	performRemoveEvent(event->canceled, event->files);
	removeAllTaskLinks(event->task);
}

FILE_SYSTEM_NS_END
