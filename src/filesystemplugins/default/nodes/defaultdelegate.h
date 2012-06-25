#ifndef DEFAULTDELEGATE_H_
#define DEFAULTDELEGATE_H_

#include <QtGui/QAbstractProxyModel>
#include "../default_ns.h"
#include "../../../filesystem/tasks/filesystemtasksnodedelegate.h"


DEFAULT_PLUGIN_NS_BEGIN

class Delegate : public TasksNodeDelegate
{
	Q_DISABLE_COPY(Delegate)

public:
	Delegate(QAbstractProxyModel *proxy, QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QAbstractProxyModel *m_proxy;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTDELEGATE_H_ */
