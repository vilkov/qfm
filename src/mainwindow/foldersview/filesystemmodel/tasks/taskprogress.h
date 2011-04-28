#ifndef TASKPROGRESS_H_
#define TASKPROGRESS_H_

#include <QtCore/QDateTime>
#include "../events/filesystemmodelevents.h"


class TaskProgress
{
public:
	typedef FileSystemModelEvents::BaseTask::Params::Snapshot Snapshot;
	typedef FileSystemModelEvents::UpdatePerformProgressEvent UpdateProgressEvent;

	TaskProgress(const Snapshot &snapshot);

public:
	void init();
	void update(quint64 progressIncrement);

private:
	void postEvent();

private:
	quint64 m_doneSize;
	QDateTime m_baseTime;
	QDateTime m_currentTime;
	QDateTime m_timeElapsed;
	const Snapshot &m_snapshot;
};

#endif /* TASKPROGRESS_H_ */
