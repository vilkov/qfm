#ifndef PERFORMCOPYTREETASK_H_
#define PERFORMCOPYTREETASK_H_

#include "performcopyentrytask.h"
#include "../scan/scanfilestask.h"


class PerformCopyTreeTask : public PerformCopyEntryTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTreeTask)

public:
	struct Params : public PerformCopyEntryTask::Params
	{
		Params(QObject *receiver, const ScanFilesForCopyTask::EventParams &params) :
			subtree(params.subtree)
		{
			source.object = receiver;
			source.fileSystemTree = params.snapshot.fileSystemTree;
			source.entry = params.snapshot.entry;
			destination = params.destination;
			removeSource = false;
		}
		Params(QObject *receiver, const ScanFilesForMoveTask::EventParams &params) :
			subtree(params.subtree)
		{
			source.object = receiver;
			source.fileSystemTree = params.snapshot.fileSystemTree;
			source.entry = params.snapshot.entry;
			destination = params.destination;
			removeSource = true;
		}

		FileSystemTree *subtree;
	};
	struct EventParams : public PerformCopyEntryTask::EventParams
	{
		FileSystemTree *subtree;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;


public:
	PerformCopyTreeTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformCopyEntryTask::parameters()); }

private:
	bool cd(QDir &destination, FileSystemEntry *entry, const volatile bool &stopedFlag);
	void copy(QDir &destination, FileSystemTree *tree, const volatile bool &stopedFlag);

private:
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_overwriteAll;
	bool m_canceled;
};

#endif /* PERFORMCOPYTREETASK_H_ */
