#ifndef TASKPROGRESS_H_
#define TASKPROGRESS_H_

#include <QtCore/QDateTime>
#include "../filesystemtasksnode.h"


FILE_SYSTEM_NS_BEGIN


class TaskProgress
{
public:
	TaskProgress(TasksNode *receiver);

	void init(const QString &fileName);
	void update(quint64 progressIncrement);
	void complete();
	void clear();

private:
	void postEvent();

private:
	quint64 m_progress;
	QString m_fileName;
	TasksNode *m_receiver;
	QDateTime m_baseTime;
	QDateTime m_currentTime;
	QDateTime m_startTime;
};

FILE_SYSTEM_NS_END

#endif /* TASKPROGRESS_H_ */
