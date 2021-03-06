/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DESKTOP_DEVICES_P_H_
#define DESKTOP_DEVICES_P_H_

#include <tools/memory/memory_scopedpointer.h>
#include "desktop_devices_udisks.h"
#include "desktop_device_partition_unix.h"
#include "../desktop_devices.h"
#include "../drives/desktop_device_harddrive.h"
#include "../drives/desktop_device_flashdrive.h"
#include "../drives/desktop_device_floppydrive.h"
#include "../drives/desktop_device_opticaldrive.h"
#include "../drives/desktop_device_removabledrive.h"


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
		qDeleteAll(m_topLevelDevices);
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

	static QIcon mediaTypeIconByConnectionInterface(const QDBusInterface &interface)
	{
		static const QString usbInterface = QString::fromLatin1("usb");

		if (interface.property("DriveConnectionInterface").toString() == usbInterface)
			return ::Desktop::Theme::current()->driveRemovableMediaUsb();
		else
			return ::Desktop::Theme::current()->driveRemovableMedia();
	}

	::Desktop::Drive *drive(QIcon &icon, const QString &path, const QDBusInterface &interface)
	{
		QString string = interface.property("DevicePresentationIconName").toString();
		::Desktop::Drive::MediaTypeSet typeSet = ::Desktop::Drive::stringListToMediaTypeSet(interface.property("DriveMediaCompatibility").toStringList());

		if (!string.isEmpty())
			icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8());

		if (!::Desktop::Drive::MediaTypeSet(typeSet).intersect(::Desktop::FlashDrive::mediaTypeSet()).isEmpty())
		{
			::Desktop::Drive::MediaType type = ::Desktop::Drive::stringToMediaType(interface.property("DriveMedia").toString());

			if (icon.isNull() && (icon = ::Desktop::Drive::mediaTypeIcon(type)).isNull())
				icon = mediaTypeIconByConnectionInterface(interface);

			return new ::Desktop::FlashDrive(path,
											 icon,
											 driveLabel(interface),
											 interface.property("DevicePresentationHide").toBool(),
											 NULL,
											 interface.property("DeviceSize").toULongLong(),
											 typeSet,
											 type,
											 interface.property("DriveCanDetach").toBool());
		}
		else if (!::Desktop::Drive::MediaTypeSet(typeSet).intersect(::Desktop::OpticalDrive::mediaTypeSet()).isEmpty())
		{
			::Tools::Memory::ScopedPointer< ::Desktop::OpticalDrive > drive;

			if (icon.isNull())
				icon = ::Desktop::Theme::current()->driveOptical();

			drive.reset(new ::Desktop::OpticalDrive(path,
					   	   	   	   	   	   	   	    icon,
					   	   	   	   	   	   	   	    driveLabel(interface),
					   	   	   	   	   	   	   	    interface.property("DevicePresentationHide").toBool(),
					   	   	   	   	   	   	   	    NULL,
					   	   	   	   	   	   	   	    interface.property("DeviceSize").toULongLong(),
					   	   	   	   	   	   	   	    typeSet,
					   	   	   	   	   	   	   	    ::Desktop::Drive::stringToMediaType(interface.property("DriveMedia").toString()),
					   	   	   	   	   	   	   	    interface.property("DriveCanDetach").toBool(),
					   	   	   	   	   	   	   	    interface.property("DriveIsMediaEjectable").toBool()));

			if (interface.property("DeviceIsMediaAvailable").toBool())
				opticalDrivePartition(drive.data(), path, interface);

			return drive.take();
		}
		else if (!::Desktop::Drive::MediaTypeSet(typeSet).intersect(::Desktop::FloppyDrive::mediaTypeSet()).isEmpty())
		{
			::Desktop::Drive::MediaType type = ::Desktop::Drive::stringToMediaType(interface.property("DriveMedia").toString());

			if (icon.isNull() && (icon = ::Desktop::Drive::mediaTypeIcon(type)).isNull())
				icon = mediaTypeIconByConnectionInterface(interface);

			return new ::Desktop::FloppyDrive(path,
											  icon,
											  driveLabel(interface),
											  interface.property("DevicePresentationHide").toBool(),
 											  NULL,
											  interface.property("DeviceSize").toULongLong(),
											  typeSet,
											  type,
											  interface.property("DriveCanDetach").toBool());
		}

		return NULL;
	}

	::Desktop::Drive *hardDrive(const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;

		if (::Desktop::Drive *res = drive(icon, path, interface))
			return res;
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

	void hardDriveChanged(::Desktop::HardDrive *device, const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;
		QString string = interface.property("DevicePresentationIconName").toString();

		if (string.isEmpty() ||
			(icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8())).isNull())
		{
			icon = ::Desktop::Theme::current()->driveHarddisk();
		}

		device->setIcon(icon);
		device->setLabel(driveLabel(interface));
		device->setHidden(interface.property("DevicePresentationHide").toBool());
		device->setSize(interface.property("DeviceSize").toULongLong());

		parent->slotDeviceChanged(device);
	}

	::Desktop::Drive *removableDrive(const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;

		if (::Desktop::Drive *res = drive(icon, path, interface))
			return res;
		else
		{
			if (icon.isNull())
				icon = mediaTypeIconByConnectionInterface(interface);

			return new ::Desktop::RemovableDrive(path,
											   	 icon,
											   	 driveLabel(interface),
											   	 interface.property("DevicePresentationHide").toBool(),
												 NULL,
											   	 interface.property("DeviceSize").toULongLong(),
											   	 ::Desktop::Drive::MediaTypeSet(),
											   	 ::Desktop::Drive::NoMedia,
											   	 interface.property("DriveCanDetach").toBool());
		}
	}

	void removableDriveChanged(::Desktop::RemovableDrive *device, const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;
		QString string = interface.property("DevicePresentationIconName").toString();

		if (string.isEmpty() ||
			(icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8())).isNull())
		{
			icon = device->icon();
		}

		device->setIcon(icon);
		device->setLabel(driveLabel(interface));
		device->setHidden(interface.property("DevicePresentationHide").toBool());
		device->setSize(interface.property("DeviceSize").toULongLong());
		device->setMedia(::Desktop::Drive::stringToMediaType(interface.property("DriveMedia").toString()));
		device->setDetachable(interface.property("DriveCanDetach").toBool());
		device->as< ::Desktop::OpticalDrive >()->setEjectable(interface.property("DriveIsMediaEjectable").toBool());

		if (device->isOpticalDrive())
			if (interface.property("DeviceIsMediaAvailable").toBool())
				if (device->partitions().isEmpty())
					opticalDrivePartition(device->as< ::Desktop::OpticalDrive >(), path, interface);
				else
				{
					::Desktop::Partition *partition = *device->partitions().constBegin();

					QStringList mountPaths(interface.property("DeviceMountPaths").toStringList());
					string = interface.property("DevicePresentationIconName").toString();

					if (string.isEmpty() ||
						(icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8())).isNull())
					{
						icon = ::Desktop::Drive::mediaTypeIcon(device->media());
					}

					partition->setIcon(icon);
					partition->setLabel(partitionLabel(interface, mountPaths));
					partition->setHidden(interface.property("DevicePresentationHide").toBool());
					partition->setNumber(interface.property("PartitionNumber").toInt());
					partition->setSize(interface.property("PartitionSize").toULongLong());
					partition->setMountPaths(mountPaths);

					parent->slotDeviceChanged(partition);
				}
			else
				if (!device->partitions().isEmpty())
				{
					::Tools::Memory::ScopedPointer< ::Desktop::Partition > partition(*device->partitions().constBegin());
					device->removePartition(partition->id());
					parent->slotDeviceRemoved(partition);
				}

		parent->slotDeviceChanged(device);
	}

