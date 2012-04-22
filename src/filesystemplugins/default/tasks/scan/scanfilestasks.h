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
	ScanFilesForSizeTask(TasksNode *receiver, const IFileContainer *container, const TasksNode::TasksItemList &entries);

	virtual void run(const volatile Flags &aborted);
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask
{
public:
	ScanFilesForRemoveTask(TasksNode *receiver, const IFileContainer *container, const TasksNode::TasksItemList &entries);

	virtual void run(const volatile Flags &aborted);
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesExtendedTask
{
public:
	class Event : public ScanFilesExtendedTask::Event
	{
	public:
		Event(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot, bool move) :
			ScanFilesExtendedTask::Event(task, type, destination, canceled, snapshot),
			move(move)
		{}

		bool move;
	};

public:
	ScanFilesForCopyTask(TasksNode *receiver, ICopyControl::Holder &destination, const TasksNode::TasksItemList &files, bool move);

	virtual void run(const volatile Flags &aborted);

private:
	bool m_move;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASKS_H_ */
