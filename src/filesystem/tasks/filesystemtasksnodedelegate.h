#ifndef FILESYSTEMTASKSNODEDELEGATE_H_
#define FILESYSTEMTASKSNODEDELEGATE_H_

#include "items/filesystemtasknodeitemlist.h"
#include "../filesystemdelegate.h"


FILE_SYSTEM_NS_BEGIN

class TasksNodeDelegate : public Delegate
{
	Q_DISABLE_COPY(TasksNodeDelegate)

public:
	TasksNodeDelegate(QObject *parent = 0);

protected:
    void paintProgressInMb(const TaskNodeItem *entry, QPainter *painter, const QStyleOptionViewItem &option) const;
    void paintProgressInMb(const TaskNodeListItem *entry, QPainter *painter, const QStyleOptionViewItem &option) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKSNODEDELEGATE_H_ */
