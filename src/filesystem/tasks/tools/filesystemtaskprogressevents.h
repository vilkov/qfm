#ifndef FILESYSTEMTASKPROGRESSEVENTS_H_
#define FILESYSTEMTASKPROGRESSEVENTS_H_

#include "../filesystembasetask.h"
#include "../../model/items/filesystemnodeitem.h"


FILE_SYSTEM_NS_BEGIN

class UpdateProgressEvent : public BaseTask::Event
{
public:
	UpdateProgressEvent(const NodeItem::Holder &item, quint64 progress, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::Progress, false),
		item(item),
		progress(progress),
		timeElapsed(timeElapsed)
	{}

	NodeItem::Holder item;
	quint64 progress;
	quint64 timeElapsed;
};


class CompletedProgressEvent : public BaseTask::Event
{
public:
	CompletedProgressEvent(const NodeItem::Holder &item, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::Completed, false),
		item(item),
		timeElapsed(timeElapsed)
	{}

	NodeItem::Holder item;
	quint64 timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKPROGRESSEVENTS_H_ */
