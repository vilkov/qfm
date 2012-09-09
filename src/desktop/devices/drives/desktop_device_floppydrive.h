#ifndef DESKTOP_DEVICE_FLOPPYDRIVE_H_
#define DESKTOP_DEVICE_FLOPPYDRIVE_H_

#include "desktop_device_removabledrive.h"


DESKTOP_NS_BEGIN

class FloppyDrive : public RemovableDrive
{
public:
	FloppyDrive(const Id &id,
		  	    const QIcon &icon,
		  	    const QString &label,
		  	    bool hidden,
		  	    Device *parent,
		  	    quint64 size,
		  	    MediaTypeSet mediaCompatibility,
		  	    MediaType media,
		  	    bool detachable);

	static const MediaTypeSet &mediaTypeSet();
	virtual bool isFloppyDrive() const;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_FLOPPYDRIVE_H_ */
