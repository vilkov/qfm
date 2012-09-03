#ifndef DESKTOP_DEVICES_UDISKS_H_
#define DESKTOP_DEVICES_UDISKS_H_

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusConnectionInterface>

/* UDisks */
#define UD_DBUS_SERVICE                 "org.freedesktop.UDisks"
#define UD_DBUS_PATH                    "/org/freedesktop/UDisks"
#define UD_DBUS_INTERFACE_DISKS         "org.freedesktop.UDisks"
#define UD_DBUS_INTERFACE_DISKS_DEVICE  "org.freedesktop.UDisks.Device"
#define UD_UDI_DISKS_PREFIX             "/org/freedesktop/UDisks"

#endif /* DESKTOP_DEVICES_UDISKS_H_ */
