#ifndef DEFAULTMODELEVENT_H_
#define DEFAULTMODELEVENT_H_

#include "../tasks/defaultfilesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

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

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTMODELEVENT_H_ */
