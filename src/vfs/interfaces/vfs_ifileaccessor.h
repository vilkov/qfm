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
#ifndef VFS_IFILEACCESSOR_H_
#define VFS_IFILEACCESSOR_H_

#include <QtCore/QString>
#include "vfs_ifileinfo.h"


VFS_NS_BEGIN

class IFileAccessor
{
public:
	typedef PScopedPointer<IFileAccessor> Holder;
	typedef uchar                         value_type;
	typedef IFileInfo::size_type          size_type;
	enum OpenMode
	{
		ReadOnly  = 0x00000001,
		ReadWrite = 0x00000002,
		Open      = 0x00000004,
		Create    = 0x00000008,
		Truncate  = 0x00000010
	};
    enum Permissions
    {
        UserRead   = 0x00000001, UserWrite   = 0x00000002, UserExec   = 0x00000004,
        GroupRead  = 0x00000008, GroupWrite  = 0x00000010, GroupExec  = 0x00000020,
        OthersRead = 0x00000040, OthersWrite = 0x00000080, OthersExec = 0x00000100
    };
    enum Seek
    {
    	FromBeggining,
    	FromCurrent,
    	FromEnd
    };

public:
	virtual ~IFileAccessor();

	virtual QString lastError() const = 0;
	virtual int permissions() const = 0;

	virtual size_type size() = 0;
	virtual bool setPermissions(int mode) = 0;
	virtual size_type read(value_type *data, size_type size) = 0;
	virtual size_type write(const value_type *data, size_type size) = 0;
	virtual size_type seek(size_type offset, Seek from = FromCurrent) = 0;

protected:
	static const int open_flags[];
	static const int permissions_flags[];
	static const int flags_permissions[];
	static const int seek_flags[];
};

VFS_NS_END

#endif /* VFS_IFILEACCESSOR_H_ */
