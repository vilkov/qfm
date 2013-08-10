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
#ifndef VFS_IAPPLICATION_H_
#define VFS_IAPPLICATION_H_

#include <QtGui/QIcon>
#include <QtCore/QString>
#include "../vfs_ns.h"


VFS_NS_BEGIN
class IFileInfo;
class IFileContainer;


class IApplication
{
public:
	virtual ~IApplication();

	virtual const QIcon &icon() const = 0;
	virtual const QString &name() const = 0;
	virtual const QString &genericName() const = 0;
	virtual const QString &description() const = 0;

	virtual bool exec(const IFileContainer *container, const IFileInfo *file, QString &error) const = 0;
};

VFS_NS_END

#endif /* VFS_IAPPLICATION_H_ */
