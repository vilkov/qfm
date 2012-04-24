#ifndef FILESYSTEMTASKSNODEDELEGATE_H_
#define FILESYSTEMTASKSNODEDELEGATE_H_

#include "model/items/filesystemtasksnodeitem.h"
#include "../filesystemdelegate.h"


FILE_SYSTEM_NS_BEGIN

class TasksNodeDelegate : public Delegate
{
	Q_DISABLE_COPY(TasksNodeDelegate)

public:
	TasksNodeDelegate(QObject *parent = 0);

protected:
    void paintProgressInMb(const TasksNodeItem *entry, QPainter *painter, const QStyleOptionViewItem &option) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKSNODEDELEGATE_H_ */
