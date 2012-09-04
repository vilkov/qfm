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
	return d->m_devices.constBegin();
}

Devices::const_iterator Devices::end() const
{
	return d->m_devices.constEnd();
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
