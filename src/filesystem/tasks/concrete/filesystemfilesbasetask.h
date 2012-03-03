#ifndef FILESYSTEMFILESBASETASK_H_
#define FILESYSTEMFILESBASETASK_H_

#include "containers/filesystemsnapshot.h"
#include "../filesystembasetask.h"


FILE_SYSTEM_NS_BEGIN

class FilesBaseTask : public BaseTask
{
public:
	class Event : public BaseTask::Event
	{
	public:
		Event(BaseTask *task, Type type, bool canceled, const Snapshot &snapshot) :
			BaseTask::Event(task, type, canceled),
			snapshot(snapshot)
		{}

		Snapshot snapshot;
	};

public:
	FilesBaseTask(TasksNode *receiver);
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILESBASETASK_H_ */
