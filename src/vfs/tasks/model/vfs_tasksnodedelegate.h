#ifndef VFS_TASKSNODEDELEGATE_H_
#define VFS_TASKSNODEDELEGATE_H_

#include "items/vfs_tasksnodeitem.h"
#include "../../model/vfs_delegate.h"


VFS_NS_BEGIN

class TasksNodeDelegate : public Delegate
{
	Q_DISABLE_COPY(TasksNodeDelegate)

public:
	TasksNodeDelegate(QObject *parent = 0);

protected:
    void paintProgressInMb(const TasksNodeItem *entry, QPainter *painter, const QStyleOptionViewItem &option) const;
};

VFS_NS_END

#endif /* VFS_TASKSNODEDELEGATE_H_ */
