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
#include "desktop_device.h"


DESKTOP_NS_BEGIN

Device::Device(const Id &id,
			   const QIcon &icon,
			   const QString &label,
			   bool hidden,
			   Device *parent) :
	m_id(id),
	m_icon(icon),
	m_label(label),
	m_hidden(hidden),
	m_parent(parent)
{}

Device::~Device()
{}

bool Device::isDrive() const
{
	return false;
}

bool Device::isHardDrive() const
{
	return false;
}

bool Device::isFlashDrive() const
{
	return false;
}

bool Device::isRemovableDrive() const
{
	return false;
}

bool Device::isOpticalDrive() const
{
	return false;
}

bool Device::isFloppyDrive() const
{
	return false;
}

bool Device::isPartition() const
{
	return false;
}

DESKTOP_NS_END
