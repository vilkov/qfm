#ifndef ARC_DELEGATE_H_
#define ARC_DELEGATE_H_

#include <QtGui/QAbstractProxyModel>
#include <vfs/tasks/model/vfs_tasksnodedelegate.h>
#include "../arc_ns.h"


ARC_PLUGIN_NS_BEGIN

class Delegate : public TasksNodeDelegate
{
	Q_DISABLE_COPY(Delegate)

public:
	Delegate(QAbstractProxyModel *proxy, QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QAbstractProxyModel *m_proxy;
};

ARC_PLUGIN_NS_END

#endif /* ARC_DELEGATE_H_ */
