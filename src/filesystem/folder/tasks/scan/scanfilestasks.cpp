#include "scanfilestasks.h"
#include "../items/filesystemlist.h"
#include "../items/filesystementry.h"
#include "../../../../application.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask(QObject *receiver, const Info &info, const QStringList &entries) :
	ScanFilesTask(receiver, info, entries)
{}

void ScanFilesForSizeTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().subnode.swap(subnode());
		Application::postEvent(receiver(), event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(QObject *receiver, const Info &info, const QStringList &entries) :
	ScanFilesTask(receiver, info, entries)
{}

void ScanFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().subnode.swap(subnode());
		Application::postEvent(receiver(), event.take());
	}
}


ScanFilesForCopyTask::ScanFilesForCopyTask(QObject *receiver, const Info &info, const QStringList &entries, IFileControl *destination, bool move) :
	ScanFilesTask(receiver, info, entries),
	m_destination(destination),
	m_move(move)
{}

void ScanFilesForCopyTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().subnode.swap(subnode());
		event->params().destination = m_destination;
		event->params().move = m_move;
		Application::postEvent(receiver(), event.take());
	}
}

FILE_SYSTEM_NS_END
