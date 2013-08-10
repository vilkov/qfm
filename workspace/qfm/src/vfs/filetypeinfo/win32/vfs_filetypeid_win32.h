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
#ifndef VFS_FILETYPEID_WIN32_H_
#define VFS_FILETYPEID_WIN32_H_

#include <QtCore/QString>
#include "../../vfs_ns.h"


VFS_NS_BEGIN

struct FileTypeId_Win32
{
	FileTypeId_Win32();

	bool operator==(const FileTypeId_Win32 &other) const;
	bool operator<(const FileTypeId_Win32 &other) const;

	QString extension;
    QIcon icon;
};

VFS_NS_END

#endif /* VFS_FILETYPEID_WIN32_H_ */
