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
#include "idm_invalidfileinfo.h"
#include <desktop/theme/desktop_theme.h>


IDM_PLUGIN_NS_BEGIN

InvalidInfo::InvalidInfo(const QString &fileName) :
	m_fileName(IFileInfo::location(fileName, fileName.toUtf8())),
	m_icon(::Desktop::Theme::current()->missingIcon())
{}

bool InvalidInfo::isDir() const
{
	return false;
}

bool InvalidInfo::isFile() const
{
	return false;
}

bool InvalidInfo::isLink() const
{
	return false;
}

InvalidInfo::size_type InvalidInfo::fileSize() const
{
	return 0;
}

const Location &InvalidInfo::fileName() const
{
	return m_fileName;
}

const IFileType *InvalidInfo::fileType() const
{
	return this;
}

const QDateTime &InvalidInfo::lastModified() const
{
	return m_lastModified;
}

int InvalidInfo::permissions() const
{
	return 0;
}

const FileTypeId &InvalidInfo::id() const
{
	return m_id;
}

const QIcon &InvalidInfo::icon() const
{
	return m_icon;
}

const QString &InvalidInfo::name() const
{
	return m_name;
}

const QString &InvalidInfo::description() const
{
	return m_description;
}

IDM_PLUGIN_NS_END
