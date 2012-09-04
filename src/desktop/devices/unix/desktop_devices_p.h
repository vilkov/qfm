#ifndef DESKTOP_DEVICES_P_H_
#define DESKTOP_DEVICES_P_H_

#include <QtCore/QScopedPointer>
#include "desktop_devices_udisks.h"
#include "../desktop_devices.h"
#include "../desktop_device_partition.h"
#include "../drives/desktop_device_harddrive.h"
#include "../drives/desktop_device_flashdrive.h"
#include "../drives/desktop_device_floppydrive.h"
#include "../drives/desktop_device_opticaldrive.h"
#include "../drives/desktop_device_removabledrive.h"
#include "../../theme/desktop_theme.h"


class DevicesPrivate : public QObject
{
	Q_OBJECT

public:
	DevicesPrivate(::Desktop::Devices *parent) :
		QObject(NULL),
		parent(parent),
		manager(QString::fromLatin1(UD_DBUS_SERVICE),
				QString::fromLatin1(UD_DBUS_PATH),
				QString::fromLatin1(UD_DBUS_INTERFACE_DISKS),
				QDBusConnection::systemBus())
	{
//		qDBusRegisterMetaType<QList<QDBusObjectPath> >();
//		qDBusRegisterMetaType<QVariantMap>();

		bool serviceFound = manager.isValid();

		if (!serviceFound)
		{
			// find out whether it will be activated automatically
			QDBusMessage message = QDBusMessage::createMethodCall(QString::fromLatin1("org.freedesktop.DBus"),
																  QString::fromLatin1("/org/freedesktop/DBus"),
																  QString::fromLatin1("org.freedesktop.DBus"),
																  QString::fromLatin1("ListActivatableNames"));

			QDBusReply<QStringList> reply = QDBusConnection::systemBus().call(message);

			if (reply.isValid() && reply.value().contains(QString::fromLatin1(UD_DBUS_SERVICE)))
			{
				QDBusConnection::systemBus().interface()->startService(QString::fromLatin1(UD_DBUS_SERVICE));
				serviceFound = true;
			}
		}

	    if (serviceFound)
	    {
	        connect(&manager, SIGNAL(DeviceAdded(QDBusObjectPath)),
	                this, SLOT(slotDeviceAdded(QDBusObjectPath)));
	        connect(&manager, SIGNAL(DeviceRemoved(QDBusObjectPath)),
	                this, SLOT(slotDeviceRemoved(QDBusObjectPath)));
	        connect(&manager, SIGNAL(DeviceChanged(QDBusObjectPath)),
	                this, SLOT(slotDeviceChanged(QDBusObjectPath)));
	    }
	}
	~DevicesPrivate()
	{
		qDeleteAll(m_devices);
	}

public:
	static QString driveLabel(const QDBusInterface &interface)
	{
		QString res = interface.property("DevicePresentationName").toString();

		if (res.isEmpty())
			res = interface.property("DriveModel").toString();

		return res;
	}

	static QString partitionLabel(const QDBusInterface &interface, const QStringList &mountPaths)
	{
		QString res = interface.property("DevicePresentationName").toString();

		if (res.isEmpty())
		{
			res = interface.property("PartitionLabel").toString();

			if (res.isEmpty())
			{
				res = interface.property("IdLabel").toString();

				if (res.isEmpty())
					if (interface.property("DeviceIsMounted").toBool())
						res = mountPaths.at(0);
					else
					{
						res = interface.property("DeviceFilePresentation").toString();

						if (res.isEmpty())
							res = interface.property("DeviceFile").toString();
					}
			}
		}

		return res;
	}

	static ::Desktop::Drive *drive(const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;
		QString string = interface.property("DevicePresentationIconName").toString();
		::Desktop::Drive::MediaType type = ::Desktop::Drive::stringToMeduaType(interface.property("DriveMediaCompatibility").toString());

		if (!string.isEmpty())
			icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8());

