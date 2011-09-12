#include "scanfilestasks.h"
#include "../../../../../application.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesForSizeTask::ScanFilesForSizeTask(QObject *receiver, const Info &info, const EntryList &entries) :
	ScanFilesTask(receiver, info, entries)
{}

void ScanFilesForSizeTask::run(const volatile bool &aborted)
{
	ScanFilesTask::run(aborted);

	if (!aborted && !isControllerDead())
	{
		PScopedPointer<Event> event(new Event(Event::ScanFilesForSize, isCanceled(), subnode()));
		Application::postEvent(receiver(), event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(QObject *receiver, const Info &info, const EntryList &entries) :
	ScanFilesTask(receiver, info, entries)
{}

void ScanFilesForRemoveTask::run(const volatile bool &aborted)
{
	ScanFilesTask::run(aborted);

	if (!aborted && !isControllerDead())
	{
		PScopedPointer<Event> event(new Event(Event::ScanFilesForRemove, isCanceled(), subnode()));
		Application::postEvent(receiver(), event.take());
	}
}


ScanFilesForCopyTask::ScanFilesForCopyTask(QObject *receiver, const Info &info, const EntryList &entries, PScopedPointer<ICopyControl> &control, bool move) :
	ScanFilesTask(receiver, info, entries),
	m_control(control.take()),
	m_move(move)
{}

void ScanFilesForCopyTask::run(const volatile bool &aborted)
{
	ScanFilesTask::run(aborted);

	if (!aborted && !isControllerDead())
	{
		PScopedPointer<Event> event(new Event(Event::ScanFilesForCopy, isCanceled(), subnode(), m_control, m_move));
		Application::postEvent(receiver(), event.take());
	}
}

FILE_SYSTEM_NS_END
