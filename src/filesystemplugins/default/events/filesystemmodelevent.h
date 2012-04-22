#ifndef FILESYSTEMMODELEVENT_H_
#define FILESYSTEMMODELEVENT_H_

#include "../../../tasks/filesystembasetask.h"


FILE_SYSTEM_NS_BEGIN

struct ModelEvent
{
	enum Type
	{
		UpdateFiles = BaseTask::Event::User,
		ScanFilesForSize = BaseTask::Event::User + 1,
		ScanFilesForRemove = BaseTask::Event::User + 2,
		ScanFilesForCopy = BaseTask::Event::User + 3,
		RemoveFiles = BaseTask::Event::User + 4,
		CopyFiles = BaseTask::Event::User + 5
	};
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMMODELEVENT_H_ */
