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
#ifndef VFS_INODE_H_
#define VFS_INODE_H_

#include "vfs_ifileoperations.h"
#include "vfs_ifilenavigation.h"


class QKeyEvent;


VFS_NS_BEGIN

class INode : public IFileOperations, public IFileNavigation
{
public:
    typedef INodeView::Geometry Geometry;
    typedef INodeView::Sorting  Sorting;

public:
	virtual ~INode();

    virtual void refresh() = 0;
	virtual QString title() const = 0;
	virtual QString location() const = 0;
	virtual QString fileName(const QModelIndex &index) const = 0;
	virtual bool shortcut(INodeView *view, QKeyEvent *event) = 0;

	virtual Sorting sorting() const = 0;
	virtual Geometry geometry() const = 0;

	virtual int columnsCount() const = 0;
	virtual QAbstractItemModel *model() const = 0;
	virtual QAbstractItemDelegate *delegate() const = 0;
	virtual const INodeView::MenuActionList &actions() const = 0;
	virtual QAbstractItemView::SelectionMode selectionMode() const = 0;
	virtual void menuAction(INodeView *view, QAction *action) = 0;

private:
	friend class Node;
	friend class TasksNode;
	virtual void addLinks(qint32 count) = 0;
	virtual void removeLink() = 0;
};

VFS_NS_END

#endif /* VFS_INODE_H_ */
