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
#ifndef DESKTOP_DEVICES_H_
#define DESKTOP_DEVICES_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include <tools/events/publisher-subscribers/events_publisher.h>
#include "desktop_device.h"


class DevicesPrivate;


DESKTOP_NS_BEGIN

class Devices : public ::Tools::Events::Publisher
{
public:
	typedef QMap<Device::Id, Device *> Container;
	typedef Container::const_iterator  const_iterator;

public:
	enum Events
	{
		DeviceAdded,
		DeviceRemoved,
		DeviceChanged
	};

	DECLARE_EVENT_SUBSCRIBER_1(DeviceAdded, const Device *);
	DECLARE_EVENT_SUBSCRIBER_1(DeviceRemoved, const Device *);
	DECLARE_EVENT_SUBSCRIBER_1(DeviceChanged, const Device *);

public:
	Devices();
	virtual ~Devices();

	const_iterator begin() const;
	const_iterator end() const;

	static Devices *instance();

private:
	void slotDeviceAdded(const Device *device);
	void slotDeviceRemoved(const Device *device);
	void slotDeviceChanged(const Device *device);

private:
	friend class ::DevicesPrivate;
	::DevicesPrivate *d;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICES_H_ */
