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
#ifndef DESKTOP_CONTEXTMENUFACTORY_H_
#define DESKTOP_CONTEXTMENUFACTORY_H_

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtGui/QMenu>

#include <vfs/actions/vfs_fileaction.h>
#include <vfs/filetypeinfo/vfs_filetypeinfo.h>

#include "../desktop_ns.h"


DESKTOP_NS_BEGIN

class ContextMenuFactory
{
public:
	typedef ::VFS::FileTypeId                FileTypeId;
	typedef QList<FileTypeId>                FileTypesList;
	typedef QList<const ::VFS::FileAction *> FileActionsList;

	enum Type
	{
		SingleFolder,
		SingleFile,
		MultipleFilesOrFolders,
		AnyFilesOrFolders,

		/* Internal use only! */
		TypeCount
	};

public:
	ContextMenuFactory();

	FileActionsList actions(Type type) { return m_actions[type]; }
	FileActionsList actions(const FileTypeId &type) { return m_files.value(type); }

	void registerAction(const ::VFS::FileAction *action, Type type);
	void registerAction(const ::VFS::FileAction *action, const FileTypesList &fileTypes);

private:
	typedef QMap<FileTypeId, FileActionsList> FileActionsMap;

private:
	FileActionsMap m_files;
	FileActionsList m_actions[TypeCount];
};

DESKTOP_NS_END

#endif /* DESKTOP_CONTEXTMENUFACTORY_H_ */
