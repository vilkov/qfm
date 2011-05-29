#include "scanfilestasks.h"
#include "../../../../application.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask(Params *params) :
	parent_class(params)
{}

void ScanFilesForSizeTask::run(const volatile bool &stopedFlag)
{
	parent_class::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subnode = parameters()->subnode;
		Application::postEvent(parameters()->source.node, event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(ParamsPointer &params) :
	parent_class(params.take())
{}

void ScanFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	parent_class::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subnode = parameters()->subnode;
		Application::postEvent(parameters()->source.node, event.take());
	}
}


ScanFilesWithDestinationTask::ScanFilesWithDestinationTask(Params *params) :
	parent_class(params)
{}

ScanFilesForCopyTask::ScanFilesForCopyTask(Params *params) :
	ScanFilesWithDestinationTask(params)
{}

void ScanFilesForCopyTask::run(const volatile bool &stopedFlag)
{
	ScanFilesWithDestinationTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subnode = parameters()->subnode;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->source.node, event.take());
	}
}

ScanFilesForMoveTask::ScanFilesForMoveTask(Params *params) :
	ScanFilesWithDestinationTask(params)
{}

void ScanFilesForMoveTask::run(const volatile bool &stopedFlag)
{
	ScanFilesWithDestinationTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subnode = parameters()->subnode;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->source.node, event.take());
	}
}

FILE_SYSTEM_NS_END
