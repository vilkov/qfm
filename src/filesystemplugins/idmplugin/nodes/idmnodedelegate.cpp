#include "idmnodedelegate.h"
#include <QtGui/QPainter>
#include <QtGui/QApplication>


IDM_PLUGIN_NS_BEGIN

IdmNodeDelegate::IdmNodeDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

//void IdmDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	QStyle *style;
//    QStyleOptionButton buttonOption;
//	int width = option.rect.width() / 5;
//
//	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
//		style = v3->widget->style();
//	else
//		style = QApplication::style();
//
//	for (int i = 0; i < 5; ++i)
//    {
//		buttonOption.rect = option.rect;
//		buttonOption.rect.setLeft(option.rect.left() + i * width);
//		buttonOption.rect.setRight(buttonOption.rect.left() + width);
//		buttonOption.text = QString::number(i);
//		style->drawControl(QStyle::CE_PushButton, &buttonOption, painter);
//    }
//}
//
//QSize IdmDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	QSize res = QStyledItemDelegate::sizeHint(option, index);
//	res.setHeight(res.height() + 5);
//
//	return res;
//}

IDM_PLUGIN_NS_END
