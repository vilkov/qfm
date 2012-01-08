#ifndef ARHDELEGATE_H_
#define ARHDELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include "../arhplugin_ns.h"


ARH_PLUGIN_NS_BEGIN

class ArhDelegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(ArhDelegate)

public:
	ArhDelegate(QObject *parent = 0);

    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

ARH_PLUGIN_NS_END

#endif /* ARHDELEGATE_H_ */
