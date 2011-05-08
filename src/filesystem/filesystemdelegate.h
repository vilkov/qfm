#ifndef FILESYSTEMDELEGATE_H_
#define FILESYSTEMDELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include <QtGui/QStyleOptionViewItem>
#include "filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class Delegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(Delegate)

public:
	Delegate(QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMDELEGATE_H_ */
