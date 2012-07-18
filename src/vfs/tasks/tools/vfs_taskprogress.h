#ifndef VFS_TASKPROGRESS_H_
#define VFS_TASKPROGRESS_H_

#include <QtCore/QDateTime>
#include "../../model/items/vfs_item.h"


VFS_NS_BEGIN
class TasksNode;


class TaskProgress
{
public:
	TaskProgress(TasksNode *receiver);

	void init(const Item::Holder &item);
	void update(quint64 progressIncrement);
	void complete();
	void clear();

private:
	void postEvent();

private:
	Item::Holder m_item;
	quint64 m_progress;
	TasksNode *m_receiver;
	QDateTime m_baseTime;
	QDateTime m_currentTime;
	QDateTime m_startTime;
};

VFS_NS_END

#endif /* VFS_TASKPROGRESS_H_ */
