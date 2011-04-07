#ifndef FILESYSTEMDECORATIONDELEGATE_H_
#define FILESYSTEMDECORATIONDELEGATE_H_

#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>


class FileSystemDecorationDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	FileSystemDecorationDelegate(QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /* FILESYSTEMDECORATIONDELEGATE_H_ */
