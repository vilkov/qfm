#include "default_searchtask.h"


DEFAULT_PLUGIN_NS_BEGIN

SearchTask::SearchTask(const Snapshot &snapshot, IFileContainerScanner::Filter::Holder &filter, TasksNode *receiver) :
	BaseTask(receiver),
	m_snapshot(snapshot),
	m_filter(filter.take())
{}

void SearchTask::run(const volatile Flags &aborted)
{
	QString error;
	IFileContainerScanner::SearchArguments args = {m_snapshot, m_filter, {newElement, this}, aborted};
	m_snapshot.container()->scanner()->search(args, error);
	postEvent(new DoneEvent(this, aborted));
}

void SearchTask::newElement(void *user_data, SnapshotItem *item)
{
	SearchTask *self(static_cast<SearchTask *>(user_data));
	self->postEvent(new NewFileEvent(self, item));
}

DEFAULT_PLUGIN_NS_END
