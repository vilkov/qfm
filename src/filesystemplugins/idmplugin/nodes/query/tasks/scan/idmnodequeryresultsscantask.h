#ifndef IDMNODEQUERYRESULTSSCANTASK_H_
#define IDMNODEQUERYRESULTSSCANTASK_H_

#include "../../events/idmqueryresultsmodelevents.h"
#include "../../../../../default/tasks/defaultfilesbasetask.h"


IDM_PLUGIN_NS_BEGIN

class ScanFilesTask : public ::FileSystem::Plugins::Default::FilesBaseTask
{
public:
	class UpdatesEvent : public Event
	{
	public:
		UpdatesEvent(BaseTask *task, const Snapshot &snapshot, bool canceled) :
			Event(task, static_cast<Type>(ModelEvent::UpdateFiles), canceled, snapshot)
		{}
	};

	class CopyEvent : public ExtendedEvent
	{
	public:
		CopyEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot, bool move) :
			ExtendedEvent(task, type, destination, canceled, snapshot),
			move(move)
		{}

		bool move;
	};

public:
	ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, const Snapshot &snapshot);
	ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	ModelEvent::Type m_type;
	Snapshot m_snapshot;
	bool m_move;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSSCANTASK_H_ */