		if (type >= ::Desktop::Drive::Flash && type <= ::Desktop::Drive::Flash_MultiMediaCard)
		{
			if (icon.isNull())
				icon = ::Desktop::Theme::current()->driveRemovableMediaUsb();

			return new ::Desktop::FlashDrive(path,
											 icon,
											 driveLabel(interface),
											 interface.property("DevicePresentationHide").toBool(),
											 NULL,
											 interface.property("DeviceSize").toULongLong(),
											 type,
											 ::Desktop::Drive::stringToMeduaType(interface.property("DriveMedia").toString()),
											 interface.property("DriveIsMediaEjectable").toBool());
		}
		else if (type >= ::Desktop::Drive::Floppy && type <= ::Desktop::Drive::Floppy_Jaz)
		{
			if (icon.isNull())
				icon = ::Desktop::Theme::current()->driveRemovableMedia();

			return new ::Desktop::FloppyDrive(path,
											  icon,
											  driveLabel(interface),
											  interface.property("DevicePresentationHide").toBool(),
 											  NULL,
											  interface.property("DeviceSize").toULongLong(),
											  type,
											  ::Desktop::Drive::stringToMeduaType(interface.property("DriveMedia").toString()),
											  interface.property("DriveIsMediaEjectable").toBool());
		}
		else if (type >= ::Desktop::Drive::Optical && type <= ::Desktop::Drive::Optical_Mrw_W)
		{
			if (icon.isNull())
				icon = ::Desktop::Theme::current()->driveOptical();

			return new ::Desktop::OpticalDrive(path,
					   	   	   	   	   	   	   icon,
					   	   	   	   	   	   	   driveLabel(interface),
					   	   	   	   	   	   	   interface.property("DevicePresentationHide").toBool(),
											   NULL,
					   	   	   	   	   	   	   interface.property("DeviceSize").toULongLong(),
					   	   	   	   	   	   	   type,
					   	   	   	   	   	   	   ::Desktop::Drive::stringToMeduaType(interface.property("DriveMedia").toString()),
					   	   	   	   	   	   	   interface.property("DriveIsMediaEjectable").toBool());
		}
		else
		{
			if (icon.isNull())
				icon = ::Desktop::Theme::current()->driveHarddisk();

			return new ::Desktop::HardDrive(path,
											icon,
											driveLabel(interface),
											interface.property("DevicePresentationHide").toBool(),
											NULL,
											interface.property("DeviceSize").toULongLong());
		}
	}

	static ::Desktop::Drive *removableDrive(const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;
		QString string = interface.property("DevicePresentationIconName").toString();
		::Desktop::Drive::MediaType type = ::Desktop::Drive::stringToMeduaType(interface.property("DriveMediaCompatibility").toString());

		if (!string.isEmpty())
			icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8());

		if (type >= ::Desktop::Drive::Flash && type <= ::Desktop::Drive::Flash_MultiMediaCard)
		{
			if (icon.isNull())
				icon = interface.property("DeviceIsSystemInternal").toBool() ?
						::Desktop::Theme::current()->driveRemovableMediaUsb() :
						::Desktop::Theme::current()->driveRemovableMediaUsbPendrive();

			return new ::Desktop::FlashDrive(path,
											 icon,
											 driveLabel(interface),
											 interface.property("DevicePresentationHide").toBool(),
											 NULL,
											 interface.property("DeviceSize").toULongLong(),
											 type,
											 ::Desktop::Drive::stringToMeduaType(interface.property("DriveMedia").toString()),
											 interface.property("DriveIsMediaEjectable").toBool());
		}
		else if (type >= ::Desktop::Drive::Floppy && type <= ::Desktop::Drive::Floppy_Jaz)
		{
			if (icon.isNull())
				icon = ::Desktop::Theme::current()->driveRemovableMedia();

			return new ::Desktop::FloppyDrive(path,
											  icon,
											  driveLabel(interface),
											  interface.property("DevicePresentationHide").toBool(),
											  NULL,
											  interface.property("DeviceSize").toULongLong(),
											  type,
											  ::Desktop::Drive::stringToMeduaType(interface.property("DriveMedia").toString()),
											  interface.property("DriveIsMediaEjectable").toBool());
		}
		else if (type >= ::Desktop::Drive::Optical && type <= ::Desktop::Drive::Optical_Mrw_W)
		{
			if (icon.isNull())
				icon = ::Desktop::Theme::current()->driveOptical();

			return new ::Desktop::OpticalDrive(path,
					   	   	   	   	   	   	   icon,
					   	   	   	   	   	   	   driveLabel(interface),
					   	   	   	   	   	   	   interface.property("DevicePresentationHide").toBool(),
											   NULL,
					   	   	   	   	   	   	   interface.property("DeviceSize").toULongLong(),
					   	   	   	   	   	   	   type,
					   	   	   	   	   	   	   ::Desktop::Drive::stringToMeduaType(interface.property("DriveMedia").toString()),
					   	   	   	   	   	   	   interface.property("DriveIsMediaEjectable").toBool());
		}
		else
		{
			if (icon.isNull())
				icon = ::Desktop::Theme::current()->driveRemovableMedia();

			return new ::Desktop::RemovableDrive(path,
											   	 icon,
											   	 driveLabel(interface),
											   	 interface.property("DevicePresentationHide").toBool(),
												 NULL,
											   	 interface.property("DeviceSize").toULongLong(),
											   	 type,
											   	 type,
											   	 interface.property("DriveIsMediaEjectable").toBool());
		}
	}

	static ::Desktop::OpticalDrive *opticalDrive(const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;
		QString string = interface.property("DevicePresentationIconName").toString();

		if (string.isEmpty() ||
			(icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8())).isNull())
		{
			icon = ::Desktop::Theme::current()->driveOptical();
		}

		return new ::Desktop::OpticalDrive(path,
		 	 							   icon,
		 	 							   driveLabel(interface),
		 	 							   interface.property("DevicePresentationHide").toBool(),
		 	 							   NULL,
		 	 							   interface.property("DeviceSize").toULongLong(),
		 	 							   ::Desktop::Drive::stringToMeduaType(interface.property("DriveMediaCompatibility").toString()),
		 	 							   ::Desktop::Drive::stringToMeduaType(interface.property("DriveMedia").toString()),
		 	 							   interface.property("DriveIsMediaEjectable").toBool());
	}

