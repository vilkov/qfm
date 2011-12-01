#ifndef PERFORMREMOVEBASETASK_H_
#define PERFORMREMOVEBASETASK_H_

#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "taskprogress.h"


FILE_SYSTEM_NS_BEGIN

class PerformRemoveBaseTask : public PerformTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveBaseTask)

public:
	PerformRemoveBaseTask(TasksNode *receiver);

protected:
	void remove(const InfoListItem *entries, const volatile bool &aborted);

private:
	void removeEntry(InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted);
	void removeDir(InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted);
	void removeFile(InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted);
	bool doRemoveFile(const QString &filePath, QString &error);

private:
	QString m_error;
	bool m_skipAllIfNotRemove;
	TaskProgress m_progress;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVEBASETASK_H_ */
