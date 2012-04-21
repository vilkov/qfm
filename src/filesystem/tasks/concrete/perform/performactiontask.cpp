#include "performactiontask.h"


FILE_SYSTEM_NS_BEGIN

PerformActionTask::PerformActionTask(TasksNode *receiver, const AsyncFileAction::FilesList &files) :
	BaseTask(receiver),
	m_files(files)
{}

void PerformActionTask::run(const volatile Flags &aborted)
{
	process(aborted);
	postEvent(new Event(this, Event::Action, m_files, aborted));
}

FILE_SYSTEM_NS_END
