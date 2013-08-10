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
#ifndef VFS_IFILEOPERATIONS_H_
#define VFS_IFILEOPERATIONS_H_

#include "vfs_inodeview.h"
#include "vfs_icopycontrol.h"


VFS_NS_BEGIN

class IFileOperations
{
public:
	virtual ~IFileOperations();

	virtual ICopyControl *createControl(INodeView *view) const = 0;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view) = 0;
	virtual void cancel(const QModelIndexList &list, INodeView *view) = 0;
};

VFS_NS_END

#endif /* VFS_IFILEOPERATIONS_H_ */
