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
#include "vfs_snapshotitem.h"


VFS_NS_BEGIN

SnapshotItem::SnapshotItem() :
	m_container(NULL),
	m_totalSize(0),
	m_removed(true)
{}

SnapshotItem::SnapshotItem(const IFileContainer *container, IFileInfo::Holder &info, SnapshotItem *parent) :
	m_container(container),
	m_totalSize(info->fileSize()),
	m_info(info.take()),
	m_parent(parent),
	m_removed(true)
{
	Q_ASSERT(m_info.data());
}

SnapshotItem::~SnapshotItem()
{
	qDeleteAll(m_items);
}

SnapshotItem::SnapshotItem(const IFileContainer *container, SnapshotItem *parent) :
	m_container(container),
	m_totalSize(0),
	m_parent(parent),
	m_removed(true)
{}

SnapshotItem::SnapshotItem(const IFileContainer *container, IFileInfo::size_type totalSize, SnapshotItem *parent) :
	m_container(container),
	m_totalSize(totalSize),
	m_parent(parent),
	m_removed(true)
{}

VFS_NS_END
