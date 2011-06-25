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
	typedef ModelEvents::RemoveFilesCompletedEvent CompletedEvent;
	typedef ModelEvents::RemoveFilesCanceledEvent  CanceledEvent;
	typedef ModelEvents::QuestionAnswerEvent       QuestionAnswerEvent;

public:
	struct Params : public PerformTask::Params
	{
		Params(QObject *receiver, QScopedPointer<FileSystemList> &entries) :
			receiver(receiver),
			entries(entries.take())
		{}

		QObject *receiver;
		QScopedPointer<FileSystemList> entries;
	};

public:
	PerformRemoveTask(QObject *receiver, QScopedPointer<FileSystemList> &entries);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformTask::parameters()); }

	void removeEntry(FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag);
	void removeDir(FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag);
	void removeFile(FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag);
	bool doRemoveFile(const QString &filePath, QString &error);
	void postCompletedEvent() const;
	void postCanceledEvent() const;

protected:
	QString m_error;
	bool m_skipAllIfNotRemove;
	TaskProgress m_progress;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVETASK_H_ */
