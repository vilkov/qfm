#ifndef PERFORMREMOVETASK_H_
#define PERFORMREMOVETASK_H_

#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "../taskprogress.h"


FILE_SYSTEM_NS_BEGIN

class PerformRemoveTask : public PerformTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveTask)

public:
	PerformRemoveTask(QObject *receiver, PScopedPointer<InfoListItem> &entries);

	virtual void run(const volatile bool &aborted);

protected:
	void removeEntry(InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted);
	void removeDir(InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted);
	void removeFile(InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted);
	bool doRemoveFile(const QString &filePath, QString &error);

protected:
	QString m_error;
	bool m_skipAllIfNotRemove;
	TaskProgress m_progress;
	PScopedPointer<InfoListItem> m_entries;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVETASK_H_ */
