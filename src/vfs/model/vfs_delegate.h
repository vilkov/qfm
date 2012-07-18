#ifndef VFS_DELEGATE_H_
#define VFS_DELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include <QtGui/QStyleOptionViewItem>
#include "../vfs_ns.h"


VFS_NS_BEGIN

class Delegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(Delegate)

public:
	Delegate(QObject *parent = 0);

//    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    void paintBackgroundLines(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

VFS_NS_END

#endif /* VFS_DELEGATE_H_ */
