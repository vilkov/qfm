#ifndef DEFAULTFOLDERDELEGATE_H_
#define DEFAULTFOLDERDELEGATE_H_

#include <QtGui/QAbstractProxyModel>
#include "default_ns.h"
#include "../../filesystem/tasks/filesystemtasksnodedelegate.h"


DEFAULT_PLUGIN_NS_BEGIN

class FolderDelegate : public TasksNodeDelegate
{
	Q_DISABLE_COPY(FolderDelegate)

public:
	FolderDelegate(QAbstractProxyModel *proxy, QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QAbstractProxyModel *m_proxy;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERDELEGATE_H_ */
