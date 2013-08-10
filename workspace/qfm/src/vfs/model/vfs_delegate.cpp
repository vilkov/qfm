/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "vfs_delegate.h"

#include <QtGui/QApplication>


VFS_NS_BEGIN

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

VFS_NS_END
