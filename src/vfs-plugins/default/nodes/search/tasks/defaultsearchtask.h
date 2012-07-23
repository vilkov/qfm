#ifndef DEFAULTSEARCHTASK_H_
#define DEFAULTSEARCHTASK_H_

#include "../../../tasks/defaultfilesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

class SearchTask : public BaseTask
{
public:
	class NewFileEvent : public Event
	{
	public:
		NewFileEvent(BaseTask *task, SnapshotItem *file) :
			Event(task, static_cast<Event::Type>(FilesBaseTask::Event::SearchFiles_NewFile), false),
			m_file(file)
		{}

		const PScopedPointer<SnapshotItem> &file() const { return m_file; }
		PScopedPointer<SnapshotItem> &file() { return m_file; }

	private:
		PScopedPointer<SnapshotItem> m_file;
	};

	class DoneEvent : public Event
	{
	public:
		DoneEvent(BaseTask *task, bool canceled) :
			Event(task, static_cast<Event::Type>(FilesBaseTask::Event::SearchFiles_Done), canceled)
		{}
	};

public:
	SearchTask(const Snapshot &snapshot, IFileContainerScanner::Filter::Holder &filter, TasksNode *receiver);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	static void newElement(void *user_data, SnapshotItem *item);

private:
	Snapshot m_snapshot;
	IFileContainerScanner::Filter::Holder m_filter;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTSEARCHTASK_H_ */
