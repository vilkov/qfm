#include "taskprogress.h"
#include "basetask.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

TaskProgress::TaskProgress(QObject *receiver) :
	m_receiver(receiver)
{}


void TaskProgress::init(const QString &fileName)
{
	m_doneSize = 0;
	m_fileName = fileName;
	m_baseTime = m_currentTime = m_timeElapsed = QDateTime::currentDateTime();
}

void TaskProgress::update(quint64 progressIncrement)
{
	if (!m_fileName.isEmpty())
	{
		m_doneSize += progressIncrement;

		if (m_baseTime.secsTo(m_currentTime = QDateTime::currentDateTime()) > 1)
		{
			postEvent();
			m_baseTime = m_currentTime;
		}
	}
}

void TaskProgress::clear()
{
	m_fileName.clear();
}

void TaskProgress::postEvent()
{
	typedef BaseTask::UpdateProgressEvent UpdateProgressEvent;

	PScopedPointer<UpdateProgressEvent> event(new UpdateProgressEvent(m_fileName, m_doneSize, m_timeElapsed.msecsTo(m_currentTime)));
	Application::postEvent(m_receiver, event.take());
}

FILE_SYSTEM_NS_END