public:
	::Desktop::Device *processDevice(const QString &path, const QDBusInterface &interface)
	{
		if (!m_topLevelDevices.contains(path))
			if (interface.property("DeviceIsPartition").toBool())
				partition(path, interface);
			else if (interface.property("DeviceIsRemovable").toBool())
				return removableDrive(path, interface);
			else if (interface.property("DeviceIsDrive").toBool())
				return hardDrive(path, interface);

		return NULL;
	}

	void opticalDrivePartition(::Desktop::OpticalDrive *parent, const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;
		QString string;
		::Tools::Memory::ScopedPointer< ::Desktop::PartitionUnix > partition;

		QStringList mountPaths(interface.property("DeviceMountPaths").toStringList());
		string = interface.property("DevicePresentationIconName").toString();

		if (string.isEmpty() ||
			(icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8())).isNull())
		{
			icon = ::Desktop::Drive::mediaTypeIcon(parent->media());
		}

		partition.reset(new ::Desktop::PartitionUnix(path,
												 icon,
												 partitionLabel(interface, mountPaths),
												 interface.property("DevicePresentationHide").toBool(),
												 static_cast< ::Desktop::Drive *>(parent),
												 interface.property("PartitionNumber").toInt(),
												 interface.property("PartitionSize").toULongLong(),
												 mountPaths));

		parent->addPartition(partition.data());
		this->parent->slotDeviceAdded(partition.take());
	}

	void partition(const QString &path, const QDBusInterface &interface)
	{
		QIcon icon;
		QString string;
		::Tools::Memory::ScopedPointer< ::Desktop::PartitionUnix > partition;
		::Desktop::Device *parent = m_topLevelDevices.value(string = interface.property("PartitionSlave").value<QDBusObjectPath>().path());

		if (parent == NULL)
		{
			QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), string, QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

			if (interface.isValid() && (parent = processDevice(string, interface)))
				this->parent->slotDeviceAdded(m_allDevices[string] = m_topLevelDevices[string] = parent);
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

			partition.reset(new ::Desktop::PartitionUnix(path,
													 icon,
													 partitionLabel(interface, mountPaths),
													 interface.property("DevicePresentationHide").toBool(),
													 static_cast< ::Desktop::Drive *>(parent),
													 interface.property("PartitionNumber").toInt(),
													 interface.property("PartitionSize").toULongLong(),
													 mountPaths));

			m_allDevices[path] = partition.data();
			parent->as< ::Desktop::Drive >()->addPartition(partition.data());
			this->parent->slotDeviceAdded(partition.take());
		}
	}

	void refresh()
	{
		typedef QList<QDBusObjectPath> List;
		typedef QDBusReply<List>       Reply;

		QString string;
		::Tools::Memory::ScopedPointer< ::Desktop::Device > device;
		Reply reply = manager.call(QString::fromLatin1("EnumerateDevices"));

		if (reply.isValid())
		{
			List list(reply.value());

			for (List::size_type i = 0, size = list.size(); i < size; ++i)
			{
				QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), string = list.at(i).path(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

				if (interface.isValid() && (device = processDevice(string, interface)))
				{
					m_allDevices[string] = m_topLevelDevices[string] = device.data();
					parent->slotDeviceAdded(device.take());
				}
			}
		}
	}

