#ifndef PERFORMCOPYTREETASK_H_
#define PERFORMCOPYTREETASK_H_

#include "performcopyentrytask.h"


FILE_SYSTEM_NS_BEGIN

class PerformCopyTreeTask : public PerformCopyEntryTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTreeTask)

public:
	struct Params : public PerformCopyEntryTask::Params
	{
		Params(Node *receiver, const ModelEvents::ScanFilesWithDestParams &params, bool moveFiles) :
			subnode(params.subnode)
		{
			source.node = receiver;
			source.entry = params.snapshot.entry;
			destination = params.destination;
			removeSource = moveFiles;
		}

		Node *subnode;
	};
	typedef ModelEvents::CopyTreeFilesCompletedEvent CompletedEvent;
	typedef ModelEvents::CopyTreeFilesCanceledEvent  CanceledEvent;


public:
	PerformCopyTreeTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformCopyEntryTask::parameters()); }

private:
	bool cd(QDir &destination, FolderNodeEntry *entry, const volatile bool &stopedFlag);
	void copy(QDir &destination, FolderNode *tree, const volatile bool &stopedFlag);
};

FILE_SYSTEM_NS_END

#endif /* PERFORMCOPYTREETASK_H_ */
