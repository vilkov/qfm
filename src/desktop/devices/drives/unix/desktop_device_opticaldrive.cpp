#include "../desktop_device_opticaldrive.h"
#include "../../unix/desktop_devices_udisks.h"


DESKTOP_NS_BEGIN

bool OpticalDrive::eject(QString &error)
{
	QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), id(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

	if (interface.isValid())
	{
		static const QString unmount = QString::fromLatin1("unmount");
		QDBusMessage message = QDBusMessage::createMethodCall(QString::fromLatin1(UD_DBUS_SERVICE),
															  id(),
															  QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE),
															  QString::fromLatin1("DriveEject"));

		message.setArguments(QList<QVariant>() << (QStringList() << unmount));

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