public Q_SLOTS:
	void slotDeviceAdded(const QDBusObjectPath &path)
	{
		::Tools::Memory::ScopedPointer< ::Desktop::Device > device;
		QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), path.path(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

		if (interface.isValid() && (device = processDevice(path.path(), interface)))
		{
			m_allDevices[path.path()] = m_topLevelDevices[path.path()] = device.data();
			parent->slotDeviceAdded(device.take());
		}
	}

	void slotDeviceRemoved(const QDBusObjectPath &path)
	{
		if (::Desktop::Device *device = m_allDevices.take(path.path()))
		{
			if (device->isPartition())
			{
				::Tools::Memory::ScopedPointer< ::Desktop::Partition > partition(device->parent()->as< ::Desktop::Drive >()->takePartition(device->id()));
				parent->slotDeviceRemoved(partition);
			}
			else
			{
				::Tools::Memory::ScopedPointer< ::Desktop::Device > dev(m_topLevelDevices.take(device->id()));
				parent->slotDeviceRemoved(dev);
			}
		}
	}

	void slotDeviceChanged(const QDBusObjectPath &path)
	{
		QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), path.path(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

		if (interface.isValid())
			if (::Desktop::Device *device = m_allDevices.value(path.path()))
				if (device->isPartition())
				{
					QIcon icon;
					QStringList mountPaths(interface.property("DeviceMountPaths").toStringList());
					QString string = interface.property("DevicePresentationIconName").toString();
					::Desktop::Partition *partition = device->as< ::Desktop::Partition >();

					if (string.isEmpty() ||
						(icon = ::Desktop::Theme::current()->driveIcon(string.toUtf8())).isNull())
					{
						icon = device->icon();
					}

					partition->setIcon(icon);
					partition->setLabel(partitionLabel(interface, mountPaths));
					partition->setHidden(interface.property("DevicePresentationHide").toBool());
					partition->setNumber(interface.property("PartitionNumber").toInt());
					partition->setSize(interface.property("PartitionSize").toULongLong());
					partition->setMountPaths(mountPaths);

					parent->slotDeviceChanged(partition);
				}
				else
				{
					if (device->isRemovableDrive())
						removableDriveChanged(device->as< ::Desktop::RemovableDrive >(), path.path(), interface);
					else if (device->isDrive())
						hardDriveChanged(device->as< ::Desktop::HardDrive >(), path.path(), interface);
				}
			else
			{
				::Tools::Memory::ScopedPointer< ::Desktop::Device > deviceHolder;

				if (deviceHolder = processDevice(path.path(), interface))
				{
					m_allDevices[path.path()] = m_topLevelDevices[path.path()] = deviceHolder.data();
					parent->slotDeviceAdded(deviceHolder.take());
				}
			}
	}

public:
	::Desktop::Devices::Container m_topLevelDevices;
	::Desktop::Devices::Container m_allDevices;
	::Desktop::Devices *parent;
	QDBusInterface manager;
};

#endif /* DESKTOP_DEVICES_P_H_ */
