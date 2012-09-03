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
