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
#include "arc_rootnodeitem.h"


ARC_PLUGIN_NS_BEGIN

RootNodeItem::RootNodeItem(Base *parent) :
	NodeItem(parent),
	m_label(QString::fromLatin1(".."))
{}

QVariant RootNodeItem::data(qint32 column, qint32 role) const
{
	if (column == 0)
		switch (role)
		{
			case Qt::EditRole:
			case Qt::DisplayRole:
				return m_label;

			case Qt::DecorationRole:
				if (isLocked())
					return lockIcon();
				else
					break;

			case Qt::TextAlignmentRole:
				return Qt::AlignLeft;

			case Qt::ToolTipRole:
				if (isLocked())
					return lockReason();
				else
					break;
		}

	return QVariant();
}

bool RootNodeItem::isRoot() const
{
	return true;
}

ARC_PLUGIN_NS_END
