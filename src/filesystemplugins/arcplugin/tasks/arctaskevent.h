#ifndef ARCTASKEVENT_H_
#define ARCTASKEVENT_H_

#include "../arcplugin_ns.h"
#include "../../../filesystem/tasks/filesystembasetask.h"


ARC_PLUGIN_NS_BEGIN

class TaskEvent : public BaseTask::Event
{
public:
	enum Type
	{
		ScanComplete = User,
		CopyComplete = User + 1
	};

public:
	TaskEvent(BaseTask *task, Type type, bool canceled) :
		BaseTask::Event(task, static_cast<BaseTask::Event::Type>(type), canceled)
	{}
};

ARC_PLUGIN_NS_END

#endif /* ARCTASKEVENT_H_ */
