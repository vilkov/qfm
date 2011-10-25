#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include "../filesystemfolderbasetask.h"


FILE_SYSTEM_NS_BEGIN

class PerformTask : public FolderBaseTask
{
public:
	class Event : public FolderBaseTask::Event
	{
	public:
		Event(Type type, bool canceled, PScopedPointer<InfoListItem> &entries) :
			FolderBaseTask::Event(type, canceled),
			entries(entries.take())
		{}

		PScopedPointer<InfoListItem> entries;
	};

public:
	PerformTask(QObject *receiver) :
		FolderBaseTask(receiver)
	{}
};

FILE_SYSTEM_NS_END

#endif /* PERFORMTASK_H_ */
