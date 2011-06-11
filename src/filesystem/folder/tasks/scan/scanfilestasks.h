#ifndef SCANFILESTASKS_H_
#define SCANFILESTASKS_H_

#include "scanfilestask.h"
#include "../controlabletask.h"
#include "../destcontrolabletask.h"


FILE_SYSTEM_NS_BEGIN

/********************************************************************************************************/
class ScanFilesForSizeTask : public ScanFilesTask<ControlableTask>
{
public:
	typedef ScanFilesTask<ControlableTask>     parent_class;
	typedef parent_class::Params               Params;
	typedef ModelEvents::ScanFilesForSizeEvent Event;

public:
	ScanFilesForSizeTask();

	virtual void run(const volatile bool &stopedFlag);
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask<ControlableTask>
{
public:
	typedef ScanFilesTask<ControlableTask>       parent_class;
	typedef parent_class::Params                 Params;
	typedef ModelEvents::ScanFilesForRemoveEvent Event;

public:
	ScanFilesForRemoveTask();

	virtual void run(const volatile bool &stopedFlag);
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }
};


/********************************************************************************************************/
class ScanFilesWithDestinationTask : public ScanFilesTask<DestControlableTask>
{
public:
	typedef ScanFilesTask<DestControlableTask> parent_class;
	typedef parent_class::Params               Params;

public:
	ScanFilesWithDestinationTask(Params *params);

	Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesWithDestinationTask
{
public:
	struct Params : public parent_class::Params
	{
		bool move;
	};
	typedef ModelEvents::ScanFilesForCopyEvent Event;

public:
	ScanFilesForCopyTask();

	Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }
	virtual void run(const volatile bool &stopedFlag);
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASKS_H_ */
