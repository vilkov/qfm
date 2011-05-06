#ifndef TASKPROGRESS_H_
#define TASKPROGRESS_H_

#include <QtCore/QDateTime>
#include "../../filesystem_ns.h"
#include "../events/filesystemmodelevents.h"


FILE_SYSTEM_NS_BEGIN

class TaskProgress
{
public:
	typedef ModelEvents::BaseTask::Params::Snapshot Snapshot;
	typedef ModelEvents::UpdatePerformProgressEvent UpdateProgressEvent;

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

FILE_SYSTEM_NS_END

#endif /* TASKPROGRESS_H_ */
