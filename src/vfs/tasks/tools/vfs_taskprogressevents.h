#ifndef VFS_TASKPROGRESSEVENTS_H_
#define VFS_TASKPROGRESSEVENTS_H_

#include "../vfs_basetask.h"
#include "../../model/items/vfs_nodeitem.h"


VFS_NS_BEGIN

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

VFS_NS_END

#endif /* VFS_TASKPROGRESSEVENTS_H_ */
