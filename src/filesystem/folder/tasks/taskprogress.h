#ifndef TASKPROGRESS_H_
#define TASKPROGRESS_H_

#include <QtCore/QDateTime>
#include "../../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class TaskProgress
{
public:
	TaskProgress(QObject *receiver);

	void init(const QString &fileName);
	void update(quint64 progressIncrement);

private:
	void postEvent();

private:
	quint64 m_doneSize;
	QString m_fileName;
	QObject *m_receiver;
	QDateTime m_baseTime;
	QDateTime m_currentTime;
	QDateTime m_timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* TASKPROGRESS_H_ */
