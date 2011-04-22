#ifndef FILESYSTEMDECORATIONDELEGATE_H_
#define FILESYSTEMDECORATIONDELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include <QtGui/QStyleOptionViewItem>
#include <QtGui/QAbstractProxyModel>


class FileSystemDecorationDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	FileSystemDecorationDelegate(QAbstractProxyModel *proxy, QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QAbstractProxyModel *m_proxy;
};

#endif /* FILESYSTEMDECORATIONDELEGATE_H_ */
