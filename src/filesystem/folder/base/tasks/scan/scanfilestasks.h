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
	ScanFilesForSizeTask(TasksNode *receiver, const TasksNode::TasksItemList &entries);

	virtual void run(const volatile Flags &aborted);
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask
{
public:
	ScanFilesForRemoveTask(TasksNode *receiver, const TasksNode::TasksItemList &entries);

	virtual void run(const volatile Flags &aborted);
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesTask
{
public:
	class Event : public ScanFilesTask::Event
	{
	public:
		Event(BaseTask *task, ModelEvent::Type type, PScopedPointer<ICopyControl> &control, bool move) :
			ScanFilesTask::Event(task, type),
			control(control.take()),
			move(move)
		{}

		PScopedPointer<ICopyControl> control;
		bool move;
	};

public:
	ScanFilesForCopyTask(TasksNode *receiver, const TasksNode::TasksItemList &entries, PScopedPointer<ICopyControl> &control, bool move);

	virtual void run(const volatile Flags &aborted);

private:
	INode *m_destination;
	PScopedPointer<ICopyControl> m_control;
	bool m_move;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASKS_H_ */
