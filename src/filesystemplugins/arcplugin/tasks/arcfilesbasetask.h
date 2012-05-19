#ifndef ARCFILESBASETASK_H_
#define ARCFILESBASETASK_H_

#include "../arcplugin_ns.h"
#include "../../default/tasks/defaultfilesbasetask.h"


ARC_PLUGIN_NS_BEGIN

class FilesBaseTask : public Default::FilesBaseTask
{
public:
	typedef Default::FilesBaseTask Base;

public:
	class Event : public Base::Event
	{
		Q_DISABLE_COPY(Event)

	public:
		enum Type
		{
			ScanComplete = Base::Event::ScanFilesForSize,
			CopyComplete = Base::Event::CopyFiles
		};

	public:
		Event(BaseTask *task, Type type, const QString &error, bool canceled, const Snapshot &snapshot) :
			Base::Event(task, static_cast<Base::Event::Type>(type), canceled, snapshot),
			error(error)
		{}

		QString error;
	};

	class ExtendedEvent : public Base::ExtendedEvent
	{
		Q_DISABLE_COPY(ExtendedEvent)

	public:
		typedef FilesBaseTask::Event::Type Type;

	public:
		ExtendedEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot) :
			Base::ExtendedEvent(task, static_cast<Base::ExtendedEvent::Type>(type), destination, canceled, snapshot)
		{}
	};

public:
	FilesBaseTask(TasksNode *receiver);
	FilesBaseTask(TasksNode *receiver, ICopyControl::Holder &destination);
};

ARC_PLUGIN_NS_END

#endif /* ARCFILESBASETASK_H_ */
