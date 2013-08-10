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
#include "../desktop_device_removabledrive.h"
#include "../../desktop_device_partition.h"
#include "../../unix/desktop_devices_udisks.h"


DESKTOP_NS_BEGIN

bool RemovableDrive::detach(QString &error)
{
	QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), id(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

	if (interface.isValid())
	{
		const Container &p = partitions();

		for (Container::const_iterator i = p.constBegin(), end = p.constEnd(); i != end; ++i)
			if (!(*i)->mountPaths().isEmpty())
				if (!(*i)->unmount(error))
					return false;

		QDBusMessage message = QDBusMessage::createMethodCall(QString::fromLatin1(UD_DBUS_SERVICE),
															  id(),
															  QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE),
															  QString::fromLatin1("DriveDetach"));

		message.setArguments(QList<QVariant>() << QStringList());

		QDBusReply<void> reply = QDBusConnection::systemBus().call(message);

		if (reply.isValid())
			return true;
		else
			error = reply.error().message();
	}
	else
		error = interface.lastError().message();

	return false;
}

bool RemovableDrive::eject(QString &error)
{
	QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), id(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

	if (interface.isValid())
	{
		const Container &p = partitions();

		for (Container::const_iterator i = p.constBegin(), end = p.constEnd(); i != end; ++i)
			if (!(*i)->mountPaths().isEmpty())
				if (!(*i)->unmount(error))
					return false;

		QDBusMessage message = QDBusMessage::createMethodCall(QString::fromLatin1(UD_DBUS_SERVICE),
															  id(),
															  QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE),
															  QString::fromLatin1("DriveEject"));

		message.setArguments(QList<QVariant>() << QStringList());

		QDBusReply<void> reply = QDBusConnection::systemBus().call(message);

		if (reply.isValid())
			return true;
		else
			error = reply.error().message();
	}
	else
		error = interface.lastError().message();

	return false;
}

DESKTOP_NS_END
