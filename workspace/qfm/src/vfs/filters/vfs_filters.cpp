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
#include "vfs_filters.h"


VFS_NS_BEGIN

Filters::~Filters()
{
	qDeleteAll(m_filters);
}

bool Filters::match(const IFileInfo *info) const
{
	for (Container::const_iterator i = m_filters.constBegin(), end = m_filters.constEnd(); i != end; ++i)
		if (!(*i)->match(info))
			return false;

	return true;
}

bool FileNameFilter::match(const IFileInfo *info) const
{
	return m_pattern.exactMatch(info->fileName());
}

bool ModificationTimeFilterRange::match(const IFileInfo *info) const
{
	return DateFilterRange::match(info->lastModified());
}

VFS_NS_END
