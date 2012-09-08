#include "desktop_device_opticaldrive.h"


DESKTOP_NS_BEGIN

OpticalDrive::OpticalDrive(const Id &id,
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

const OpticalDrive::MediaTypeSet &OpticalDrive::mediaTypeSet()
{
	static MediaTypeSet set = MediaTypeSet::fromList(
			MediaTypeList() << Optical
							<< Optical_Cd
							<< Optical_Cd_R
							<< Optical_Cd_Rw
							<< Optical_Dvd
							<< Optical_Dvd_R
							<< Optical_Dvd_Rw
							<< Optical_Dvd_Ram
							<< Optical_Dvd_Plus_R
							<< Optical_Dvd_Plus_Rw
							<< Optical_Dvd_Plus_R_Dl
							<< Optical_Dvd_Plus_Rw_Dl
							<< Optical_Bd
							<< Optical_Bd_R
							<< Optical_Bd_Re
							<< Optical_HdDvd
							<< Optical_HdDvd_R
							<< Optical_HdDvd_Rw
							<< Optical_Mo
							<< Optical_Mrw
							<< Optical_Mrw_W);
	return set;
}

bool OpticalDrive::isOpticalDrive() const
{
	return true;
}

DESKTOP_NS_END
