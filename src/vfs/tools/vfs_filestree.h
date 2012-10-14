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
#ifndef VFS_FILESTREE_H_
#define VFS_FILESTREE_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include "../interfaces/vfs_ifilecontainer.h"


VFS_NS_BEGIN

class FilesTree
{
public:
	FilesTree(const IFileContainer *root, bool createSubfolders) :
		m_root(root),
		m_createSubfolders(createSubfolders)
	{}
	~FilesTree();

	const IFileContainer *open(const QString &filePath, bool isPathToDirectory, QString &error);

private:
	class Directory;
	typedef QMap<QString, Directory *> Directories;

	class Directory
	{
	public:
		Directory(IFileContainer::Holder &control) :
			control(control.take())
		{}
		~Directory();

		IFileContainer::Holder control;
		Directories files;
	};

private:
	inline const QChar *strchr(const QChar *string, QChar c) const;

private:
	const IFileContainer *m_root;
	bool m_createSubfolders;
	Directories m_files;
};

VFS_NS_END

#endif /* VFS_FILESTREE_H_ */
