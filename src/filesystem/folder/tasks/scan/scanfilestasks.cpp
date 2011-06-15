#include "scanfilestasks.h"
#include "../items/filesystemlist.h"
#include "../items/filesystementry.h"
#include "../../../../application.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask(QObject *listener, const Info &node, const QStringList &entries) :
	parent_class(new Params(listener, node, entries))
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


ScanFilesForRemoveTask::ScanFilesForRemoveTask(QObject *listener, const Info &node, const QStringList &entries) :
	parent_class(new Params(listener, node, entries))
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

ScanFilesForCopyTask::ScanFilesForCopyTask(QObject *listener, const Info &node, const QStringList &entries, INode *destination, bool move) :
	ScanFilesWithDestinationTask(new Params(listener, node, entries, destination, move))
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
