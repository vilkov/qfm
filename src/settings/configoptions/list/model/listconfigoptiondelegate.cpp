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
#include "listconfigoptiondelegate.h"
#include <QtGui/QLineEdit>


ListConfigOptionDelegate::ListConfigOptionDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

QWidget *ListConfigOptionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return new QLineEdit(parent);
}

void ListConfigOptionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	static_cast<QLineEdit*>(editor)->setText(index.data().toString());
}

void ListConfigOptionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QString text = static_cast<QLineEdit*>(editor)->text().simplified();

	if (!text.isEmpty())
		model->setData(index, text);
}

void ListConfigOptionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize ListConfigOptionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}
