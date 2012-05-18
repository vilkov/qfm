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
		Q_DISABLE_COPY(Event)

	public:
		enum Type
		{
			UpdateFiles = BaseTask::Event::User,
			ScanFilesForSize = BaseTask::Event::User + 1,
			ScanFilesForRemove = BaseTask::Event::User + 2,
			ScanFilesForCopy = BaseTask::Event::User + 3,
			RemoveFiles = BaseTask::Event::User + 4,
			CopyFiles = BaseTask::Event::User + 5
		};

	public:
		Event(BaseTask *task, Type type, bool canceled, const Snapshot &snapshot) :
			BaseTask::Event(task, static_cast<BaseTask::Event::Type>(type), canceled),
			snapshot(snapshot)
		{}

		Snapshot snapshot;
	};

	class ExtendedEvent : public BaseTask::ExtendedEvent
	{
		Q_DISABLE_COPY(ExtendedEvent)

	public:
		typedef FilesBaseTask::Event::Type Type;

	public:
		ExtendedEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot) :
			BaseTask::ExtendedEvent(task, static_cast<BaseTask::Event::Type>(type), destination, canceled),
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