public:
	::Desktop::Device *processDevice(const QString &path, const QDBusInterface &interface)
	{
		if (!m_devices.contains(path))
			if (interface.property("DeviceIsOpticalDisc").toBool())
				return opticalDrive(path, interface);
			else if (interface.property("DeviceIsRemovable").toBool())
				return removableDrive(path, interface);
			else if (interface.property("DeviceIsDrive").toBool())
				return drive(path, interface);
			else if (interface.property("DeviceIsPartition").toBool())
				partition(path, interface);

		return NULL;
	}

	void partition(const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;
		QString string;
		QScopedPointer< ::Desktop::Partition > partition;
		::Desktop::Device *parent = m_devices.value(string = interface.property("PartitionSlave").value<QDBusObjectPath>().path());

		if (parent == NULL)
		{
			QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), string, QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

			if (interface.isValid() && (parent = processDevice(string, interface)))
				this->parent->slotDeviceAdded(m_devices[string] = parent);
		}

		if (parent)
		{
			QStringList mountPaths(interface.property("DeviceMountPaths").toStringList());
			string = interface.property("DevicePresentationIconName").toString();

			if (string.isEmpty() ||
				(icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8())).isNull())
			{
				icon = parent->icon();
			}

			partition.reset(new ::Desktop::Partition(path,
													 icon,
													 partitionLabel(interface, mountPaths),
													 interface.property("DevicePresentationHide").toBool(),
													 static_cast< ::Desktop::Drive *>(parent),
													 interface.property("PartitionNumber").toInt(),
													 interface.property("PartitionSize").toULongLong(),
													 mountPaths));

			parent->as< ::Desktop::Drive >()->addPartition(partition.take());
		}
	}

	void refresh()
	{
		typedef QList<QDBusObjectPath> List;
		typedef QDBusReply<List>       Reply;

		QString string;
		::Desktop::Device *device;
		Reply reply = manager.call(QString::fromLatin1("EnumerateDevices"));

		if (reply.isValid())
		{
			List list(reply.value());

			for (List::size_type i = 0, size = list.size(); i < size; ++i)
			{
				QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), string = list.at(i).path(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

				if (interface.isValid() && (device = processDevice(string, interface)))
					parent->slotDeviceAdded(m_devices[string] = device);
			}
		}
	}

public Q_SLOTS:
	void slotDeviceAdded(const QDBusObjectPath &path)
	{
//		QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), path.path(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());
//
//		if (interface.isValid() && interface.property("DeviceIsPartition").toBool())
//		{
//			::Desktop::Device &dev = m_container[path.path()];
//			parent->slotDeviceAdded(dev = device(path.path(), interface));
//		}
	}

	void slotDeviceRemoved(const QDBusObjectPath &path)
	{
//		QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), path.path(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());
//
//		if (interface.isValid() && interface.property("DeviceIsPartition").toBool())
//			parent->slotDeviceRemoved(path.path());
	}

	void slotDeviceChanged(const QDBusObjectPath &path)
	{
//		QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), path.path(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());
//
//		if (interface.isValid() && interface.property("DeviceIsPartition").toBool())
//			parent->slotDeviceChanged(path.path());
	}

public:
	::Desktop::Devices::Container m_devices;
	::Desktop::Devices *parent;
	QDBusInterface manager;
};

#endif /* DESKTOP_DEVICES_P_H_ */
