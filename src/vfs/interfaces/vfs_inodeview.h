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
#ifndef VFS_INODEVIEW_H_
#define VFS_INODEVIEW_H_

#include <QtCore/QList>
#include <QtCore/QModelIndexList>
#include <QtCore/QAbstractItemModel>
#include <QtGui/QAbstractItemDelegate>
#include <QtGui/QAbstractItemView>
#include <QtGui/QAction>
#include "../vfs_ns.h"
#include "../../history/historyentry.h"


VFS_NS_BEGIN
class INode;


class INodeView
{
public:
    typedef QList<qint32>             Geometry;
    typedef QPair<int, Qt::SortOrder> Sorting;
	typedef QList<QAction*>           MenuActionList;

public:
	virtual ~INodeView();

	virtual INode *node() const = 0;
	virtual INodeView *opposite() const = 0;
	virtual QModelIndex currentIndex() const = 0;
	virtual QModelIndexList selectedIndexes() const = 0;

	virtual void save(::History::Entry *entry) = 0;
	virtual void edit(const QModelIndex &index) = 0;
	virtual void select(const QModelIndex &index) = 0;
	virtual void setNode(INode *node) = 0;
};

VFS_NS_END

#endif /* VFS_INODEVIEW_H_ */
