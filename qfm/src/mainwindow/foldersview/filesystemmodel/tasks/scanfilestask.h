#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include "filestask.h"


/********************************************************************************************************/
class ScanFilesTask : public FilesTask
{
public:
	struct Params : public FilesTask::Params
	{
		quint64 size;
		FileSystemTree *subtree;
	};
	struct EventParams : public FilesTask::EventParams
	{
		quint64 size;
		FileSystemTree *subtree;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ScanFilesTask(Params *params, QObject *controller1);
	ScanFilesTask(Params *params, QObject *controller1, QObject *controller2);
	ScanFilesTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(FilesTask::parameters()); }

private:
	void scan(FileSystemTree *tree, const volatile bool &stopedFlag);
};


/********************************************************************************************************/
class ScanFilesForSizeTask : public ScanFilesTask
{
public:
	ScanFilesForSizeTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask
{
public:
	ScanFilesForRemoveTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);
};


/********************************************************************************************************/
class ScanFilesWithDestinationTask : public ScanFilesTask
{
public:
	struct Params : public ScanFilesTask::Params
	{
		EventListener destination;
	};
	struct EventParams : public ScanFilesTask::EventParams
	{
		Params::EventListener destination;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ScanFilesWithDestinationTask(Params *params, QObject *controller1);
	ScanFilesWithDestinationTask(Params *params, QObject *controller1, QObject *controller2);
	ScanFilesWithDestinationTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3);

protected:
	Params *parameters() const { return static_cast<Params*>(ScanFilesTask::parameters()); }
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesWithDestinationTask
{
public:
	ScanFilesForCopyTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);
};


/********************************************************************************************************/
class ScanFilesForMoveTask : public ScanFilesWithDestinationTask
{
public:
	ScanFilesForMoveTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);
};

#endif /* SCANFILESTASK_H_ */
