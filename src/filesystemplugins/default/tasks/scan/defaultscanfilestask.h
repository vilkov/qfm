#ifndef DEFAULTSCANFILESTASK_H_
#define DEFAULTSCANFILESTASK_H_

#include "../defaultfilesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

class ScanFilesTask : public FilesBaseTask
{
public:
	class UpdatesEvent : public Event
	{
	public:
		UpdatesEvent(BaseTask *task, bool isLastEvent, const Snapshot &snapshot, bool canceled) :
			Event(task, static_cast<Type>(Event::UpdateFiles), canceled, snapshot),
			isLastEvent(isLastEvent)
		{}

		bool isLastEvent;
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
	ScanFilesTask(Event::Type type, TasksNode *receiver, const Snapshot &snapshot);
	ScanFilesTask(Event::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move);

protected:
	virtual void run(const volatile Flags &aborted);

protected:
	Snapshot takeUpdates(Snapshot &snapshot);
	Snapshot takeAllUpdates(Snapshot &snapshot);

private:
	Event::Type m_type;
	Snapshot m_snapshot;
	QString m_error;
	bool m_move;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTSCANFILESTASK_H_ */
