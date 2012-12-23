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
#include "idm_copycontrolbase.h"

#include <vfs/tools/vfs_uri.h>


IDM_PLUGIN_NS_BEGIN

CopyControlBase::CopyControlBase(ICopyControl::Holder &dest, const IdmContainer &container) :
	m_dest(dest.take()),
	m_container(container),
	m_storage(difference(m_dest->location(), m_container.container()->location()).append(QChar(L'/')))
{}

bool CopyControlBase::isDefault() const
{
	return m_dest->isDefault();
}

IFileInfo::size_type CopyControlBase::freeSpace() const
{
	return m_dest->freeSpace();
}

ICopyControl *CopyControlBase::createControl(INodeView *view) const
{
	return NULL;
}

const Location &CopyControlBase::location() const
{
	return m_dest->location();
}

Location CopyControlBase::location(const IFileInfo *info) const
{
	return m_dest->location(info);
}

Location CopyControlBase::location(const QString &fileName) const
{
	return m_dest->location(fileName);
}

bool CopyControlBase::contains(const QString &fileName) const
{
	return m_dest->contains(fileName);
}

IFileInfo *CopyControlBase::info(const QString &fileName, QString &error) const
{
	return m_dest->info(fileName, error);
}

bool CopyControlBase::remove(const IFileInfo *info, QString &error) const
{
	return m_dest->remove(info, error);
}

bool CopyControlBase::remove(const Location &fileName, QString &error) const
{
    return m_dest->remove(fileName, error);
}

bool CopyControlBase::rename(const IFileInfo *info, const QString &fileName, QString &error) const
{
	return m_dest->rename(info, fileName, error);
}

bool CopyControlBase::move(const IFileContainer *source, const IFileInfo *info, QString &error) const
{
	return m_dest->move(source, info, error);
}

IFileContainer *CopyControlBase::open() const
{
	return m_dest->open();
}

IFileContainer *CopyControlBase::open(const IFileInfo *info, QString &error) const
{
	return m_dest->open(info, error);
}

IFileAccessor *CopyControlBase::open(const IFileInfo *info, int flags, QString &error) const
{
	return m_dest->open(info, flags, error);
}

IFileContainer *CopyControlBase::create(const QString &fileName, QString &error) const
{
	return m_dest->create(fileName, error);
}

IFileAccessor *CopyControlBase::create(const QString &fileName, int flags, QString &error) const
{
	return m_dest->create(fileName, flags, error);
}

const IFileContainerScanner *CopyControlBase::scanner() const
{
	return m_dest->scanner();
}

const IApplications *CopyControlBase::applications() const
{
	return m_dest->applications();
}

INode *CopyControlBase::node() const
{
	return m_dest->node();
}

bool CopyControlBase::start(const Snapshot &files, bool move)
{
	return false;
}

void CopyControlBase::done(bool error)
{
	m_dest->done(error);
}

void CopyControlBase::canceled()
{
	m_dest->canceled();
}

QString CopyControlBase::difference(const QString &path1, const QString &path2)
{
	Uri current(path1);
	Uri other(path2);

	for (Uri::Iterator otherIt = other.begin(), currentIt = current.begin();
		!otherIt.atEnd() && !currentIt.atEnd();
		++otherIt)
	{
		if (*currentIt == *otherIt)
			currentIt = current.erase(currentIt);
		else
			break;
	}

	return current.toString();
}

IDM_PLUGIN_NS_END
