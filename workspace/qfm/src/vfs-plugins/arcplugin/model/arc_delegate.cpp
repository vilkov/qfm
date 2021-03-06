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
#include "arc_delegate.h"


ARC_PLUGIN_NS_BEGIN

Delegate::Delegate(QAbstractProxyModel *proxy, QObject *parent) :
	TasksNodeDelegate(parent),
	m_proxy(proxy)
{
	Q_ASSERT(m_proxy != 0);
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		TasksNodeItem *entry = static_cast<TasksNodeItem *>(m_proxy->mapToSource(index).internalPointer());

		if (entry->isInProgress())
		{
			paintProgressInMb(entry, painter, option);
			return;
		}
	}

	TasksNodeDelegate::paint(painter, option, index);
}

void Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}

ARC_PLUGIN_NS_END
