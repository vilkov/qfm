#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include "../filesystemfoldertask.h"


FILE_SYSTEM_NS_BEGIN

class PerformTask : public FolderTask
{
public:
	class Event : public FolderTask::Event
	{
	public:
		Event(Type type, bool canceled, PScopedPointer<InfoListItem> &entries) :
			FolderTask::Event(type, canceled),
			entries(entries.take())
		{}

		PScopedPointer<InfoListItem> entries;
	};

public:
	PerformTask(TasksNode *receiver) :
		FolderTask(receiver)
	{}
};

FILE_SYSTEM_NS_END

#endif /* PERFORMTASK_H_ */
