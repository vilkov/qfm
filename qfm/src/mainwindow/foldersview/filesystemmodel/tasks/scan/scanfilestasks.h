#ifndef SCANFILESTASKS_H_
#define SCANFILESTASKS_H_

#include "scanfilestask.h"
#include "../controlabletask.h"
#include "../destcontrolabletask.h"


/********************************************************************************************************/
class ScanFilesForSizeTask : public ScanFilesTask<ControlableTask>
{
public:
	typedef ScanFilesTask<ControlableTask> parent_class;
	typedef parent_class::Params           Params;
	typedef parent_class::EventParams      EventParams;

public:
	ScanFilesForSizeTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask<ControlableTask>
{
public:
	typedef ScanFilesTask<ControlableTask> parent_class;
	typedef parent_class::Params           Params;
	typedef parent_class::EventParams      EventParams;

public:
	ScanFilesForRemoveTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);
};


/********************************************************************************************************/
class ScanFilesWithDestinationTask : public ScanFilesTask<DestControlableTask>
{
public:
	typedef ScanFilesTask<DestControlableTask> parent_class;
	typedef parent_class::Params               Params;
	typedef parent_class::EventParams          EventParams;

public:
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ScanFilesWithDestinationTask(Params *params);

protected:
	Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesWithDestinationTask
{
public:
	struct Params : public ScanFilesWithDestinationTask::Params
	{};
	struct EventParams : public ScanFilesWithDestinationTask::EventParams
	{};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ScanFilesForCopyTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);
};


/********************************************************************************************************/
class ScanFilesForMoveTask : public ScanFilesWithDestinationTask
{
public:
	struct Params : public ScanFilesWithDestinationTask::Params
	{};
	struct EventParams : public ScanFilesWithDestinationTask::EventParams
	{};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ScanFilesForMoveTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);
};

#endif /* SCANFILESTASKS_H_ */
