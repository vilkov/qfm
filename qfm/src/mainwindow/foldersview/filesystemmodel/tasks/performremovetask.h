#ifndef PERFORMREMOVETASK_H_
#define PERFORMREMOVETASK_H_

#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "scanfilestask.h"


class PerformRemoveTask : public PerformTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveTask)

public:
	struct Params : public PerformTask::Params
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
	struct EventParams : public PerformTask::EventParams
	{
		bool shoulRemoveEntry;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	PerformRemoveTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformTask::parameters()); }

private:
	void remove(FileSystemTree *tree, const volatile bool &stopedFlag);
	void removeEntry(FileSystemEntry *entry, bool &tryAgain, const volatile bool &stopedFlag);

private:
	bool m_shoulRemoveEntry;
	bool m_skipAllIfNotRemove;
	bool m_skipAllIfNotExists;
	volatile bool m_canceled;
};

#endif /* PERFORMREMOVETASK_H_ */
