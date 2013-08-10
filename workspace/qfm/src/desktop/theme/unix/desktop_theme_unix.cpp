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
#include "../desktop_theme.h"
#include "desktop_iconcache.h"

#include <tools/platform/platform.h>

#if PLATFORM_DE(KDE)
#	include "kde/desktop_kde_p.h"
#elif PLATFORM_DE(GNOME) || PLATFORM_DE(XFCE4)
#	include "gtk/desktop_gtk_p.h"
#endif

#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <X11/Xlib.h>
#include <X11/Xatom.h>


DESKTOP_NS_BEGIN

enum X11Atom
{
    DTWM_IS_RUNNING,
    _DT_SAVE_MODE,
    _SGI_DESKS_MANAGER,
    NAtoms
};

static const char *x11_atomnames[NAtoms] =
{
	"DTWM_IS_RUNNING",
	"_DT_SAVE_MODE",
	"_SGI_DESKS_MANAGER"
};


#if PLATFORM_DE(KDE)
	static int kde_version = 0;
#endif


static IconCache iconCache;
static Theme *theme;


Theme::Theme() :
	m_type(DE_Unknown)
{
	Q_ASSERT(theme == NULL);

	if (Display *display = ::XOpenDisplay(NULL))
    {
        int rc;
        Atom type;
        int format;
        const char *var;
        unsigned long length, after;
        unsigned char *data = 0;
        Atom atoms[NAtoms];

#if defined(XlibSpecificationRelease) && (XlibSpecificationRelease >= 6)
        ::XInternAtoms(display, const_cast<char **>(x11_atomnames), NAtoms, False, atoms);
#else
		for (int i = 0; i = 0; i < NAtoms; ++i)
			atoms[i] = ::XInternAtom(display, const_cast<char *>(x11_atomnames[i]), False);
#endif

		do
		{
			if (var = ::getenv("KDE_FULL_SESSION"))
			{
				m_type = DE_Kde;

#if PLATFORM_DE(KDE)
				if (var = ::getenv("KDE_SESSION_VERSION"))
					kde_version = atoi(var);
#endif
				break;
			}

			if (var = ::getenv("DESKTOP_SESSION"))
				if (!::strcmp(var, "gnome"))
				{
					m_type = DE_Gnome;
					break;
				}

			// GNOME_DESKTOP_SESSION_ID is deprecated, but still check it
			if (var = ::getenv("GNOME_DESKTOP_SESSION_ID"))
			{
				m_type = DE_Gnome;
				break;
			}

			rc = ::XGetWindowProperty(display, RootWindow(display, DefaultScreen(display)),
									atoms[_DT_SAVE_MODE],
									0, 2, False, XA_STRING, &type, &format, &length,
									&after, &data);
			if (rc == Success && length)
			{
				if (!::strcmp(reinterpret_cast<const char *>(data), "xfce4"))
				{
					m_type = DE_Gnome;
					break;
				}

				::XFree(data);
				data = 0;
			}

			rc = ::XGetWindowProperty(display, RootWindow(display, DefaultScreen(display)),
									atoms[DTWM_IS_RUNNING],
									0, 1, False, AnyPropertyType, &type, &format, &length,
									&after, &data);
			if (rc == Success && length)
			{
				m_type = DE_Cde;
				break;
			}

			rc = ::XGetWindowProperty(display, RootWindow(display, DefaultScreen(display)),
									atoms[_SGI_DESKS_MANAGER], 0, 1, False, XA_WINDOW,
									&type, &format, &length, &after, &data);
			if (rc == Success && length)
			{
				m_type = DE_4Dwm;
				break;
			}
		}
		while (true);

		if (data)
			::XFree(reinterpret_cast<char *>(data));

		::XCloseDisplay(display);
    }

#if PLATFORM_DE(KDE)
	m_name = DesktopPrivate::iconThemeName(kde_version).toUtf8();
#else
	m_name = DesktopPrivate::iconThemeName().toUtf8();
#endif

	::xdg_init();

	theme = this;
}

Theme::~Theme()
{
	::xdg_shutdown();
}

Theme *Theme::current()
{
	Q_ASSERT(theme);
	return theme;
}

