#include "taskprogress.h"
#include "basetask.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

TaskProgress::TaskProgress(QObject *receiver) :
	m_receiver(receiver)
{}


void TaskProgress::init(const QString &fileName)
{
	m_progress = 0;
	m_fileName = fileName;
	m_baseTime = m_currentTime = m_startTime = QDateTime::currentDateTime();
}

void TaskProgress::update(quint64 progressIncrement)
{
	if (!m_fileName.isEmpty())
	{
		m_progress += progressIncrement;

		if (m_baseTime.secsTo(m_currentTime = QDateTime::currentDateTime()) > 1)
		{
			postEvent();
			m_baseTime = m_currentTime;
		}
	}
}

void TaskProgress::completed()
{
	typedef BaseTask::CompletedProgressEvent Event;

	PScopedPointer<Event> event(new Event(m_fileName, m_startTime.msecsTo(QDateTime::currentDateTime())));
	Application::postEvent(m_receiver, event.take());
}

void TaskProgress::clear()
{
	m_fileName.clear();
}

void TaskProgress::postEvent()
{
	typedef BaseTask::UpdateProgressEvent Event;

	PScopedPointer<Event> event(new Event(m_fileName, m_progress, m_startTime.msecsTo(m_currentTime)));
	Application::postEvent(m_receiver, event.take());
}

FILE_SYSTEM_NS_END
