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
#ifndef VFS_IFILENAVIGATION_H_
#define VFS_IFILENAVIGATION_H_

#include <QtCore/QString>
#include <QtCore/QModelIndex>
#include "vfs_inodeview.h"


VFS_NS_BEGIN
class PluginsManager;


class IFileNavigation
{
public:
	virtual ~IFileNavigation();

	virtual void viewClosed(INodeView *nodeView) = 0;
	virtual ::History::Entry *viewParent(INodeView *nodeView) = 0;
	virtual ::History::Entry *viewChild(INodeView *nodeView, const QModelIndex &idx) = 0;
	virtual ::History::Entry *viewInNewTab(INodeView *nodeView, const QModelIndex &idx) = 0;
	virtual void viewHistory(INodeView *nodeView, ::History::Entry *entry) = 0;
};

VFS_NS_END

#endif /* VFS_IFILENAVIGATION_H_ */
