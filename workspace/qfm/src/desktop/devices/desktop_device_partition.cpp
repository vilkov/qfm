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
#include "desktop_device_partition.h"
#include "drives/desktop_device_drive.h"


DESKTOP_NS_BEGIN

Partition::Partition(const Id &id,
		  	  	  	 const QIcon &icon,
		  	  	  	 const QString &label,
		  	  	  	 bool hidden,
		  	  	  	 Drive *parent,
		  	  	  	 int number,
		  	  	  	 quint64 size,
		  	  	  	 const QStringList &mountPaths) :
	Device(id, icon, label, hidden, parent),
	m_mountPaths(mountPaths),
	m_number(number),
	m_size(size)
{}

bool Partition::isPartition() const
{
	return true;
}

bool Partition::mount(QString &error)
{
	error = tr("Not implemented");
	return false;
}

bool Partition::unmount(QString &error)
{
	error = tr("Not implemented");
	return false;
}

DESKTOP_NS_END
