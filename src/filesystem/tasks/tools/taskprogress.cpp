#include "taskprogress.h"
#include "filesystemtaskprogressevents.h"
#include "../filesystemtasksnode.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

TaskProgress::TaskProgress(TasksNode *receiver) :
	m_item(0),
	m_receiver(receiver)
{}


void TaskProgress::init(const FileSystemItem *item)
{
	m_item = item;
	m_progress = 0;
	m_baseTime = m_currentTime = m_startTime = QDateTime::currentDateTime();
}

void TaskProgress::update(quint64 progressIncrement)
{
	if (m_item)
	{
		m_progress += progressIncrement;

		if (m_baseTime.secsTo(m_currentTime = QDateTime::currentDateTime()) > 1)
		{
			postEvent();
			m_baseTime = m_currentTime;
		}
	}
}

void TaskProgress::complete()
{
	typedef CompletedProgressEvent Event;

	PScopedPointer<Event> event(new Event(m_item, m_startTime.msecsTo(QDateTime::currentDateTime())));
	Application::postEvent(m_receiver, event.take());
}

void TaskProgress::clear()
{
	m_item = 0;
}

void TaskProgress::postEvent()
{
	typedef UpdateProgressEvent Event;

	PScopedPointer<Event> event(new Event(m_item, m_progress, m_startTime.msecsTo(m_currentTime)));
	Application::postEvent(m_receiver, event.take());
}

FILE_SYSTEM_NS_END
