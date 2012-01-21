#ifndef MOUNTPOINTS_P_H_
#define MOUNTPOINTS_P_H_

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusConnectionInterface>
#include "../mountpoint.h"
#include "../../../filesystem/tools/filesystemcommontools.h"


/* UDisks */
#define UD_DBUS_SERVICE                 "org.freedesktop.UDisks"
#define UD_DBUS_PATH                    "/org/freedesktop/UDisks"
#define UD_DBUS_INTERFACE_DISKS         "org.freedesktop.UDisks"
#define UD_DBUS_INTERFACE_DISKS_DEVICE  "org.freedesktop.UDisks.Device"
#define UD_UDI_DISKS_PREFIX             "/org/freedesktop/UDisks"


class MountPoints::MountPointsPrivate
{
public:
	typedef MountPoint::value_type value_type;
	typedef MountPoint::size_type  size_type;

public:
	MountPointsPrivate();

	void refresh();

	QString lastError;
	value_type items;
	QDBusInterface manager;
};


MountPoints::MountPointsPrivate::MountPointsPrivate() :
	manager(UD_DBUS_SERVICE, UD_DBUS_PATH, UD_DBUS_INTERFACE_DISKS, QDBusConnection::systemBus())
{
//	qDBusRegisterMetaType<QList<QDBusObjectPath> >();
//  qDBusRegisterMetaType<QVariantMap>();

    bool serviceFound = manager.isValid();

    if (!serviceFound)
    {
        // find out whether it will be activated automatically
        QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.DBus",
                                                              "/org/freedesktop/DBus",
                                                              "org.freedesktop.DBus",
                                                              "ListActivatableNames");

        QDBusReply<QStringList> reply = QDBusConnection::systemBus().call(message);

        if (reply.isValid() && reply.value().contains(UD_DBUS_SERVICE))
        {
            QDBusConnection::systemBus().interface()->startService(UD_DBUS_SERVICE);
            serviceFound = true;
        }
    }

//    if (serviceFound)
//    {
//        connect(&manager, SIGNAL(DeviceAdded(QDBusObjectPath)),
//                this, SLOT(slotDeviceAdded(QDBusObjectPath)));
//        connect(&manager, SIGNAL(DeviceRemoved(QDBusObjectPath)),
//                this, SLOT(slotDeviceRemoved(QDBusObjectPath)));
//        connect(&manager, SIGNAL(DeviceChanged(QDBusObjectPath)),
//                this, SLOT(slotDeviceChanged(QDBusObjectPath)));
//    }
}

void MountPoints::MountPointsPrivate::refresh()
{
	QString value;
	QStringList list;
	QDBusReply<QList<QDBusObjectPath> > reply = manager.call("EnumerateDevices");

	if (reply.isValid())
		items.clear();
	else
	{
		lastError = reply.error().name();
		return;
	}

	foreach(const QDBusObjectPath &path, reply.value())
	{
		QDBusInterface device(UD_DBUS_SERVICE, path.path(), UD_DBUS_INTERFACE_DISKS_DEVICE, QDBusConnection::systemBus());

		if (device.isValid() && device.property("DeviceIsPartition").toBool())
		{
			list = device.property("DeviceMountPaths").value<QStringList>();

			if (list.isEmpty())
				items.push_back(MountPoint(QString(),
											(value = device.property("IdLabel").toString()).isEmpty() ? path.path() : value,
											path.path(),
											QPixmap(),
											0,
											0));
			else
				items.push_back(MountPoint(list.at(0),
											(value = device.property("IdLabel").toString()).isEmpty() ? path.path() : value,
											path.path(),
											QPixmap(),
											FileSystem::Tools::freeSpace(list.at(0).toUtf8()),
											device.property("PartitionSize").toULongLong()));


//			qDebug() << device->property("DeviceMountPaths");
//			qDebug() << device->property("PartitionLabel");
//			qDebug() << device->property("IdLabel");
//			qDebug() << device->property("PartitionSize");
//			qDebug() << device->property("DeviceIsMounted");



//                if (!prop.isValid())
//                {
//                    qWarning() << Q_FUNC_INFO << " error: " << reply.error().name();
//                }
//
//                foreach(const QDBusObjectPath &path, reply.value())
//                {
//                    qDebug() << path.path();
//                }
		}
	}
}

#endif /* MOUNTPOINTS_P_H_ */
