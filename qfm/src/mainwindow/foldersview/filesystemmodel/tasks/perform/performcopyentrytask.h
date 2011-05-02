#ifndef PERFORMCOPYENTRYTASK_H_
#define PERFORMCOPYENTRYTASK_H_

#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "../taskprogress.h"
#include "../destcontrolabletask.h"


class PerformCopyEntryTask : public PerformTask<DestControlableTask>
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	typedef PerformTask<DestControlableTask> parent_class;
	typedef FileSystemModelEvents::CopyFilesCompletedEvent    CompletedEvent;
	typedef FileSystemModelEvents::CopyFilesCanceledEvent     CanceledEvent;
	typedef FileSystemModelEvents::QuestionAnswerEvent        QuestionAnswerEvent;
	typedef FileSystemModelEvents::UpdatePerformProgressEvent UpdateProgressEvent;

public:
	struct Params : public parent_class::Params
	{
		bool removeSource;
	};

public:
	PerformCopyEntryTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

protected:
	void copyFile(const QDir &destination, FileSystemEntry *entry, bool &tryAgain, const volatile bool &stopedFlag);
	void askForSkipAllIfNotCopy(const QString &title, const QString &text, bool &tryAgain, const volatile bool &stopedFlag);

protected:
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_overwriteAll;
	TaskProgress m_progress;

private:
	friend class CopyProgressRoutine;
};

#endif /* PERFORMCOPYENTRYTASK_H_ */
