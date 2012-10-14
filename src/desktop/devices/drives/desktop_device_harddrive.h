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
#ifndef DESKTOP_DEVICE_HARDDRIVE_H_
#define DESKTOP_DEVICE_HARDDRIVE_H_

#include "desktop_device_drive.h"


DESKTOP_NS_BEGIN

class HardDrive : public Drive
{
public:
	struct Details : public Drive::Details
	{
		quint16 rotationRate;
		bool writeCacheEnabled;
		bool canDetach;
		bool canSpindown;
		bool ataSmartIsAvailable;
		bool ataSmartTimeCollected;
		bool ataSmartStatus;
		QByteArray ataSmartBlob;
	};

public:
	HardDrive(const Id &id,
			  const QIcon &icon,
			  const QString &label,
			  bool hidden,
			  Device *parent,
			  quint64 size);

	virtual bool isHardDrive() const;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_HARDDRIVE_H_ */
