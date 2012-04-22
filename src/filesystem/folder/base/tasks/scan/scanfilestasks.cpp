#include "scanfilestasks.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask(TasksNode *receiver, const IFileContainer *container, const TasksNode::TasksItemList &files) :
	ScanFilesTask(receiver, container, files)
{}

void ScanFilesForSizeTask::run(const volatile Flags &aborted)
{
	Snapshot snapshot = scan(aborted);
	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::ScanFilesForSize), aborted, snapshot));
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(TasksNode *receiver, const IFileContainer *container, const TasksNode::TasksItemList &files) :
	ScanFilesTask(receiver, container, files)
{}

void ScanFilesForRemoveTask::run(const volatile Flags &aborted)
{
	Snapshot snapshot = scan(aborted);
	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::ScanFilesForRemove), aborted, snapshot));
}


ScanFilesForCopyTask::ScanFilesForCopyTask(TasksNode *receiver, ICopyControl::Holder &destination, const TasksNode::TasksItemList &files, bool move) :
	ScanFilesExtendedTask(receiver, destination, files),
	m_move(move)
{}

void ScanFilesForCopyTask::run(const volatile Flags &aborted)
{
	Snapshot snapshot = scan(aborted);
	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::ScanFilesForCopy), destination(), aborted, snapshot, m_move));
}

FILE_SYSTEM_NS_END
