#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include "../destcontrolabletask.h"


FILE_SYSTEM_NS_BEGIN

class PerformTask : public DestControlableTask
{
public:
	class Event : public DestControlableTask::Event
	{
	public:
		Event(Type type, bool canceled, PScopedPointer<InfoListItem> &entries) :
			DestControlableTask::Event(type, canceled),
			entries(entries.take())
		{}

		PScopedPointer<InfoListItem> entries;
	};

public:
	PerformTask(QObject *receiver) :
		DestControlableTask(receiver)
	{}
};

FILE_SYSTEM_NS_END

#endif /* PERFORMTASK_H_ */
