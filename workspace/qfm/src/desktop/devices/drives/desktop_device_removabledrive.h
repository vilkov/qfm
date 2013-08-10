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
#ifndef DESKTOP_DEVICE_REMOVABLEDRIVE_H_
#define DESKTOP_DEVICE_REMOVABLEDRIVE_H_

#include <QtCore/QCoreApplication>
#include "desktop_device_drive.h"


DESKTOP_NS_BEGIN

class RemovableDrive : public Drive
{
	Q_DECLARE_TR_FUNCTIONS(RemovableDrives)

public:
	struct Details : public Drive::Details
	{
		bool writeCacheEnabled;
		bool canDetach;
		bool canSpindown;
	};

public:
	RemovableDrive(const Id &id,
			  	   const QIcon &icon,
			  	   const QString &label,
			  	   bool hidden,
			  	   Device *parent,
			  	   quint64 size,
			   	   MediaTypeSet mediaCompatibility,
			   	   MediaType media,
			   	   bool detachable);

	const MediaTypeSet &mediaCompatibility() const { return m_mediaCompatibility; }

	MediaType media() const { return m_media; }
	void setMedia(MediaType media) { m_media = media; }

	bool isDetachable() const { return m_detachable; }
	void setDetachable(bool detachable) { m_detachable = detachable; }

	virtual bool isRemovableDrive() const;

	bool detach(QString &error);
	bool eject(QString &error);

private:
	MediaTypeSet m_mediaCompatibility;
	MediaType m_media;
	bool m_detachable;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_REMOVABLEDRIVE_H_ */
