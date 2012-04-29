#ifndef DEFAULTFILESBASETASK_H_
#define DEFAULTFILESBASETASK_H_

#include "../default_ns.h"
#include "../../../filesystem/tasks/filesystembasetask.h"
#include "../../../filesystem/containers/filesystemsnapshot.h"


DEFAULT_PLUGIN_NS_BEGIN

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

	class ExtendedEvent : public BaseTask::ExtendedEvent
	{
		Q_DISABLE_COPY(ExtendedEvent)

	public:
		ExtendedEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot) :
			BaseTask::ExtendedEvent(task, type, destination, canceled),
			snapshot(snapshot)
		{}

		Snapshot snapshot;
	};

public:
	FilesBaseTask(TasksNode *receiver);
	FilesBaseTask(TasksNode *receiver, ICopyControl::Holder &destination);
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILESBASETASK_H_ */
