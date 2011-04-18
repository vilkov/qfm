#ifndef PERFORMREMOVEENTRYTASK_H_
#define PERFORMREMOVEENTRYTASK_H_

#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "../scan/scanfilestask.h"


class PerformRemoveEntryTask : public PerformTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveEntryTask)

public:
	struct Params : public PerformTask::Params
	{};
	struct EventParams : public PerformTask::EventParams
	{
		bool shoulRemoveEntry;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	PerformRemoveEntryTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformTask::parameters()); }

protected:
	void removeEntry(FileSystemEntry *entry, bool &tryAgain, const volatile bool &stopedFlag);

protected:
	bool m_shoulRemoveEntry;
	bool m_skipAllIfNotRemove;
	bool m_skipAllIfNotExists;
};

#endif /* PERFORMREMOVEENTRYTASK_H_ */
