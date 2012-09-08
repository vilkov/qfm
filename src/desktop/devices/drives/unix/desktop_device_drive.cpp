#include "../desktop_device_drive.h"
#include "../../desktop_device_partition.h"
#include "../../../theme/desktop_theme.h"


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

void Drive::addPartition(Partition *partition)
{
	m_partitions.insert(partition->id(), partition);
}

void Drive::removePartition(const Id &id)
{
	m_partitions.remove(id);
}

bool Drive::isDrive() const
{
	return true;
}

QIcon Drive::mediaTypeIcon(MediaType mediaType, int iconSize)
{
	switch (mediaType)
	{
		case Flash:
		case Flash_CompactFlash:
			return Theme::current()->mediaFlash(iconSize);

		case Flash_MemoryStick:
			return Theme::current()->mediaFlashMemoryStick(iconSize);

		case Flash_SmartMedia:
			return Theme::current()->mediaFlashSmartMedia(iconSize);

		case Flash_SecureDigital:
		case Flash_SecureDigitalHighCapacity:
		case Flash_MultiMediaCard:
			return Theme::current()->mediaFlashSdMmc(iconSize);

		case Floppy:
		case Floppy_Zip:
		case Floppy_Jaz:
			return Theme::current()->mediaFloppy(iconSize);

		case Optical:
		case Optical_Cd:
		case Optical_Cd_R:
		case Optical_Cd_Rw:
			return Theme::current()->mediaOptical(iconSize);

		case Optical_Dvd:
		case Optical_Dvd_R:
		case Optical_Dvd_Rw:
		case Optical_Dvd_Ram:
		case Optical_Dvd_Plus_R:
		case Optical_Dvd_Plus_Rw:
		case Optical_Dvd_Plus_R_Dl:
		case Optical_Dvd_Plus_Rw_Dl:
			return Theme::current()->mediaOpticalDvd(iconSize);

		case Optical_Bd:
		case Optical_Bd_R:
		case Optical_Bd_Re:
		case Optical_HdDvd:
		case Optical_HdDvd_R:
		case Optical_HdDvd_Rw:
			return Theme::current()->mediaOpticalBluRay(iconSize);

		case Optical_Mo:
		case Optical_Mrw:
		case Optical_Mrw_W:
			return Theme::current()->mediaOptical(iconSize);

		default:
			return QIcon();
	}
}

Drive::MediaType Drive::stringToMediaType(const QString &media)
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

Drive::MediaTypeSet Drive::stringListToMediaTypeSet(const QStringList &media)
{
	Drive::MediaTypeSet res;

	for (QStringList::size_type i = 0, size = media.size(); i < size; ++i)
		res.insert(stringToMediaType(media.at(i)));

	return res;
}

DESKTOP_NS_END
