#include "../desktop_device_drive.h"
#include "../../desktop_device_partition.h"


DESKTOP_NS_BEGIN

Drive::Drive(const Id &id,
		  	 const QIcon &icon,
		  	 const QString &label,
		  	 bool hidden,
		  	 Device *parent,
		  	 quint64 size) :
	Device(id, icon, label, hidden, parent),
	m_size(size)
{}

Drive::~Drive()
{
	qDeleteAll(m_partitions);
}

bool Drive::isDrive() const
{
	return true;
}

Drive::MediaType Drive::stringToMeduaType(const QString &media)
{
	static const QString types[NoMedia] =
	{
		QString::fromLatin1("flash"),
		QString::fromLatin1("flash_cf"),
		QString::fromLatin1("flash_ms"),
		QString::fromLatin1("flash_sm"),
		QString::fromLatin1("flash_sd"),
		QString::fromLatin1("flash_sdhc"),
		QString::fromLatin1("flash_mmc"),
		QString::fromLatin1("floppy"),
		QString::fromLatin1("floppy_zip"),
		QString::fromLatin1("floppy_jaz"),
		QString::fromLatin1("optical"),
		QString::fromLatin1("optical_cd"),
		QString::fromLatin1("optical_cd_r"),
		QString::fromLatin1("optical_cd_rw"),
		QString::fromLatin1("optical_dvd"),
		QString::fromLatin1("optical_dvd_r"),
		QString::fromLatin1("optical_dvd_rw"),
		QString::fromLatin1("optical_dvd_ram"),
		QString::fromLatin1("optical_dvd_plus_r"),
		QString::fromLatin1("optical_dvd_plus_rw"),
		QString::fromLatin1("optical_dvd_plus_r_dl"),
		QString::fromLatin1("optical_dvd_plus_rw_dl"),
		QString::fromLatin1("optical_bd"),
		QString::fromLatin1("optical_bd_r"),
		QString::fromLatin1("optical_bd_re"),
		QString::fromLatin1("optical_hddvd"),
		QString::fromLatin1("optical_hddvd_r"),
		QString::fromLatin1("optical_hddvd_rw"),
		QString::fromLatin1("optical_mo"),
		QString::fromLatin1("optical_mrw"),
		QString::fromLatin1("optical_mrw_w")
	};

	for (int i = Flash; i < NoMedia; ++i)
		if (media == types[i])
			return static_cast<MediaType>(i);

	return NoMedia;
}

DESKTOP_NS_END