QIcon Theme::processingIcon(int iconSize) const
{
	return iconCache.findIcon("view-refresh", iconSize, XdgThemeActions, m_name);
}

QIcon Theme::cancelingIcon(int iconSize) const
{
	return iconCache.findIcon("application-exit", iconSize, XdgThemeActions, m_name);
}

QIcon Theme::copyActionIcon(int iconSize) const
{
	return iconCache.findIcon("edit-copy", iconSize, XdgThemeActions, m_name);
}

QIcon Theme::cutActionIcon(int iconSize) const
{
	return iconCache.findIcon("edit-cut", iconSize, XdgThemeActions, m_name);
}

QIcon Theme::pasteActionIcon(int iconSize) const
{
	return iconCache.findIcon("edit-paste", iconSize, XdgThemeActions, m_name);
}

QIcon Theme::propertiesActionIcon(int iconSize) const
{
	return iconCache.findIcon("document-properties", iconSize, XdgThemeActions, m_name);
}

QIcon Theme::packActionIcon(int iconSize) const
{
	return iconCache.findIcon("application-x-archive", iconSize, XdgThemeMimeTypes, m_name);
}

QIcon Theme::unpackActionIcon(int iconSize) const
{
	return iconCache.findIcon("archive-extract", iconSize, XdgThemeActions, m_name);
}

QIcon Theme::searchIcon(int iconSize) const
{
	return iconCache.findIcon("system-search", iconSize, XdgThemeActions, m_name);
}

QIcon Theme::openDataIcon(int iconSize) const
{
	return iconCache.findIcon("document-open-data", iconSize, XdgThemeActions, m_name);
}

QIcon Theme::missingIcon(int iconSize) const
{
	return iconCache.findIcon("image-missing", iconSize, XdgThemeStatus, m_name);
}

QIcon Theme::driveIcon(const char *iconName, int iconSize) const
{
	return iconCache.findIcon(iconName, iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::driveIcon(const QByteArray &iconName, int iconSize) const
{
	return iconCache.findIcon(iconName, iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::driveHarddisk(int iconSize) const
{
	return iconCache.findIcon("drive-harddisk", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::driveOptical(int iconSize) const
{
	return iconCache.findIcon("drive-optical", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::driveRemovableMedia(int iconSize) const
{
	return iconCache.findIcon("drive-removable-media", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::driveRemovableMediaUsb(int iconSize) const
{
	return iconCache.findIcon("drive-removable-media-usb", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::driveRemovableMediaUsbPendrive(int iconSize) const
{
	return iconCache.findIcon("drive-removable-media-usb-pendrive", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaFlashMemoryStick(int iconSize) const
{
	return iconCache.findIcon("media-flash-memory-stick", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaFlashSdMmc(int iconSize) const
{
	return iconCache.findIcon("media-flash-sd-mmc", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaFlashSmartMedia(int iconSize) const
{
	return iconCache.findIcon("media-flash-smart-media", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaFlash(int iconSize) const
{
	return iconCache.findIcon("media-flash", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaFloppy(int iconSize) const
{
	return iconCache.findIcon("media-floppy", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaOpticalAudio(int iconSize) const
{
	return iconCache.findIcon("media-optical-audio", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaOpticalBluRay(int iconSize) const
{
	return iconCache.findIcon("media-optical-blu-ray", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaOpticalData(int iconSize) const
{
	return iconCache.findIcon("media-optical-data", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaOpticalDvdVideo(int iconSize) const
{
	return iconCache.findIcon("media-optical-dvd-video", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaOpticalDvd(int iconSize) const
{
	return iconCache.findIcon("media-optical-dvd", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaOpticalMixedCd(int iconSize) const
{
	return iconCache.findIcon("media-optical-mixed-cd", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaOpticalRecordable(int iconSize) const
{
	return iconCache.findIcon("media-optical-recordable", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaOpticalVideo(int iconSize) const
{
	return iconCache.findIcon("media-optical-video", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaOptical(int iconSize) const
{
	return iconCache.findIcon("media-optical", iconSize, XdgThemeDevices, m_name);
}

QIcon Theme::mediaTape(int iconSize) const
{
	return iconCache.findIcon("media-tape", iconSize, XdgThemeDevices, m_name);
}

DESKTOP_NS_END
