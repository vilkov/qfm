#ifndef PERFORMCOPYTREETASK_H_
#define PERFORMCOPYTREETASK_H_

#include "performcopyentrytask.h"


class PerformCopyTreeTask : public PerformCopyEntryTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTreeTask)

public:
	struct Params : public PerformCopyEntryTask::Params
	{
		Params(QObject *receiver, const FileSystemModelEvents::ScanFilesWithDestParams &params, bool moveFiles) :
			subtree(params.subtree)
		{
			source.object = receiver;
			source.fileSystemTree = params.snapshot.fileSystemTree;
			source.entry = params.snapshot.entry;
			destination = params.destination;
			removeSource = moveFiles;
		}

		FileSystemTree *subtree;
	};
	typedef FileSystemModelEvents::CopyTreeFilesCompletedEvent CompletedEvent;
	typedef FileSystemModelEvents::CopyTreeFilesCanceledEvent  CanceledEvent;


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
