#ifndef FILESYSTEMTASKSNODEDELEGATE_H_
#define FILESYSTEMTASKSNODEDELEGATE_H_

#include <QtGui/QAbstractProxyModel>
#include "../filesystemdelegate.h"


FILE_SYSTEM_NS_BEGIN

class TasksNodeDelegate : public Delegate
{
	Q_DISABLE_COPY(TasksNodeDelegate)

public:
	TasksNodeDelegate(QAbstractProxyModel *proxy, uint progressColumn, QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QAbstractProxyModel *m_proxy;
    uint m_progressColumn;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKSNODEDELEGATE_H_ */
