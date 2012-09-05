#ifndef DESKTOP_DEVICE_DRIVE_H_
#define DESKTOP_DEVICE_DRIVE_H_

#include <QtCore/QLinkedList>
#include "../desktop_device.h"


DESKTOP_NS_BEGIN
class Partition;


/**
 * http://hal.freedesktop.org/docs/udisks/Device.html
 */
class Drive : public Device
{
public:
	typedef QLinkedList<Partition *> Container;

	enum MediaType
	{
		Flash, // Flash Card
		Flash_CompactFlash, // CompactFlash
		Flash_MemoryStick, // MemoryStick
		Flash_SmartMedia, // SmartMedia
		Flash_SecureDigital, // Secure Digital
		Flash_SecureDigitalHighCapacity, // Secure Digital High-Capacity
		Flash_MultiMediaCard, // MultiMediaCard
		Floppy, // Floppy Disk
		Floppy_Zip, // Zip Disk
		Floppy_Jaz, // Jaz Disk
		Optical, // Optical Disc
		Optical_Cd, // Compact Disc
		Optical_Cd_R, // Compact Disc Recordable
		Optical_Cd_Rw, // Compact Disc Rewritable
		Optical_Dvd, // Digital Versatile Disc
		Optical_Dvd_R, // DVD-R
		Optical_Dvd_Rw, // DVD-RW
		Optical_Dvd_Ram, // DVD-RAM
		Optical_Dvd_Plus_R, // DVD+R
		Optical_Dvd_Plus_Rw, // DVD+RW
		Optical_Dvd_Plus_R_Dl, // DVD+R Dual Layer
		Optical_Dvd_Plus_Rw_Dl, // DVD+RW Dual Layer
		Optical_Bd, // Bluray Disc
		Optical_Bd_R, // BluRay Recordable
		Optical_Bd_Re, // BluRay Rewritable
		Optical_HdDvd, // HD DVD
		Optical_HdDvd_R, // HD DVD Recordable
		Optical_HdDvd_Rw, // HD DVD Rewritable
		Optical_Mo, // Magneto Optical
		Optical_Mrw, // Can read Mount Rainer media
		Optical_Mrw_W, // Can write Mount Rainer media
		NoMedia
	};

	enum ConnectionInterface
	{
		Virtual, // Device is a composite device e.g. Software RAID or similar
		Ata, // Connected via ATA
		Ata_Serial, // Connected via Serial ATA
		Ata_Serial_eSata, // Connected via eSATA
		Ata_Parallel, // Connected via Parallel ATA
		Scsi, // Connected via SCSI
		Usb, // Connected via the Universal Serial Bus
		Firewire, // Connected via Firewire
		Sdio, // Connected via SDIO
		Platform, // Part of the platform, e.g. PC floppy drive
		Unknown
	};

	struct Details
	{
		QString vendor;
		QString revision;
		QString serial;
		QString wwn;
		ConnectionInterface interface;
		quint32 connectionSpeed; // in bits per second
	};

public:
	Drive(const Id &id,
		  const QIcon &icon,
		  const QString &label,
		  bool hidden,
		  Device *parent,
		  quint64 size);
	virtual ~Drive();

	quint64 size() const { return m_size; }
	const Container &partitions() const { return m_partitions; }

	void addPartition(Partition *partition) { m_partitions.push_back(partition); }
	void removePartition(Partition *partition) { m_partitions.removeOne(partition); }

	virtual bool isDrive() const;

	static MediaType stringToMeduaType(const QString &media);

private:
	QString m_model;
	quint64 m_size;
	Container m_partitions;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_DRIVE_H_ */
