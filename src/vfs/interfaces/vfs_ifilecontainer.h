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
#ifndef VFS_IFILECONTAINER_H_
#define VFS_IFILECONTAINER_H_

#include "vfs_iapplications.h"
#include "vfs_ifileaccessor.h"
#include "vfs_ifilecontainerscanner.h"
#include "../location/vfs_location.h"


VFS_NS_BEGIN
class ICopyControl;
class INodeView;


class IFileContainer
{
public:
	typedef PScopedPointer<IFileContainer> Holder;

public:
	virtual ~IFileContainer();

	virtual bool isDefault() const = 0;
	virtual IFileInfo::size_type freeSpace() const = 0;
	virtual ICopyControl *createControl(INodeView *view) const = 0;

	virtual const Location &location() const = 0;
	virtual Location location(const IFileInfo *info) const = 0;
	virtual Location location(const QString &fileName) const = 0;

	virtual bool contains(const QString &fileName) const = 0;
	virtual IFileInfo *info(const QString &fileName, QString &error) const = 0;

	virtual bool remove(const IFileInfo *info, QString &error) const = 0;
    virtual bool remove(const Location &fileName, QString &error) const = 0;
	virtual bool rename(const IFileInfo *info, const QString &fileName, QString &error) const = 0;
	virtual bool move(const IFileContainer *source, const IFileInfo *info, QString &error) const = 0;

	virtual IFileContainer *open() const = 0;
	virtual IFileContainer *open(const IFileInfo *info, QString &error) const = 0;
	virtual IFileAccessor *open(const IFileInfo *info, int flags, QString &error) const = 0;
	virtual IFileContainer *create(const QString &fileName, QString &error) const = 0;
	virtual IFileAccessor *create(const QString &fileName, int flags, QString &error) const = 0;

	virtual const IFileContainerScanner *scanner() const = 0;
	virtual const IApplications *applications() const = 0;

protected:
	static Location location(const QString &label, const QByteArray &location)
	{
		return Location(label, location);
	}
};

VFS_NS_END

#endif /* VFS_IFILECONTAINER_H_ */
