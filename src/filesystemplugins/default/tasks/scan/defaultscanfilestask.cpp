#include "defaultscanfilestask.h"


DEFAULT_PLUGIN_NS_BEGIN

DefaultScanFilesTask::DefaultScanFilesTask(ModelEvent::Type type, TasksNode *receiver, const Snapshot &snapshot) :
	FilesBaseTask(receiver),
	m_type(type),
	m_snapshot(snapshot)
{}

void DefaultScanFilesTask::run(const volatile Flags &aborted)
{
	switch (m_type)
	{
		case ModelEvent::UpdateFiles:
		{
			m_snapshot.container()->scanner();

			break;
		}

		case ModelEvent::ScanFilesForSize:
		case ModelEvent::ScanFilesForRemove:
		case ModelEvent::ScanFilesForCopy:
		{
			break;
		}

		default:
		{
			postEvent(new Event(this, static_cast<Event::Type>(m_type), aborted, Snapshot()));
			break;
		}
	}
}


DefaultScanFilesExtendedTask::DefaultScanFilesExtendedTask(ModelEvent::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot) :
	FilesExtendedBaseTask(receiver, destination),
	m_type(type),
	m_snapshot(snapshot)
{}

void DefaultScanFilesExtendedTask::run(const volatile Flags &aborted)
{

}

DEFAULT_PLUGIN_NS_END
