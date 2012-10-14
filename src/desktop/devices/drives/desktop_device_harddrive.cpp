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
#include "desktop_device_harddrive.h"


DESKTOP_NS_BEGIN

HardDrive::HardDrive(const Id &id,
		  	  	     const QIcon &icon,
		  	  	     const QString &label,
		  	  	     bool hidden,
		  	  	     Device *parent,
		  	  	     quint64 size) :
	Drive(id, icon, label, hidden, parent, size)
{}

bool HardDrive::isHardDrive() const
{
	return true;
}

DESKTOP_NS_END
