#include "desktop_device_flashdrive.h"


DESKTOP_NS_BEGIN

FlashDrive::FlashDrive(const Id &id,
	  	   	   	   	   const QIcon &icon,
	  	   	   	   	   const QString &label,
	  	   	   	   	   bool hidden,
	  	   	   	   	   Device *parent,
	  	   	   	   	   quint64 size,
	  	   	   	   	   MediaType mediaCompatibility,
	  	   	   	   	   MediaType media,
	  	   	   	   	   bool ejectable) :
	RemovableDrive(id, icon, label, hidden, parent, size, mediaCompatibility, media, ejectable)
{}

bool FlashDrive::isFlashDrive() const
{
	return true;
}

DESKTOP_NS_END
