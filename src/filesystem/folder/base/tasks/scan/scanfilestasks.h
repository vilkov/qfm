#ifndef SCANFILESTASKS_H_
#define SCANFILESTASKS_H_

#include "scanfilestask.h"


FILE_SYSTEM_NS_BEGIN

/********************************************************************************************************/
class ScanFilesForSizeTask : public ScanFilesTask
{
public:
	ScanFilesForSizeTask(QObject *receiver, const Info &info, const EntryList &entries);

	virtual void run(const volatile bool &aborted);
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask
{
public:
	ScanFilesForRemoveTask(QObject *receiver, const Info &info, const EntryList &entries);

	virtual void run(const volatile bool &aborted);
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesTask
{
public:
	class Event : public ScanFilesTask::Event
	{
	public:
		Event(Type type, bool canceled, PScopedPointer<FileSystemList> &entries, PScopedPointer<ICopyControl> &control, bool move) :
			ScanFilesTask::Event(type, canceled, entries),
			control(control.take()),
			move(move)
		{}

		PScopedPointer<ICopyControl> control;
		bool move;
	};

public:
	ScanFilesForCopyTask(QObject *receiver, const Info &info, const EntryList &entries, PScopedPointer<ICopyControl> &control, bool move);

	virtual void run(const volatile bool &aborted);

private:
	INode *m_destination;
	PScopedPointer<ICopyControl> m_control;
	bool m_move;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASKS_H_ */
