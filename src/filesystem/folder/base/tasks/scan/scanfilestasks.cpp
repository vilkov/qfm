#include "scanfilestasks.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask(TasksNode *receiver, const TasksNode::TasksItemList &entries) :
	ScanFilesTask(receiver, entries)
{}

void ScanFilesForSizeTask::run(const volatile Flags &aborted)
{
	PScopedPointer<Event> event(new Event(this, ModelEvent::ScanFilesForSize));

	event->files = scan(aborted);
	event->canceled = aborted;

	postEvent(event.take());
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(TasksNode *receiver, const TasksNode::TasksItemList &entries) :
	ScanFilesTask(receiver, entries)
{}

void ScanFilesForRemoveTask::run(const volatile Flags &aborted)
{
	PScopedPointer<Event> event(new Event(this, ModelEvent::ScanFilesForRemove));

	event->files = scan(aborted);
	event->canceled = aborted;

	postEvent(event.take());
}


ScanFilesForCopyTask::ScanFilesForCopyTask(TasksNode *receiver, const TasksNode::TasksItemList &entries, PScopedPointer<ICopyControl> &control, bool move) :
	ScanFilesTask(receiver, entries),
	m_control(control.take()),
	m_move(move)
{}

void ScanFilesForCopyTask::run(const volatile Flags &aborted)
{
	PScopedPointer<Event> event(new Event(this, ModelEvent::ScanFilesForCopy, m_control, m_move));

	event->files = scan(aborted);
	event->canceled = aborted;

	postEvent(event.take());
}

FILE_SYSTEM_NS_END
