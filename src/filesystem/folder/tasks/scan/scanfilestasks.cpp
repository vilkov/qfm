#include "scanfilestasks.h"
#include "../items/filesystemlist.h"
#include "../items/filesystementry.h"
#include "../../../../application.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask(QObject *listener, const Info &node, const QStringList &entries) :
	ScanFilesTask(new Params(listener, node, entries), listener)
{}

void ScanFilesForSizeTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().subnode.swap(parameters()->subnode);
		Application::postEvent(parameters()->source.listener, event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(QObject *listener, const Info &node, const QStringList &entries) :
	ScanFilesTask(new Params(listener, node, entries),  listener)
{}

void ScanFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().subnode.swap(parameters()->subnode);
		Application::postEvent(parameters()->source.listener, event.take());
	}
}


ScanFilesWithDestinationTask::ScanFilesWithDestinationTask(Params *params) :
	ScanFilesTask(params, params->source.listener)
{}

ScanFilesForCopyTask::ScanFilesForCopyTask(QObject *listener, const Info &node, const QStringList &entries, IFileControl *destination, bool move) :
	ScanFilesWithDestinationTask(new Params(listener, node, entries, destination, move))
{}

void ScanFilesForCopyTask::run(const volatile bool &stopedFlag)
{
	ScanFilesWithDestinationTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().subnode.swap(parameters()->subnode);
		event->params().destination = parameters()->destination;
		event->params().move = parameters()->move;
		Application::postEvent(parameters()->source.listener, event.take());
	}
}

FILE_SYSTEM_NS_END
