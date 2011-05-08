#ifndef FILESYSTEMFOLDERDELEGATE_H_
#define FILESYSTEMFOLDERDELEGATE_H_

#include <QtGui/QAbstractProxyModel>
#include "../filesystemdelegate.h"


FILE_SYSTEM_NS_BEGIN

class FolderDelegate : public Delegate
{
	Q_DISABLE_COPY(FolderDelegate)

public:
	FolderDelegate(QAbstractProxyModel *proxy, QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QAbstractProxyModel *m_proxy;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERDELEGATE_H_ */
