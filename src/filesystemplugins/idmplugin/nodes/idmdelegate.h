#ifndef IDMDELEGATE_H_
#define IDMDELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include <QtGui/QStyleOptionViewItem>
#include "../../../filesystem/filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IdmDelegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(IdmDelegate)

public:
	IdmDelegate(QObject *parent = 0);

//    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

FILE_SYSTEM_NS_END

#endif /* IDMDELEGATE_H_ */
