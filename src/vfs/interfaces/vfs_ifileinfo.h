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
#ifndef VFS_IFILEINFO_H_
#define VFS_IFILEINFO_H_

#include <QtCore/QDateTime>
#include <QtCore/QTextCodec>
#include <tools/pointers/pscopedpointer.h>
#include "vfs_ifiletype.h"
#include "../location/vfs_location.h"


VFS_NS_BEGIN

class IFileInfo
{
public:
	typedef PScopedPointer<IFileInfo> Holder;
	typedef qint64                    size_type;
    enum Permissions
    {
        Read = 0x0004, Write = 0x0002, Exec = 0x0001
    };

public:
    virtual ~IFileInfo();

	virtual bool isDir() const = 0;
	virtual bool isFile() const = 0;
	virtual bool isLink() const = 0;
	virtual size_type fileSize() const = 0;
	virtual const Location &fileName() const = 0;
	virtual const IFileType *fileType() const = 0;
	virtual const QDateTime &lastModified() const = 0;
	virtual int permissions() const = 0;

protected:
	static Location location(const QString &label, const QByteArray &location)
	{
		return Location(label, location);
	}

	static Location location(const QByteArray &location, const QTextCodec *codec)
	{
		return Location(codec->toUnicode(location), location);
	}

	static Location location(const QString &label, const QTextCodec *codec)
	{
		return Location(label, codec->fromUnicode(label));
	}
};

VFS_NS_END

#endif /* VFS_IFILEINFO_H_ */
