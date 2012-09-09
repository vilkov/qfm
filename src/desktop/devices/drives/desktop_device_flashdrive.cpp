#include "desktop_device_flashdrive.h"


DESKTOP_NS_BEGIN

FlashDrive::FlashDrive(const Id &id,
	  	   	   	   	   const QIcon &icon,
	  	   	   	   	   const QString &label,
	  	   	   	   	   bool hidden,
	  	   	   	   	   Device *parent,
	  	   	   	   	   quint64 size,
	  	   	   	   	   MediaTypeSet mediaCompatibility,
	  	   	   	   	   MediaType media,
	  	   	   	   	   bool detachable) :
	RemovableDrive(id, icon, label, hidden, parent, size, mediaCompatibility, media, detachable)
{}

const FlashDrive::MediaTypeSet &FlashDrive::mediaTypeSet()
{
	static MediaTypeSet set = MediaTypeSet::fromList(
			MediaTypeList() << Flash
			                << Flash_CompactFlash
			                << Flash_MemoryStick
			                << Flash_SmartMedia
			                << Flash_SecureDigital
			                << Flash_SecureDigitalHighCapacity
			                << Flash_MultiMediaCard);
	return set;
}

bool FlashDrive::isFlashDrive() const
{
	return true;
}

DESKTOP_NS_END
