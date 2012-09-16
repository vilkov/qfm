#ifndef IDM_DELEGATE_H_
#define IDM_DELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include "../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class Delegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(Delegate)

public:
	Delegate(QObject *parent = 0);

    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

IDM_PLUGIN_NS_END

#endif /* IDM_DELEGATE_H_ */
