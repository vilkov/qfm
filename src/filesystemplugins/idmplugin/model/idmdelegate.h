#ifndef IDMDELEGATE_H_
#define IDMDELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include "../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class IdmDelegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(IdmDelegate)

public:
	IdmDelegate(QObject *parent = 0);

    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

IDM_PLUGIN_NS_END

#endif /* IDMDELEGATE_H_ */
