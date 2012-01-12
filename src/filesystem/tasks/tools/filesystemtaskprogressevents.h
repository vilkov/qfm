#ifndef FILESYSTEMTASKPROGRESSEVENTS_H_
#define FILESYSTEMTASKPROGRESSEVENTS_H_

#include "../filesystembasetask.h"
#include "../items/filesystemtasknodeitem.h"


FILE_SYSTEM_NS_BEGIN

class UpdateProgressEvent : public BaseTask::Event
{
public:
	UpdateProgressEvent(TaskNodeItem::Base *item, quint64 progress, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::Progress),
		item(item),
		progress(progress),
		timeElapsed(timeElapsed)
	{}

	TaskNodeItem::Base *item;
	quint64 progress;
	quint64 timeElapsed;
};


class CompletedProgressEvent : public BaseTask::Event
{
public:
	CompletedProgressEvent(TaskNodeItem::Base *item, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::Completed),
		item(item),
		timeElapsed(timeElapsed)
	{}

	TaskNodeItem::Base *item;
	quint64 timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKPROGRESSEVENTS_H_ */
