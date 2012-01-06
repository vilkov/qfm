#include "../desktopenvironment.h"
#include "../../filesystem/interfaces/filesystemifileinfo.h"
#include "xdgmime/src/xdgmime.h"

#include <QtCore/QDebug>

#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
#	include "kde/kde_de_p.h"
#elif defined(DESKTOP_ENVIRONMENT_IS_GTK)
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

static uint userId = getuid();
static uint groupId = getgid();


static int translatePermissions(const struct stat &st)
{
	int res = 0;

	if ((st.st_mode & S_IROTH) ||
		(st.st_uid == userId && (st.st_mode & S_IRUSR)) ||
		(st.st_gid == groupId && (st.st_mode & S_IRGRP)))
		res |= FileSystem::IFileInfo::Read;

	if ((st.st_mode & S_IWOTH) ||
		(st.st_uid == userId  && (st.st_mode & S_IWUSR)) ||
		(st.st_gid == groupId && (st.st_mode & S_IWGRP)))
		res |= FileSystem::IFileInfo::Write;

	if ((st.st_mode & S_IXOTH) ||
		(st.st_uid == userId  && (st.st_mode & S_IXUSR)) ||
		(st.st_gid == groupId && (st.st_mode & S_IXGRP)))
		res |= FileSystem::IFileInfo::Exec;

	return res;
}

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

#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
static void initTypeAndIcon(const QByteArray &fileName, FileSystem::FileInfo &info, struct stat &st, int size, int version)
{
	QByteArray iconThemeName = DesktopEnvironmentPrivate::iconThemeName(version).toUtf8();
#else
static void initTypeAndIcon(const QByteArray &fileName, FileSystem::FileInfo &info, struct stat &st, int size)
{
	QByteArray iconThemeName = DesktopEnvironmentPrivate::iconThemeName().toUtf8();
#endif

	if (info.isDir)
	{
		if (char *icon_path = xdg_mime_icon_lookup("folder", size, Places, iconThemeName.constData()))
		{
			info.icon.addFile(QString::fromUtf8(icon_path), QSize(size, size));
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
			if (char *icon_path = xdg_mime_type_icon_lookup(XDG_MIME_TYPE_TEXTPLAIN, size, iconThemeName.constData()))
			{
				info.icon.addFile(QString::fromUtf8(icon_path), QSize(size, size));
				info.type = QString::fromUtf8(XDG_MIME_TYPE_TEXTPLAIN);
				free(icon_path);
			}
		}
		else
		{
			if (char *icon_path = loadIcon(mimeType, size, iconThemeName.constData()))
			{
				info.icon.addFile(QString::fromUtf8(icon_path), QSize(size, size));
				free(icon_path);
			}
			else
				if (icon_path = xdg_mime_type_icon_lookup(XDG_MIME_TYPE_TEXTPLAIN, size, iconThemeName.constData()))
				{
					info.icon.addFile(QString::fromUtf8(icon_path), QSize(size, size));
					free(icon_path);
				}

			info.type = QString::fromUtf8(mimeType);
		}
	}
}


DesktopEnvironment::DesktopEnvironment() :
	m_type(DE_Unknown)
#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
	,m_version(0)
#endif
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
}

FileSystem::FileInfo DesktopEnvironment::info(const QString &absoluteFilePath) const
{
	int res;
	struct stat st;
	FileSystem::FileInfo info;
	QByteArray name = absoluteFilePath.toUtf8();

	if ((res = lstat(name.constData(), &st)) == 0)
		if ((info.isFile = S_ISREG(st.st_mode)) || (info.isDir = S_ISDIR(st.st_mode)))
		{
			info.permissions = translatePermissions(st);
			info.size = st.st_size;
			info.lastModified = QDateTime::fromTime_t(st.st_mtime);
#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
			initTypeAndIcon(name, info, st, 16, m_version);
#else
			initTypeAndIcon(name, info, st, 16);
#endif
		}
		else if (info.isLink = S_ISLNK(st.st_mode))
		{
			char buff[PATH_MAX] = {};

			if ((res = readlink(name.constData(), buff, PATH_MAX)) == 0)
				if (char *realName = canonicalize_file_name(buff))
				{
					if ((res = stat(realName, &st)) == 0)
						if ((info.isFile = S_ISREG(st.st_mode)) || (info.isDir = S_ISDIR(st.st_mode)))
						{
							info.permissions = translatePermissions(st);
							info.size = st.st_size;
							info.lastModified = QDateTime::fromTime_t(st.st_mtime);
#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
							initTypeAndIcon(name, info, st, 16, m_version);
#else
							initTypeAndIcon(name, info, st, 16);
#endif
						}

					free(realName);
				}
		}

	return info;
}
