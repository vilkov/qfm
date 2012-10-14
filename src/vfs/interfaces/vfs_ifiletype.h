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
#ifndef VFS_IFILETYPE_H_
#define VFS_IFILETYPE_H_

#include <QtGui/QIcon>
#include <QtCore/QString>
#include "../vfs_ns.h"
#include "../filetypeinfo/vfs_filetypeinfo.h"


VFS_NS_BEGIN

class IFileType
{
public:
	virtual ~IFileType();

	virtual const FileTypeId &id() const = 0;
	virtual const QIcon &icon() const = 0;
	virtual const QString &name() const = 0;
	virtual const QString &description() const = 0;
};

VFS_NS_END

#endif /* VFS_IFILETYPE_H_ */
