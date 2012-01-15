#include "filesystemdelegate.h"
#include <QtGui/QApplication>


FILE_SYSTEM_NS_BEGIN

Delegate::Delegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

//QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	QSize size = QStyledItemDelegate::sizeHint(option, index);
//	size.setHeight(size.height() + 3);
//	return size;
//}

void Delegate::paintBackgroundLines(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

FILE_SYSTEM_NS_END
