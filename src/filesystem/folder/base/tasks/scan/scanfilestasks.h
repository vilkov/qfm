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
		Event(Type type, bool canceled, PScopedPointer<FileSystemList> &entries, PScopedPointer<IFileControl> &destination, bool move) :
			ScanFilesTask::Event(type, canceled, entries),
			destination(destination.take()),
			move(move)
		{}

		PScopedPointer<IFileControl> destination;
		bool move;
	};

public:
	ScanFilesForCopyTask(QObject *receiver, const Info &info, const EntryList &entries, PScopedPointer<IFileControl> &destination, bool move);

	virtual void run(const volatile bool &aborted);

private:
	PScopedPointer<IFileControl> m_destination;
	bool m_move;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASKS_H_ */
