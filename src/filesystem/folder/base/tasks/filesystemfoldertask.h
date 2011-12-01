#ifndef FILESYSTEMFOLDERTASK_H_
#define FILESYSTEMFOLDERTASK_H_

#include "filesystemfolderbasetask.h"


FILE_SYSTEM_NS_BEGIN

class FolderTask : public FolderBaseTask
{
public:
	class Event : public FolderBaseTask::Event
	{
	public:
		Event(Type type, bool canceled) :
			FolderBaseTask::Event(type),
			canceled(canceled)
		{}

		bool canceled;
	};

public:
	FolderTask(TasksNode *receiver) :
		FolderBaseTask(receiver)
	{}
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERTASK_H_ */
