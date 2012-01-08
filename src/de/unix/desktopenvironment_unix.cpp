#include "../desktopenvironment.h"
#include "../../filesystem/interfaces/filesystemifileinfo.h"
#include "xdgmime/src/xdgmime.h"

#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
#	include "kde/kde_de_p.h"
#elif defined(DESKTOP_ENVIRONMENT_IS_GTK)
#	include "gtk/gtk_de_p.h"
#endif

#include <QtCore/QCache>
#include <QtCore/QReadWriteLock>

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


class IconCache
{
public:
	IconCache()
	{}

	QIcon findIcon(const QString &name, const QSize &size)
	{
		if (QIcon *icon = iconFromCache(name))
			return *icon;
		else
			return *iconToCache(name, size);
	}

private:
	QIcon *iconFromCache(const QString &name)
	{
		QReadLocker lock(&m_cacheLock);
		return m_cache.object(name);
	}

	QIcon *iconToCache(const QString &name, const QSize &size)
	{
		QWriteLocker lock(&m_cacheLock);

		if (QIcon *icon = m_cache.object(name))
			return icon;
		else
		{
			QScopedPointer<QIcon> icon(new QIcon());

			icon->addFile(name, size);
			m_cache.insert(name, icon.data());

			return icon.take();
		}
	}

private:
	QReadWriteLock m_cacheLock;
	QCache<QString, QIcon> m_cache;
};
static IconCache *iconCache = 0;


static char *loadIcon(const char *mimeType, int size, const char *theme)
{
	if (char *icon_path = xdg_mime_type_icon_lookup(mimeType, size, theme))
		return icon_path;
	else
	{
		const XdgArray *apps;

		if (apps = xdg_mime_user_apps_lookup(mimeType))
			for (int i = 0, sz = xdg_mime_array_size(apps); i < sz; ++i)
				if (icon_path = xdg_mime_app_icon_lookup(xdg_mime_array_app_item_at(apps, i), theme, size))
					return icon_path;

		if (apps = xdg_mime_default_apps_lookup(mimeType))
			for (int i = 0, sz = xdg_mime_array_size(apps); i < sz; ++i)
				if (icon_path = xdg_mime_app_icon_lookup(xdg_mime_array_app_item_at(apps, i), theme, size))
					return icon_path;

		if (apps = xdg_mime_known_apps_lookup(mimeType))
			for (int i = 0, sz = xdg_mime_array_size(apps); i < sz; ++i)
				if (icon_path = xdg_mime_app_icon_lookup(xdg_mime_array_app_item_at(apps, i), theme, size))
					return icon_path;
	}

	return 0;
}


DesktopEnvironment::DesktopEnvironment() :
	m_type(DE_Unknown)
#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
	,m_version(0)
#endif
{
	iconCache = new IconCache();

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

#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
				if (var = getenv("KDE_SESSION_VERSION"))
					m_version = atoi(var);
#endif
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

    xdg_mime_init();
}

DesktopEnvironment::~DesktopEnvironment()
{
	xdg_mime_shutdown();
	delete iconCache;
}

FileTypeId DesktopEnvironment::fileTypeId(FileTypes id) const
{
	FileTypeId typeId;

	switch (id)
	{
		case M3uFile:
		{
			typeId.mime = QString::fromLatin1("");
			break;
		}
	}

	return typeId;
}

FileTypeInfo DesktopEnvironment::fileTypeInfo(const FileSystem::FileInfo &fileInfo, const QString &absoluteFilePath, int iconSize) const
{
	struct stat st;
	FileTypeInfo info;
	QByteArray fileName = absoluteFilePath.toUtf8();

#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
	QByteArray iconThemeName = DesktopEnvironmentPrivate::iconThemeName(m_version).toUtf8();
#else
	QByteArray iconThemeName = DesktopEnvironmentPrivate::iconThemeName().toUtf8();
#endif

	if (fileInfo.isDir)
	{
		if (char *icon_path = xdg_mime_icon_lookup("folder", iconSize, Places, iconThemeName.constData()))
		{
			info.icon = iconCache->findIcon(QString::fromUtf8(icon_path), QSize(iconSize, iconSize));
			info.name = info.id.mime = QString::fromLatin1("<DIR>");
			free(icon_path);
		}
	}
	else
	{
		const char *mimeType = xdg_mime_get_mime_type_from_file_name(fileName.data());

		if (mimeType == XDG_MIME_TYPE_UNKNOWN)
			mimeType = xdg_mime_get_mime_type_for_file(fileName.data(), &st);

		if (strcmp(mimeType, XDG_MIME_TYPE_TEXTPLAIN) == 0 ||
			strcmp(mimeType, XDG_MIME_TYPE_UNKNOWN) == 0 ||
			strcmp(mimeType, XDG_MIME_TYPE_EMPTY) == 0)
		{
			if (char *icon_path = xdg_mime_type_icon_lookup(XDG_MIME_TYPE_TEXTPLAIN, iconSize, iconThemeName.constData()))
			{
				info.icon = iconCache->findIcon(QString::fromUtf8(icon_path), QSize(iconSize, iconSize));
				info.name = info.id.mime = QString::fromUtf8(XDG_MIME_TYPE_TEXTPLAIN);
				free(icon_path);
			}
		}
		else
		{
			if (char *icon_path = loadIcon(mimeType, iconSize, iconThemeName.constData()))
			{
				info.icon = iconCache->findIcon(QString::fromUtf8(icon_path), QSize(iconSize, iconSize));
				free(icon_path);
			}
			else
				if (icon_path = xdg_mime_type_icon_lookup(XDG_MIME_TYPE_TEXTPLAIN, iconSize, iconThemeName.constData()))
				{
					info.icon = iconCache->findIcon(QString::fromUtf8(icon_path), QSize(iconSize, iconSize));
					free(icon_path);
				}

			info.name = info.id.mime = QString::fromUtf8(mimeType);
		}
	}

	return info;
}
