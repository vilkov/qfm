#include "filesystemdecorationdelegate.h"
#include <QtGui/QApplication>


FileSystemDecorationDelegate::FileSystemDecorationDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

void FileSystemDecorationDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    if (index.row() % 2 == 0)
        opt.backgroundBrush = QBrush(Qt::white);
    else
    	opt.backgroundBrush = QBrush(QColor(245, 245, 245));

	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
		v3->widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, v3->widget);
	else
		QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, 0);
}

//QSize FileSystemDecorationDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	QSize size = QStyledItemDelegate::sizeHint(option, index);
//	size.setHeight(size.height() + 3);
//	return size;
//}
