#include "desktop_device_floppydrive.h"


DESKTOP_NS_BEGIN

FloppyDrive::FloppyDrive(const Id &id,
  	    				 const QIcon &icon,
  	    				 const QString &label,
  	    				 bool hidden,
  	    				 Device *parent,
  	    				 quint64 size,
  	    				 MediaTypeSet mediaCompatibility,
  	    				 MediaType media,
  	    				 bool ejectable) :
	RemovableDrive(id, icon, label, hidden, parent, size, mediaCompatibility, media, ejectable)
{}

const FloppyDrive::MediaTypeSet &FloppyDrive::mediaTypeSet()
{
	static MediaTypeSet set = MediaTypeSet::fromList(
			MediaTypeList() << Floppy
			                << Floppy_Zip
			                << Floppy_Jaz);
	return set;
}

bool FloppyDrive::isFloppyDrive() const
{
	return true;
}

DESKTOP_NS_END
