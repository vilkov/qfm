#ifndef PERFORMREMOVETREETASK_H_
#define PERFORMREMOVETREETASK_H_

#include "performremoveentrytask.h"
#include "performcopytreetask.h"


FILE_SYSTEM_NS_BEGIN

class PerformRemoveTreeTask : public PerformRemoveEntryTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveTreeTask)

public:
	typedef ModelEvents::UpdatePerformProgressEvent UpdateProgressEvent;

public:
	struct Params : public PerformRemoveEntryTask::Params
	{
		Params(Node *receiver, ModelEvents::ScanFilesForRemoveEvent::Params &params) :
			subnode(params.subnode.take())
		{
			source.node = receiver;
			source.entry = params.snapshot.entry;
		}
		Params(Node *receiver, ModelEvents::CopyTreeFilesCompletedEvent::Params &params) :
			subnode(params.subnode.take())
		{
			source.node = receiver;
			source.entry = params.snapshot.entry;
		}

		QScopedPointer<FolderNodeItemList> subnode;
	};

public:
	PerformRemoveTreeTask(Node *receiver, ModelEvents::ScanFilesForRemoveEvent::Params &params);
	PerformRemoveTreeTask(Node *receiver, ModelEvents::CopyTreeFilesCompletedEvent::Params &params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformRemoveEntryTask::parameters()); }

private:
	void remove(FolderNodeItemList *node, const volatile bool &stopedFlag);
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVETREETASK_H_ */
