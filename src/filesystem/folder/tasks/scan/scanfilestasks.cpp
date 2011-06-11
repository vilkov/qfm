#include "scanfilestasks.h"
#include "../../../../application.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask() :
	parent_class(new Params())
{}

void ScanFilesForSizeTask::run(const volatile bool &stopedFlag)
{
	parent_class::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subnode.swap(parameters()->subnode);
		Application::postEvent(parameters()->source.node, event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask() :
	parent_class(new Params())
{}

void ScanFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	parent_class::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subnode.swap(parameters()->subnode);
		Application::postEvent(parameters()->source.node, event.take());
	}
}


ScanFilesWithDestinationTask::ScanFilesWithDestinationTask(Params *params) :
	parent_class(params, params->source.node)
{}

ScanFilesForCopyTask::ScanFilesForCopyTask() :
	ScanFilesWithDestinationTask(new Params())
{}

void ScanFilesForCopyTask::run(const volatile bool &stopedFlag)
{
	ScanFilesWithDestinationTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subnode.swap(parameters()->subnode);
		event->params().destination = parameters()->destination;
		event->params().move = parameters()->move;
		Application::postEvent(parameters()->source.node, event.take());
	}
}

FILE_SYSTEM_NS_END
