#include "scanfilestasks.h"
#include "../../../../../application.h"
#include <QDir>
#include <QDirIterator>
#include <QScopedPointer>
#include <windows.h>


ScanFilesForSizeTask::ScanFilesForSizeTask(Params *params) :
	parent_class(params)
{}

void ScanFilesForSizeTask::run(const volatile bool &stopedFlag)
{
	parent_class::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForSize));
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subtree = parameters()->subtree;
		Application::postEvent(parameters()->source.object, event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(Params *params) :
	parent_class(params)
{}

void ScanFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	parent_class::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForRemove));
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subtree = parameters()->subtree;
		Application::postEvent(parameters()->source.object, event.take());
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
		Sleep(3000);
		QScopedPointer<Event> event(new Event(Event::ScanFilesForCopy));
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subtree = parameters()->subtree;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->source.object, event.take());
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
		QScopedPointer<Event> event(new Event(Event::ScanFilesForMove));
		event->params().snapshot = parameters()->source;
		event->params().size = parameters()->size;
		event->params().subtree = parameters()->subtree;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->source.object, event.take());
	}
}
