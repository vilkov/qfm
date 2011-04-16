#ifndef PERFORMCOPYTASK_H_
#define PERFORMCOPYTASK_H_

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
			destination(params.destination)
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
};

#endif /* PERFORMCOPYTASK_H_ */
