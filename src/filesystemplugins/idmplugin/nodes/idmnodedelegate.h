#ifndef IDMNODEDELEGATE_H_
#define IDMNODEDELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include <QtGui/QStyleOptionViewItem>
#include "../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class IdmNodeDelegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(IdmNodeDelegate)

public:
	IdmNodeDelegate(QObject *parent = 0);

//    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEDELEGATE_H_ */
