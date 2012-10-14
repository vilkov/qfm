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
#include "arc_libarchive_fileinfo.h"
#include "../../../default/interfaces/default_fileinfo.h"


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

Info::Info()
{}

Info::Info(const Data &data, bool isDir) :
	m_data(data),
	m_isDir(isDir),
	m_fileTypeInfo(Default::Info::fileTypeInfoFromFileName(m_data.fileName, m_isDir))
{}

bool Info::isDir() const
{
	return m_isDir;
}

bool Info::isFile() const
{
	return !m_isDir;
}

bool Info::isLink() const
{
	return false;
}

Info::size_type Info::fileSize() const
{
	return m_data.fileSize;
}

const Location &Info::fileName() const
{
	return m_data.fileName;
}

const IFileType *Info::fileType() const
{
	return this;
}

const QDateTime &Info::lastModified() const
{
	return m_data.lastModified;
}

int Info::permissions() const
{
	return Read;
}

const FileTypeId &Info::id() const
{
	return m_fileTypeInfo.id;
}

const QIcon &Info::icon() const
{
	return m_fileTypeInfo.icon;
}

const QString &Info::name() const
{
	return m_fileTypeInfo.name;
}

const QString &Info::description() const
{
	return m_fileTypeInfo.description;
}

Location Info::location(const QByteArray &location)
{
	return IFileInfo::location(QString::fromUtf8(location), location);
}

ARC_PLUGIN_LIBARCHIVE_NS_END
