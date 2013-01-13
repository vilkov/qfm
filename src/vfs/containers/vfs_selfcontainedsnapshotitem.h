/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
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
#ifndef VFS_SELFCONTAINEDSNAPSHOTITEM_H_
#define VFS_SELFCONTAINEDSNAPSHOTITEM_H_

#include "vfs_snapshotitem.h"


VFS_NS_BEGIN

class SelfContainedSnapshotItem : public SnapshotItem
{
    Q_DISABLE_COPY(SelfContainedSnapshotItem)

public:
    SelfContainedSnapshotItem();
    SelfContainedSnapshotItem(IFileContainer::Holder &container, IFileInfo::Holder &info, SnapshotItem *parent);

private:
    IFileContainer::Holder m_container;
};

VFS_NS_END

#endif /* VFS_SELFCONTAINEDSNAPSHOTITEM_H_ */
