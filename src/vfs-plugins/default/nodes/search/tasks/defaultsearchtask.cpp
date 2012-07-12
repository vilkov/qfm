#include "defaultsearchtask.h"


DEFAULT_PLUGIN_NS_BEGIN

SearchTask::SearchTask(const IFileContainer *container, TasksNode *receiver) :
	FilesBaseTask(receiver),
	m_container(container)
{}

void SearchTask::run(const volatile Flags &aborted)
{
	QString error;
	Snapshot snapshot(m_container);
	m_container->scanner()->scan(snapshot, aborted, error);
	postEvent(new Event(this, Event::SearchFiles, aborted, snapshot));
}

DEFAULT_PLUGIN_NS_END
