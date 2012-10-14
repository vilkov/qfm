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
#include "default_nodeitem.h"
#include <vfs/tools/vfs_commontools.h>


DEFAULT_PLUGIN_NS_BEGIN

NodeItem::NodeItem(IFileInfo::Holder &info, Base *parent) :
	TasksNodeItem(parent),
	m_node(NULL),
	m_info(info.take())
{}

NodeItem::NodeItem(IFileInfo::Holder &info, Node *node, Base *parent) :
	TasksNodeItem(parent),
	m_node(node),
	m_info(info.take())
{}

QVariant NodeItem::data(qint32 column, qint32 role) const
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
					if (m_info->isFile())
						return Tools::humanReadableSize(m_info->fileSize());
					else
						if (m_totalSize.isNull())
							return m_info->fileType()->name();
						else
							return Tools::humanReadableSize(m_totalSize.toULongLong());
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

bool NodeItem::isRootItem() const
{
	return false;
}

void NodeItem::lock(const QString &reason, quint64 totalSize)
{
	m_totalSize = totalSize;
	TasksNodeItem::lock(reason);
	start(totalSize);
}

void NodeItem::lock(const QString &reason)
{
	TasksNodeItem::lock(reason);
}

void NodeItem::unlock()
{
	stop();
	TasksNodeItem::unlock();
}

DEFAULT_PLUGIN_NS_END
