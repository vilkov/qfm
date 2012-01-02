#include "../desktopenvironment.h"
#include "xdgmime/src/xdgmime.h"

#include <QtCore/QDebug>

#ifdef DESKTOP_ENVIRONMENT_IS_KDE
#	include "kde/kde_de_p.h"
#endif

#ifdef DESKTOP_ENVIRONMENT_IS_GTK
#	include "gtk/gtk_de_p.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <X11/Xlib.h>
#include <X11/Xatom.h>


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


DesktopEnvironment::DesktopEnvironment() :
	m_type(DE_Unknown),
	m_version(0)
{
    if (Display *display = XOpenDisplay(NULL))
    {
        int rc;
        Atom type;
        int format;
        const char *var;
        unsigned long length, after;
        unsigned char *data = 0;
        Atom atoms[NAtoms];

#if defined(XlibSpecificationRelease) && (XlibSpecificationRelease >= 6)
		XInternAtoms(display, const_cast<char **>(x11_atomnames), NAtoms, False, atoms);
#else
		for (int i = 0; i = 0; i < NAtoms; ++i)
			atoms[i] = XInternAtom(display, const_cast<char *>(x11_atomnames[i]), False);
#endif

		do
		{
			if (var = getenv("KDE_FULL_SESSION"))
			{
				m_type = DE_Kde;

				if (var = getenv("KDE_SESSION_VERSION"))
					m_version = atoi(var);

				break;
			}

			if (var = getenv("DESKTOP_SESSION"))
				if (!strcmp(var, "gnome"))
				{
					m_type = DE_Gnome;
					break;
				}

			// GNOME_DESKTOP_SESSION_ID is deprecated, but still check it
			if (var = getenv("GNOME_DESKTOP_SESSION_ID"))
			{
				m_type = DE_Gnome;
				break;
			}

			rc = XGetWindowProperty(display, RootWindow(display, DefaultScreen(display)),
									atoms[_DT_SAVE_MODE],
									0, 2, False, XA_STRING, &type, &format, &length,
									&after, &data);
			if (rc == Success && length)
			{
				if (!strcmp(reinterpret_cast<const char *>(data), "xfce4"))
				{
					m_type = DE_Gnome;
					break;
				}

				XFree(data);
				data = 0;
			}

			rc = XGetWindowProperty(display, RootWindow(display, DefaultScreen(display)),
									atoms[DTWM_IS_RUNNING],
									0, 1, False, AnyPropertyType, &type, &format, &length,
									&after, &data);
			if (rc == Success && length)
			{
				m_type = DE_Cde;
				break;
			}

			rc = XGetWindowProperty(display, RootWindow(display, DefaultScreen(display)),
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
			XFree(reinterpret_cast<char *>(data));

		XCloseDisplay(display);
    }
}

DesktopEnvironment::~DesktopEnvironment()
{
	xdg_mime_shutdown();
}

QString DesktopEnvironment::themeName() const
{
    QString result;
    return result;
}

bool DesktopEnvironment::info(const QString &absoluteFilePath) const
{
	QByteArray fileName = absoluteFilePath.toUtf8();
	const char *mimeType = xdg_mime_get_mime_type_from_file_name(fileName.data());

	if (mimeType == XDG_MIME_TYPE_UNKNOWN)
	{
		struct stat st;
		stat(fileName.data(), &st);

		if ((mimeType = xdg_mime_get_mime_type_for_file(fileName.data(), &st)) != XDG_MIME_TYPE_UNKNOWN)
		{
			if (const XdgAppArray *apps = xdg_mime_user_apps_lookup(mimeType))
			{
				const XdgAppGroup *group;
				const XdgEntryValueArray *values;

				for (int i = 0, size = xdg_mime_app_array_size(apps); i < size; ++i)
					if (group = xdg_mime_app_group_lookup(xdg_mime_app_array_item_at(apps, i), "Desktop Entry"))
						if (values = xdg_mime_app_entry_lookup(group, "Name"))
							for (int i = 0, size = xdg_mime_entry_value_array_size(values); i < size; ++i)
								qDebug() << xdg_mime_entry_value_array_item_at(values, i);
			}

			if (const XdgAppArray *apps = xdg_mime_default_apps_lookup(mimeType))
			{
				const XdgAppGroup *group;
				const XdgEntryValueArray *values;

				for (int i = 0, size = xdg_mime_app_array_size(apps); i < size; ++i)
					if (group = xdg_mime_app_group_lookup(xdg_mime_app_array_item_at(apps, i), "Desktop Entry"))
						if (values = xdg_mime_app_entry_lookup(group, "Name"))
							for (int i = 0, size = xdg_mime_entry_value_array_size(values); i < size; ++i)
								qDebug() << xdg_mime_entry_value_array_item_at(values, i);
			}

			if (const XdgAppArray *apps = xdg_mime_known_apps_lookup(mimeType))
			{
				const XdgAppGroup *group;
				const XdgEntryValueArray *values;

				for (int i = 0, size = xdg_mime_app_array_size(apps); i < size; ++i)
					if (group = xdg_mime_app_group_lookup(xdg_mime_app_array_item_at(apps, i), "Desktop Entry"))
						if (values = xdg_mime_app_entry_lookup(group, "Name"))
							for (int i = 0, size = xdg_mime_entry_value_array_size(values); i < size; ++i)
								qDebug() << xdg_mime_entry_value_array_item_at(values, i);
			}
		}
	}

	return false;
}
