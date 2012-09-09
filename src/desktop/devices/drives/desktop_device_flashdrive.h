#ifndef DESKTOP_DEVICE_FLASHDRIVE_H_
#define DESKTOP_DEVICE_FLASHDRIVE_H_

#include "desktop_device_removabledrive.h"


DESKTOP_NS_BEGIN

class FlashDrive : public RemovableDrive
{
public:
	FlashDrive(const Id &id,
		  	   const QIcon &icon,
		  	   const QString &label,
		  	   bool hidden,
		  	   Device *parent,
		  	   quint64 size,
		  	   MediaTypeSet mediaCompatibility,
		   	   MediaType media,
		   	   bool detachable);

	static const MediaTypeSet &mediaTypeSet();
	virtual bool isFlashDrive() const;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_FLASHDRIVE_H_ */
