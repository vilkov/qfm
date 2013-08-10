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
#include "arc_entrynodeitem.h"

#include <vfs/tools/vfs_commontools.h>


ARC_PLUGIN_NS_BEGIN

EntryNodeItem::EntryNodeItem(SnapshotItem *item, Base *parent) :
	NodeItem(parent),
	m_info(item->info().take()),
	m_totalSize(item->totalSize())
{
	for (SnapshotItem::const_iterator i = item->begin(), end = item->end(); i != end; ++i)
		m_items.push_back(NodeItem::Holder(new EntryNodeItem((*i), this)));
}

EntryNodeItem::Base *EntryNodeItem::at(size_type index) const
{
	return m_items.at(index).data();
}

EntryNodeItem::size_type EntryNodeItem::size() const
{
	return m_items.size();
}

EntryNodeItem::size_type EntryNodeItem::indexOf(Base *item) const
{
	Holder holder(static_cast<EntryNodeItem *>(item));
	return m_items.indexOf(holder);
}

QVariant EntryNodeItem::data(qint32 column, qint32 role) const
{
	switch (column)
	{
		case 0:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_info->fileName().as<QString>();

				case Qt::DecorationRole:
					if (isLocked())
						return lockIcon();
					else
						return m_info->fileType()->icon();

				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;

				case Qt::ToolTipRole:
					if (isLocked())
						return lockReason();
					else
						break;
			}
			break;
		}
		case 1:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return Tools::humanReadableSize(m_info->fileSize());

				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;

				case Qt::ToolTipRole:
					return m_info->fileType()->name();
			}
			break;
		}
		case 2:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_info->lastModified();

				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
			}
			break;
		}
	}

	return QVariant();
}

void EntryNodeItem::lock(const QString &reason)
{
	start(m_totalSize);
	TasksNodeItem::lock(reason);
}

ARC_PLUGIN_NS_END
