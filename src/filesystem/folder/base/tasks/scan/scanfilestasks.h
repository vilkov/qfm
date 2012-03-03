#ifndef SCANFILESTASKS_H_
#define SCANFILESTASKS_H_

#include "scanfilestask.h"
#include "../../../../interfaces/filesysteminode.h"
#include "../../../../tasks/interfaces/filesystemicopycontrol.h"


FILE_SYSTEM_NS_BEGIN

/********************************************************************************************************/
class ScanFilesForSizeTask : public ScanFilesTask
{
public:
	ScanFilesForSizeTask(TasksNode *receiver, IFileContainer::Holder &container, const TasksNode::TasksItemList &entries);

	virtual void run(const volatile Flags &aborted);
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask
{
public:
	ScanFilesForRemoveTask(TasksNode *receiver, IFileContainer::Holder &container, const TasksNode::TasksItemList &entries);

	virtual void run(const volatile Flags &aborted);
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesTask
{
public:
	class Event : public ScanFilesTask::Event
	{
	public:
		Event(BaseTask *task, Type type, bool canceled, const Snapshot &snapshot, ICopyControl::Holder &destination, bool move) :
			ScanFilesTask::Event(task, type, canceled, snapshot),
			destination(destination.take()),
			move(move)
		{}

		ICopyControl::Holder destination;
		bool move;
	};

public:
	ScanFilesForCopyTask(TasksNode *receiver, IFileContainer::Holder &container, const TasksNode::TasksItemList &files, ICopyControl::Holder &destination, bool move);

	virtual void run(const volatile Flags &aborted);

private:
	ICopyControl::Holder m_destination;
	bool m_move;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASKS_H_ */
