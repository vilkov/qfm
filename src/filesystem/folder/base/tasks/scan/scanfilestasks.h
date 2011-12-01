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
	ScanFilesForSizeTask(TasksNode *receiver, const Info &info, const QStringList &entries);

	virtual void run(const volatile bool &aborted);
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask
{
public:
	ScanFilesForRemoveTask(TasksNode *receiver, const Info &info, const QStringList &entries);

	virtual void run(const volatile bool &aborted);
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesTask
{
public:
	class Event : public ScanFilesTask::Event
	{
	public:
		Event(ModelEvent::Type type, bool canceled, PScopedPointer<InfoListItem> &entries, PScopedPointer<ICopyControl> &control, bool move) :
			ScanFilesTask::Event(type, canceled, entries),
			control(control.take()),
			move(move)
		{}

		PScopedPointer<ICopyControl> control;
		bool move;
	};

public:
	ScanFilesForCopyTask(TasksNode *receiver, const Info &info, const QStringList &entries, PScopedPointer<ICopyControl> &control, bool move);

	virtual void run(const volatile bool &aborted);

private:
	INode *m_destination;
	PScopedPointer<ICopyControl> m_control;
	bool m_move;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASKS_H_ */
