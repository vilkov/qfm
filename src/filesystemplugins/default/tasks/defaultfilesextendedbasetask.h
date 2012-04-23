#ifndef DEFAULTFILESEXTENDEDBASETASK_H_
#define DEFAULTFILESEXTENDEDBASETASK_H_

#include "../default_ns.h"
#include "../../../filesystem/tasks/filesystemextendedbasetask.h"
#include "../../../filesystem/containers/filesystemsnapshot.h"


DEFAULT_PLUGIN_NS_BEGIN

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

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILESEXTENDEDBASETASK_H_ */
