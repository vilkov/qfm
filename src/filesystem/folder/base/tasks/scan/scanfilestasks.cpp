#include "scanfilestasks.h"
#include "../../../../../application.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask(TasksNode *receiver, const Info &info, const TasksNode::TasksItemList &entries) :
	ScanFilesTask(receiver, info, entries)
{}

void ScanFilesForSizeTask::run(const volatile bool &aborted)
{
	ScanFilesTask::run(aborted);

	if (!aborted && !isReceiverDead())
	{
		PScopedPointer<Event> event(new Event(ModelEvent::ScanFilesForSize, this, isCanceled(), subnode()));
		Application::postEvent(receiver(), event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(TasksNode *receiver, const Info &info, const TasksNode::TasksItemList &entries) :
	ScanFilesTask(receiver, info, entries)
{}

void ScanFilesForRemoveTask::run(const volatile bool &aborted)
{
	ScanFilesTask::run(aborted);

	if (!aborted && !isReceiverDead())
	{
		PScopedPointer<Event> event(new Event(ModelEvent::ScanFilesForRemove, this, isCanceled(), subnode()));
		Application::postEvent(receiver(), event.take());
	}
}


ScanFilesForCopyTask::ScanFilesForCopyTask(TasksNode *receiver, const Info &info, const TasksNode::TasksItemList &entries, PScopedPointer<ICopyControl> &control, bool move) :
	ScanFilesTask(receiver, info, entries),
	m_control(control.take()),
	m_move(move)
{}

void ScanFilesForCopyTask::run(const volatile bool &aborted)
{
	ScanFilesTask::run(aborted);

	if (!aborted && !isReceiverDead())
	{
		PScopedPointer<Event> event(new Event(ModelEvent::ScanFilesForCopy, this, isCanceled(), subnode(), m_control, m_move));
		Application::postEvent(receiver(), event.take());
	}
}

FILE_SYSTEM_NS_END
