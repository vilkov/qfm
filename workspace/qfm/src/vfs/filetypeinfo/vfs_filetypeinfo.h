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
#ifndef VFS_FILETYPEINFO_H_
#define VFS_FILETYPEINFO_H_

#include <QtGui/QIcon>
#include <platform/platform.h>

#if PLATFORM_OS(UNIX)
#	include "unix/vfs_filetypeid_unix.h"
	VFS_NS(typedef ::VFS::FileTypeId_Unix FileTypeId)
#else
#	include "win32/vfs_filetypeid_win32.h"
	VFS_NS(typedef ::VFS::FileTypeId_Win32 FileTypeId)
#endif


VFS_NS_BEGIN

struct FileTypeInfo
{
	FileTypeInfo()
	{}

	bool isValid() const { return id.isValid(); }

	FileTypeId id;
	QIcon icon;
	QString name;
	QString description;
};

VFS_NS_END

#endif /* VFS_FILETYPEINFO_H_ */
