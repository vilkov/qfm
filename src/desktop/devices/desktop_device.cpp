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
