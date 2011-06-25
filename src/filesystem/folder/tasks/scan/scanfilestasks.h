#ifndef SCANFILESTASKS_H_
#define SCANFILESTASKS_H_

#include "scanfilestask.h"


FILE_SYSTEM_NS_BEGIN

/********************************************************************************************************/
class ScanFilesForSizeTask : public ScanFilesTask
{
public:
	typedef ModelEvents::ScanFilesForSizeEvent Event;

public:
	ScanFilesForSizeTask(QObject *receiver, const Info &info, const QStringList &entries);

	virtual void run(const volatile bool &stopedFlag);
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask
{
public:
	typedef ModelEvents::ScanFilesForRemoveEvent Event;

public:
	ScanFilesForRemoveTask(QObject *receiver, const Info &info, const QStringList &entries);

	virtual void run(const volatile bool &stopedFlag);
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesTask
{
public:
	typedef ModelEvents::ScanFilesForCopyEvent Event;

public:
	ScanFilesForCopyTask(QObject *receiver, const Info &info, const QStringList &entries, IFileControl *destination, bool move);

	virtual void run(const volatile bool &stopedFlag);

private:
	IFileControl *m_destination;
	bool m_move;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASKS_H_ */
