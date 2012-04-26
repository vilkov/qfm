#ifndef DEFAULTSCANFILESTASK_H_
#define DEFAULTSCANFILESTASK_H_

#include "../defaultfilesbasetask.h"
#include "../defaultfilesextendedbasetask.h"
#include "../../events/defaultmodelevent.h"


DEFAULT_PLUGIN_NS_BEGIN

class ScanFilesTask : public FilesBaseTask
{
public:
	class UpdatesEvent : public BaseTask::Event
	{
	public:
		UpdatesEvent(BaseTask *task, bool isLastEvent, const Snapshot::Updates &updates, bool canceled = false) :
			BaseTask::Event(task, static_cast<Type>(ModelEvent::UpdateFiles), canceled),
			isLastEvent(isLastEvent),
			updates(updates)
		{}

		bool isLastEvent;
		Snapshot::Updates updates;
	};

public:
	ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, const Snapshot &snapshot);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	ModelEvent::Type m_type;
	Snapshot m_snapshot;
};


class ScanFilesExtendedTask : public FilesExtendedBaseTask
{
public:
	class CopyEvent : public FilesExtendedBaseTask::Event
	{
	public:
		CopyEvent(BaseTask *task, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move) :
			FilesExtendedBaseTask::Event(task, static_cast<Type>(ModelEvent::CopyFiles), destination, snapshot),
			move(move)
		{}

		bool move;
	};

public:
	ScanFilesExtendedTask(ModelEvent::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	ModelEvent::Type m_type;
	Snapshot m_snapshot;
	bool m_move;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTSCANFILESTASK_H_ */
