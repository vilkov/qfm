#ifndef DEFAULT_SEARCHTASK_H_
#define DEFAULT_SEARCHTASK_H_

#include "../../../tasks/default_filesbasetask.h"


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

		const SnapshotItem::Holder &file() const { return m_file; }
		SnapshotItem::Holder &file() { return m_file; }

	private:
		SnapshotItem::Holder m_file;
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

#endif /* DEFAULT_SEARCHTASK_H_ */
