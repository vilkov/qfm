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
#include "default_rootnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN
static IFileInfo::Holder emptyInfo;


RootNodeItem::RootNodeItem(Base *parent) :
	NodeItem(emptyInfo, parent),
	m_label(QString::fromLatin1(".."))
{}

QVariant RootNodeItem::data(qint32 column, qint32 role) const
{
	if (column == 0)
		if (role == Qt::DisplayRole)
			return m_label;
		else
			if (isLocked())
				if (role == Qt::DecorationRole)
					return lockIcon();
				else
					if (role == Qt::ToolTipRole)
						return lockReason();

	return QVariant();
}

bool RootNodeItem::isRootItem() const
{
	return true;
}

DEFAULT_PLUGIN_NS_END
