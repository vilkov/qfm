#ifndef VFS_TASKPROGRESSEVENTS_H_
#define VFS_TASKPROGRESSEVENTS_H_

#include "../vfs_basetask.h"
#include "../../model/items/vfs_item.h"


VFS_NS_BEGIN

class UpdateProgressEvent : public BaseTask::Event
{
public:
	UpdateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::Progress, false),
		item(item),
		progress(progress),
		timeElapsed(timeElapsed)
	{}

	Item::Holder item;
	quint64 progress;
	quint64 timeElapsed;
};


class CompletedProgressEvent : public BaseTask::Event
{
public:
	CompletedProgressEvent(const Item::Holder &item, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::Completed, false),
		item(item),
		timeElapsed(timeElapsed)
	{}

	Item::Holder item;
	quint64 timeElapsed;
};

VFS_NS_END

#endif /* VFS_TASKPROGRESSEVENTS_H_ */
