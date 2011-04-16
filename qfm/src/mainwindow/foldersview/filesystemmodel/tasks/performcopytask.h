#ifndef PERFORMCOPYTASK_H_
#define PERFORMCOPYTASK_H_

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "scanfilestask.h"


class PerformCopyTask : public PerformTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	struct Params : public PerformTask::Params
	{
		Params(QObject *rcv, const ScanFilesForCopyTask::EventParams &params) :
			entry(params.entry),
			subtree(params.subtree),
			destination(params.destination),
			destinationDirectory(params.destinationDirectory)
		{
			receiver = rcv;
			fileSystemTree = params.fileSystemTree;
		}

		FileSystemEntry *entry;
		FileSystemTree *subtree;
		QObject *destination;
		QString destinationDirectory;
	};
	struct EventParams : public PerformTask::EventParams
	{
		FileSystemEntry *entry;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;


public:
	PerformCopyTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformTask::parameters()); }

private:
	void copy(QDir &destination, FileSystemTree *tree, const volatile bool &stopedFlag);
	void askForSkipAllIfNotCopy(const QString &title, const QString &text, bool &tryAgain, const volatile bool &stopedFlag);

private:
	enum { ReadFileBufferSize = 1 * 1024 * 1024 };

private:
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_overwriteAll;
	bool m_canceled;
};

#endif /* PERFORMCOPYTASK_H_ */
