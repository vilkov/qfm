#ifndef PERFORMREMOVETREETASK_H_
#define PERFORMREMOVETREETASK_H_

#include "performremoveentrytask.h"


class PerformRemoveTreeTask : public PerformRemoveEntryTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveTreeTask)

public:
	struct Params : public PerformRemoveEntryTask::Params
	{
		Params(QObject *receiver, const ScanFilesForRemoveTask::EventParams &params) :
			subtree(params.subtree)
		{
			source.object = receiver;
			source.fileSystemTree = params.snapshot.fileSystemTree;
			source.entry = params.snapshot.entry;
		}

		FileSystemTree *subtree;
	};

public:
	PerformRemoveTreeTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformRemoveEntryTask::parameters()); }

private:
	void remove(FileSystemTree *tree, const volatile bool &stopedFlag);
};

#endif /* PERFORMREMOVETREETASK_H_ */
