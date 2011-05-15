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
		Params(Node *receiver, const ModelEvents::ScanFilesForRemoveEvent::Params &params) :
			subnode(params.subnode)
		{
			source.node = receiver;
			source.entry = params.snapshot.entry;
		}
		Params(Node *receiver, const ModelEvents::CopyTreeFilesCompletedEvent::Params &params) :
			subnode(params.subnode)
		{
			source.node = receiver;
			source.entry = params.snapshot.entry;
		}

		Node *subnode;
	};

public:
	PerformRemoveTreeTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformRemoveEntryTask::parameters()); }

private:
	void remove(FolderNode *node, const volatile bool &stopedFlag);
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVETREETASK_H_ */
