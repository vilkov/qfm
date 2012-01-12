#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QtCore/QStringList>
#include "../../events/filesystemmodelevent.h"
#include "../../../../tasks/scan/scanfilesbasetask.h"
#include "../../../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class ScanFilesTask : public ScanFilesBaseTask
{
public:
	class Event : public ScanFilesBaseTask::Event
	{
	public:
		Event(BaseTask *task, ModelEvent::Type type) :
			ScanFilesBaseTask::Event(task, static_cast<Type>(type))
		{}

		ScanedFiles files;
	};

public:
	ScanFilesTask(TasksNode *receiver, const TasksNode::TasksItemList &files);

protected:
	ScanedFiles scan(const volatile Flags &aborted) const;

private:
	TasksNode::TasksItemList m_files;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASK_H_ */
