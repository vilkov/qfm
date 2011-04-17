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
		Params(QObject *receiver, const ScanFilesForCopyTask::EventParams &params) :
			source(receiver, params.snapshot.fileSystemTree, params.snapshot.entry),
			destination(params.destination),
			subtree(params.subtree)
		{}

		EventListener source;
		EventListener destination;
		FileSystemTree *subtree;
	};
	struct EventParams : public PerformTask::EventParams
	{
		FileSystemEntry *entry;
		FileSystemTree *subtree;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;


public:
	PerformCopyTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformTask::parameters()); }

private:
	bool cd(QDir &destination, FileSystemEntry *entry, const volatile bool &stopedFlag);
	void copy(QDir &destination, FileSystemTree *tree, const volatile bool &stopedFlag);
	void copyFile(QDir &destination, FileSystemEntry *entry, const volatile bool &stopedFlag);
	void askForSkipAllIfNotCopy(const QString &title, const QString &text, bool &tryAgain, const volatile bool &stopedFlag);

private:
	enum { ReadFileBufferSize = 10 * 1024 * 1024 };

private:
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_overwriteAll;
	bool m_canceled;
};

#endif /* PERFORMCOPYTASK_H_ */
