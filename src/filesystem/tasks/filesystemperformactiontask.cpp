#include "filesystemperformactiontask.h"


FILE_SYSTEM_NS_BEGIN

PerformActionTask::PerformActionTask(TasksNode *receiver, const AsyncFileAction::FilesList &files) :
	BaseTask(receiver),
	m_files(files)
{}

void PerformActionTask::run(const volatile Flags &aborted)
{
	QString error;
	process(aborted, error);
	postEvent(new Event(this, Event::Action, m_files, aborted, error));
}

FILE_SYSTEM_NS_END
