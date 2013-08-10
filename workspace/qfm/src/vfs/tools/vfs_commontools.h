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
#ifndef VFS_COMMONTOOLS_H_
#define VFS_COMMONTOOLS_H_

#include <QtGui/QWidget>
#include <QtCore/QString>
#include <tools/strings/hierarchy/stringshierarchytree.h>
#include "../vfs_ns.h"


VFS_NS_BEGIN

struct Tools
{
	static QString humanReadableTime(quint64 msecs);
	static QString humanReadableSize(quint64 size);
	static QString humanReadableShortSize(quint64 size);

	class DestinationFromPathList : public ::Tools::Strings::Hierarchy::Tree
	{
	public:
		DestinationFromPathList();

		QString choose(const QString &title, QWidget *parent) const;
		void add(const QString &file, qint32 excludeLastEntries);
		void add(const QString &file);
	};
};

VFS_NS_END

#endif /* VFS_COMMONTOOLS_H_ */
