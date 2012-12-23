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
#include "arc_libunrar_filecontainer.h"
#include "../../../default/interfaces/default_fileinfo.h"
#include "../../../default/interfaces/default_filecontainer.h"


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

IFileContainer *FileContainer::create(const IFileContainer *container, const IFileInfo *file, QString &error)
{
	if (container->isDefault())
	{
		IFileContainer::Holder localContainer(container->open());

		if (localContainer)
			return new FileContainer(localContainer, localContainer->location(file));
	}

	return NULL;
}

QString FileContainer::extractDirectoryName(const IFileInfo *file)
{
	QString fileName = file->fileName();
	return fileName.mid(0, fileName.indexOf(QChar(L'.')));
}

QString FileContainer::extractArchivedFileName(const IFileInfo *file)
{
	QString fileName = file->fileName();
	return fileName.mid(fileName.lastIndexOf(QChar(L'/')) + 1);
}

bool FileContainer::isDefault() const
{
	return false;
}

IFileInfo::size_type FileContainer::freeSpace() const
{
	return m_data->container->freeSpace();
}

ICopyControl *FileContainer::createControl(INodeView *view) const
{
	return NULL;
}

const Location &FileContainer::location() const
{
	return m_data->location;
}

Location FileContainer::location(const IFileInfo *info) const
{
	return Location();
}

Location FileContainer::location(const QString &fileName) const
{
	return Location();
}

bool FileContainer::contains(const QString &fileName) const
{
	return false;
}

IFileInfo *FileContainer::info(const QString &fileName, QString &error) const
{
	return NULL;
}

bool FileContainer::remove(const IFileInfo *info, QString &error) const
{
	return false;
}

bool FileContainer::remove(const Location &fileName, QString &error) const
{
    return false;
}

bool FileContainer::rename(const IFileInfo *info, const QString &fileName, QString &error) const
{
	return false;
}

bool FileContainer::move(const IFileContainer *source, const IFileInfo *info, QString &error) const
{
	return false;
}

IFileContainer *FileContainer::open() const
{
	return new FileContainer(*this);
}

IFileContainer *FileContainer::create(const QString &fileName, QString &error) const
{
	return NULL;
}

IFileAccessor *FileContainer::create(const QString &fileName, int flags, QString &error) const
{
	return NULL;
}

IFileContainer *FileContainer::open(const IFileInfo *info, QString &error) const
{
	return NULL;
}

IFileAccessor *FileContainer::open(const IFileInfo *info, int flags, QString &error) const
{
	return NULL;
}

const IFileContainerScanner *FileContainer::scanner() const
{
	return &m_data->scanner;
}

const IApplications *FileContainer::applications() const
{
	return m_data->container->applications();
}

FileContainer::FileContainer(IFileContainer::Holder &container, const Location &location) :
	m_data(new Data(container, location))
{}

ARC_PLUGIN_LIBUNRAR_NS_END
