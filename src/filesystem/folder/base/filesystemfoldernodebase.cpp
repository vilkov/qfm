#include "filesystemfoldernodebase.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "tasks/perform/performcopytask.h"
#include "tasks/perform/performremovetask.h"
#include "../../info/filesystemcopyinfo.h"
#include "../../../application.h"

#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

FolderNodeBase::FolderNodeBase(const Info &info, const FileSystemModelContainer &conteiner, Node *parent) :
	Node(conteiner, parent),
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
			updateFiles(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::ScanFilesForRemove:
		{
			e->accept();
			scanForRemove(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::RemoveFiles:
		{
			e->accept();
			performRemove(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::ScanFilesForSize:
		{
			e->accept();
			scanForSize(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::ScanFilesForCopy:
		{
			e->accept();
			scanForCopy(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::CopyFiles:
		{
			e->accept();
			performCopy(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::QuestionAnswer:
		{
			e->accept();
			questionAnswer(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::UpdateProgress:
		{
			e->accept();
			updateProgress(static_cast<ModelEvent*>(e));
			return true;
		}
		case ModelEvent::CompletedProgress:
		{
			e->accept();
			completedProgress(static_cast<ModelEvent*>(e));
			return true;
		}
		default:
			break;
	}

	return QAbstractItemModel::event(e);
}

ICopyControl *FolderNodeBase::createControl() const
{
	return new CopyInfo(m_info);
}

bool FolderNodeBase::isDir() const
{
	return true;
}

bool FolderNodeBase::isFile() const
{
	return false;
}

bool FolderNodeBase::exists() const
{
	((FolderNodeBase*)this)->m_info.refresh();
	return m_info.exists();
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

void FolderNodeBase::refresh()
{
	if (isUpdating())
		return;

	if (m_info.isRoot())
		updateFiles();
	else
		if (exists())
			updateFiles();
		else
			viewCloseAll();
}

void FolderNodeBase::removeChild(Node *node)
{

}

void FolderNodeBase::scanForSize(const QStringList &entries)
{
	PScopedPointer<ScanFilesForSizeTask> task(new ScanFilesForSizeTask(this, m_info, entries));
	m_tasks.add(task.data(), entries);
	addLink();
	Application::instance()->taskPool().handle(task.take());
}

void FolderNodeBase::scanForCopy(const QStringList &entries, PScopedPointer<ICopyControl> &control, bool move)
{
	PScopedPointer<ScanFilesForCopyTask> task(new ScanFilesForCopyTask(this, m_info, entries, control, move));
	m_tasks.add(task.data(), entries);
	addLink();
	Application::instance()->taskPool().handle(task.take());
}

void FolderNodeBase::scanForRemove(const QStringList &entries)
{
	PScopedPointer<ScanFilesForRemoveTask> task(new ScanFilesForRemoveTask(this, m_info, entries));
	m_tasks.add(task.data(), entries);
	addLink();
	Application::instance()->taskPool().handle(task.take());
}

void FolderNodeBase::performCopy(PScopedPointer<InfoListItem> &entries, PScopedPointer<ICopyControl> &control, bool move)
{
	InfoItem *entry = entries->at(0);
	PScopedPointer<PerformCopyTask> task(new PerformCopyTask(this, entries, control, move));
	m_tasks.resetTask(task.data(), entry->fileName());
	addLink();
	Application::instance()->taskPool().handle(task.take());
}

void FolderNodeBase::performRemove(PScopedPointer<InfoListItem> &entries)
{
	InfoItem *entry = entries->at(0);
	PScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, entries));
	m_tasks.resetTask(task.data(), entry->fileName());
	addLink();
	Application::instance()->taskPool().handle(task.take());
}

//void FolderNodeBase::add(FileSystemBaseItem *item)
//{
//	m_items.m_container.add(item->fileName(), item);
//	m_nodes.push_back(item->node());
//}
//
//void FolderNodeBase::remove(FileSystemBaseItem *item)
//{
//
//}
//
//void FolderNodeBase::remove(ItemsContainer::size_type index)
//{
//
//}

void FolderNodeBase::updateFiles()
{
	if (isVisible())
	{
		PScopedPointer<UpdateFilesTask> task(new UpdateFilesTask(this, m_info, updateFilesMap()));
		setUpdating(true);
		addLink();
		Application::instance()->taskPool().handle(task.take());
	}
}

void FolderNodeBase::updateFiles(const ModelEvent *e)
{
	typedef const UpdateFilesTask::Event * Event;
	Event event = static_cast<Event>(e);

	updateFilesEvent(event->updates);

	if (event->isLastEvent)
	{
		removeLink();
		setUpdating(false);
	}
}

void FolderNodeBase::scanForSize(const ModelEvent *e)
{
	typedef ScanFilesForSizeTask::Event * NotConstEvent;
	typedef const ScanFilesForSizeTask::Event * Event;
	Event event = static_cast<Event>(e);

	removeLink();
	scanForSizeEvent(event->canceled, const_cast<NotConstEvent>(event)->entries);
}

void FolderNodeBase::scanForCopy(const ModelEvent *e)
{
	typedef ScanFilesForCopyTask::Event * NotConstEvent;
	typedef const ScanFilesForCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	removeLink();
	scanForCopyEvent(event->canceled, const_cast<NotConstEvent>(event)->entries, const_cast<NotConstEvent>(event)->control, event->move);
}

void FolderNodeBase::scanForRemove(const ModelEvent *e)
{
	typedef ScanFilesForRemoveTask::Event * NotConstEvent;
	typedef const ScanFilesForRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	removeLink();
	scanForRemoveEvent(event->canceled, const_cast<NotConstEvent>(event)->entries);
}

void FolderNodeBase::performCopy(const ModelEvent *e)
{
	typedef PerformCopyTask::Event * NotConstEvent;
	typedef const PerformCopyTask::Event * Event;
	Event event = static_cast<Event>(e);

	removeLink();
	performCopyEvent(event->canceled, const_cast<NotConstEvent>(event)->entries, event->move);
}

void FolderNodeBase::performRemove(const ModelEvent *e)
{
	typedef PerformRemoveTask::Event * NotConstEvent;
	typedef const PerformRemoveTask::Event * Event;
	Event event = static_cast<Event>(e);

	removeLink();
	performRemoveEvent(const_cast<NotConstEvent>(event)->entries);
}

void FolderNodeBase::questionAnswer(const ModelEvent *e)
{
	typedef BaseTask::QuestionAnswerEvent * NotConstEvent;
	typedef const BaseTask::QuestionAnswerEvent * Event;
	Event event = static_cast<Event>(e);

	event->result()->lock();
	event->result()->setAnswer(QMessageBox::question(&Application::instance()->mainWindow(), event->title(), event->question(), event->buttons()));
	event->result()->unlock();
}

void FolderNodeBase::updateProgress(const ModelEvent *e)
{
	typedef PerformTask::UpdateProgressEvent * NotConstEvent;
	typedef const PerformTask::UpdateProgressEvent * Event;
	Event event = static_cast<Event>(e);

	updateProgressEvent(event->fileName, event->progress, event->timeElapsed);
}

void FolderNodeBase::completedProgress(const ModelEvent *e)
{
	typedef PerformTask::CompletedProgressEvent * NotConstEvent;
	typedef const PerformTask::CompletedProgressEvent * Event;
	Event event = static_cast<Event>(e);

	completedProgressEvent(event->fileName, event->timeElapsed);
}

FILE_SYSTEM_NS_END
