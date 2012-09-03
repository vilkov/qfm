#include "desktop_device_opticaldrive.h"


DESKTOP_NS_BEGIN

OpticalDrive::OpticalDrive(const Id &id,
	  	 	 	 	 	   const QIcon &icon,
	  	 	 	 	 	   const QString &label,
	  	 	 	 	 	   bool hidden,
	  	 	 	 	 	   Device *parent,
	  	 	 	 	 	   quint64 size,
	  	 	 	 	 	   MediaType mediaCompatibility,
	  	 	 	 	 	   MediaType media,
	  	 	 	 	 	   bool ejectable) :
	Drive(id, icon, label, hidden, parent, size),
	m_mediaCompatibility(mediaCompatibility),
	m_media(media),
	m_ejectable(ejectable)
{}

bool OpticalDrive::isOpticalDrive() const
{
	return true;
}

DESKTOP_NS_END
