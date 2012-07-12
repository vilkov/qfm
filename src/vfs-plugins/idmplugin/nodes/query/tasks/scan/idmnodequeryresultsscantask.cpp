#include "idmnodequeryresultsscantask.h"


IDM_PLUGIN_NS_BEGIN

ScanFilesTask::ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, const Snapshot &snapshot) :
	FilesBaseTask(receiver),
	m_type(type),
	m_snapshot(snapshot),
	m_move(false)
{}

ScanFilesTask::ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move) :
	FilesBaseTask(receiver, destination),
	m_type(type),
	m_snapshot(snapshot),
	m_move(move)
{}

void ScanFilesTask::run(const volatile Flags &aborted)
{
	QString error;

	switch (m_type)
	{
		case ModelEvent::UpdateFiles:
		{
			m_snapshot.container()->scanner()->scan(m_snapshot, aborted, error);
			postEvent(new UpdatesEvent(this, m_snapshot, aborted));
			break;
		}

		default:
		{
			m_snapshot.container()->scanner()->scan(m_snapshot, aborted, error);
			postEvent(new Event(this, static_cast<Event::Type>(m_type), aborted, m_snapshot));
			break;
		}
	}
}

IDM_PLUGIN_NS_END
