#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include "../destcontrolabletask.h"


FILE_SYSTEM_NS_BEGIN

class PerformTask : public DestControlableTask
{
public:
	class Event : public ModelEvent
	{
	public:
		Event(Type type, PScopedPointer<FileSystemList> &entries, bool canceled) :
			ModelEvent(type),
			entries(entries.take()),
			canceled(canceled)
		{}

		PScopedPointer<FileSystemList> entries;
		bool canceled;
	};

public:
	PerformTask(QObject *receiver) :
		DestControlableTask(receiver),
		m_canceled(false)
	{}

protected:
	volatile bool m_canceled;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMTASK_H_ */
