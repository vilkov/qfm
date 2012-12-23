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
#ifndef VFS_ACTION_H_
#define VFS_ACTION_H_

#include <QtCore/QList>
#include <QtGui/QAction>
#include "../model/items/vfs_item.h"
#include "../interfaces/vfs_ifilecontainer.h"


VFS_NS_BEGIN

class Action
{
public:
	typedef QPair<Item::Holder, const IFileInfo *> FileItem;
	typedef QList<FileItem>                        FilesList;

public:
	Action(const QIcon &icon, const QString &text);
	virtual ~Action();

	const QAction *action() const { return &m_action; }

	static const Action *fromQAction(const QAction *action);
	static Action *fromQAction(QAction *action);

	virtual bool isAsynchronous() const = 0;

private:
	QAction m_action;
};

VFS_NS_END

#endif /* VFS_ACTION_H_ */
