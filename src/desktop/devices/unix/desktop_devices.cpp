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
#include "../desktop_devices.h"
#include "desktop_devices_p.h"


DESKTOP_NS_BEGIN

static Devices *s_instance;


Devices::Devices() :
	d(new DevicesPrivate(this))
{
	Q_ASSERT(s_instance == NULL);
	s_instance = this;
	d->refresh();
}

Devices::~Devices()
{
	delete d;
}

Devices::const_iterator Devices::begin() const
{
	return d->m_topLevelDevices.constBegin();
}

Devices::const_iterator Devices::end() const
{
	return d->m_topLevelDevices.constEnd();
}

Devices *Devices::instance()
{
	Q_ASSERT(s_instance);
	return s_instance;
}

void Devices::slotDeviceAdded(const Device *device)
{
	PUBLISH_EVENT_1(DeviceAdded, device);
}

void Devices::slotDeviceRemoved(const Device *device)
{
	PUBLISH_EVENT_1(DeviceRemoved, device);
}

void Devices::slotDeviceChanged(const Device *device)
{
	PUBLISH_EVENT_1(DeviceChanged, device);
}

DESKTOP_NS_END
