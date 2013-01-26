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
#ifndef VFS_ICOPYCONTROL_H_
#define VFS_ICOPYCONTROL_H_

#include "vfs_ifilecontainer.h"
#include "../containers/vfs_snapshot.h"


VFS_NS_BEGIN
class INode;


class ICopyControl : public IFileContainer
{
public:
	typedef ::Tools::Memory::ScopedPointer<ICopyControl> Holder;

public:
	virtual INode *node() const = 0;

	virtual bool start(const Snapshot &files, bool move) = 0;
	virtual void done(bool error) = 0;
	virtual void canceled() = 0;
};

VFS_NS_END

#endif /* VFS_ICOPYCONTROL_H_ */
