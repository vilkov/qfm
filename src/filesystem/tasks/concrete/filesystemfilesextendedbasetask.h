#ifndef FILESYSTEMFILESEXTENDEDBASETASK_H_
#define FILESYSTEMFILESEXTENDEDBASETASK_H_

#include "containers/filesystemsnapshot.h"
#include "../filesystemextendedbasetask.h"


FILE_SYSTEM_NS_BEGIN

class FilesExtendedBaseTask : public ExtendedBaseTask
{
public:
	class Event : public ExtendedBaseTask::Event
	{
	public:
		Event(BaseTask *task, Type type, ICopyControl::Holder &destination, const Snapshot &snapshot) :
			ExtendedBaseTask::Event(task, type, destination),
			snapshot(snapshot)
		{}
		Event(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot) :
			ExtendedBaseTask::Event(task, type, destination, canceled),
			snapshot(snapshot)
		{}

		Snapshot snapshot;
	};

public:
	FilesExtendedBaseTask(TasksNode *receiver, ICopyControl::Holder &destination);
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILESEXTENDEDBASETASK_H_ */
