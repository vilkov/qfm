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
#ifndef VFS_IAPPLICATIONS_H_
#define VFS_IAPPLICATIONS_H_

#include <QtCore/QLinkedList>
#include "vfs_iapplication.h"
#include "vfs_ifiletype.h"


VFS_NS_BEGIN

class IApplications
{
public:
	typedef QLinkedList<const IApplication *> LinkedList;

public:
	virtual ~IApplications();

	virtual LinkedList user(const IFileType *fileType) const = 0;
	virtual LinkedList system(const IFileType *fileType) const = 0;
};

VFS_NS_END

#endif /* VFS_IAPPLICATIONS_H_ */
