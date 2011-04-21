#ifndef PERFORMCOPYENTRYTASK_H_
#define PERFORMCOPYENTRYTASK_H_

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "../destcontrolabletask.h"


class PerformCopyEntryTask : public PerformTask<DestControlableTask>
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	typedef PerformTask<DestControlableTask> parent_class;

public:
	struct Params : public parent_class::Params
	{
		bool removeSource;
	};
	struct EventParams : public parent_class::EventParams
	{
		bool removeSource;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

	struct NewEntryParams : public ControlableTask::EventParams
	{
		FileSystemTree *fileSystemTree;
		QString absoluteFilePath;
	};
	typedef FileSystemModelEventTemplate<NewEntryParams> NewEntryEvent;

public:
	PerformCopyEntryTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

protected:
	void copyFile(const QDir &destination, FileSystemEntry *entry, bool &tryAgain, const volatile bool &stopedFlag);
	void askForSkipAllIfNotCopy(const QString &title, const QString &text, bool &tryAgain, const volatile bool &stopedFlag);
	void complete(const volatile bool &stopedFlag, Event::EventType type, const QString &destinationFilePath);

protected:
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_overwriteAll;
	bool m_canceled;
};

#endif /* PERFORMCOPYENTRYTASK_H_ */
