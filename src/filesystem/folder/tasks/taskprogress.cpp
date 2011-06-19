#include "taskprogress.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

TaskProgress::TaskProgress(QObject *receiver) :
	m_doneSize(0),
	m_receiver(receiver)
{}


void TaskProgress::init(const QString &fileName)
{
	m_fileName = fileName;
	m_baseTime = m_currentTime = m_timeElapsed = QDateTime::currentDateTime();
}

void TaskProgress::update(quint64 progressIncrement)
{
	m_doneSize += progressIncrement;

	if (m_baseTime.secsTo(m_currentTime = QDateTime::currentDateTime()) > 1)
	{
		postEvent();
		m_baseTime = m_currentTime;
	}
}

void TaskProgress::postEvent()
{
	QScopedPointer<UpdateProgressEvent> event(new UpdateProgressEvent());
	event->params().fileName = m_fileName;
	event->params().progress = m_doneSize;
	event->params().timeElapsed = m_timeElapsed.msecsTo(m_currentTime);
	Application::postEvent(m_receiver, event.take());
}

FILE_SYSTEM_NS_END
