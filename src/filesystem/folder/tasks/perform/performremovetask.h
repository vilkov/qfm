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
	PerformRemoveTask(QObject *receiver, PScopedPointer<FileSystemList> &entries);

	virtual void run(const volatile bool &stopedFlag);

protected:
	void removeEntry(FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag);
	void removeDir(FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag);
	void removeFile(FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag);
	bool doRemoveFile(const QString &filePath, QString &error);

protected:
	QString m_error;
	bool m_skipAllIfNotRemove;
	TaskProgress m_progress;
	PScopedPointer<FileSystemList> m_entries;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVETASK_H_ */
