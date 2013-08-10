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
