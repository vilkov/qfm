#include "scanfilestasks.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask(TasksNode *receiver, const TasksNode::TasksItemList &entries) :
	ScanFilesTask(receiver, entries)
{}

void ScanFilesForSizeTask::run(const volatile bool &aborted)
{
	PScopedPointer<Event> event(new Event(ModelEvent::ScanFilesForSize, this));

	event->files = scan(aborted);
	event->canceled = isCanceled();

	if (!aborted && !isReceiverDead())
		postEvent(event.take());
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(TasksNode *receiver, const TasksNode::TasksItemList &entries) :
	ScanFilesTask(receiver, entries)
{}

void ScanFilesForRemoveTask::run(const volatile bool &aborted)
{
	PScopedPointer<Event> event(new Event(ModelEvent::ScanFilesForRemove, this));

	event->files = scan(aborted);
	event->canceled = isCanceled();

	if (!aborted && !isReceiverDead())
		postEvent(event.take());
}


ScanFilesForCopyTask::ScanFilesForCopyTask(TasksNode *receiver, const TasksNode::TasksItemList &entries, PScopedPointer<ICopyControl> &control, bool move) :
	ScanFilesTask(receiver, entries),
	m_control(control.take()),
	m_move(move)
{}

void ScanFilesForCopyTask::run(const volatile bool &aborted)
{
	PScopedPointer<Event> event(new Event(ModelEvent::ScanFilesForCopy, this, m_control, m_move));

	event->files = scan(aborted);
	event->canceled = isCanceled();

	if (!aborted && !isReceiverDead())
		postEvent(event.take());
}

FILE_SYSTEM_NS_END
