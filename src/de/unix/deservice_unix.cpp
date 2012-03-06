#include "../deservice.h"
#include "../../filesystem/interfaces/filesystemifileinfo.h"

#include <xdg/xdg.h>

#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
#	include "kde/kde_de_p.h"
#elif defined(DESKTOP_ENVIRONMENT_IS_GTK)
#	include "gtk/gtk_de_p.h"
#endif

#include <QtCore/QCache>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVarLengthArray>
#include <QtCore/QDebug>

#include <paths.h>
#include <sys/stat.h>
#include <unistd.h>
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


#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
	static int kde_version = 0;
#endif


namespace LocaleCodes
{
	static QByteArray lang;
	static QByteArray country;

	static void initialize(const QByteArray &localeNameAsUtf8)
	{
		int index;
		lang = localeNameAsUtf8.mid(0, index = localeNameAsUtf8.indexOf('_'));
		country = localeNameAsUtf8.mid(index + 1);
	}
}


struct IconIndex
{
	IconIndex()
	{}
	IconIndex(const QString &name, int size, int context) :
		name(name),
		size(size),
		context(context)
	{}

	bool operator==(const IconIndex &other) const
	{
		return context == other.context && size == other.size && name == other.name;
	}

	bool operator<(const IconIndex &other) const
	{
		return context < other.context || size < other.size || name < other.name;
	}

	QString name;
	int size;
	int context;
};

uint qHash(const IconIndex &index)
{
	return qHash(index.name) + index.size + index.context;
}


class IconCache
{
public:
	IconCache()
	{}

	QIcon *iconFromCache(const QString &name, int size, int context)
	{
		QReadLocker lock(&m_cacheLock);
		return m_cache.object(IconIndex(name, size, context));
	}

	QIcon *iconToCache(const QString &name, int size, int context, const QString &fileName)
	{
		IconIndex index(name, size, context);
		QWriteLocker lock(&m_cacheLock);

		if (QIcon *icon = m_cache.object(index))
			return icon;
		else
		{
			QScopedPointer<QIcon> icon(new QIcon());

			icon->addFile(fileName, QSize(size, size));
			m_cache.insert(index, icon.data());

			return icon.take();
		}
	}

private:
	QReadWriteLock m_cacheLock;
	QCache<IconIndex, QIcon> m_cache;
};
static IconCache *iconCache = 0;


inline static char *loadMimeTypeIcon(const char *mimeType, int size, const char *theme)
{
	if (char *icon_path = xdg_mime_type_icon_lookup(mimeType, size, theme))
		return icon_path;
	else
	{
		const XdgJointList *apps;

		if (apps = xdg_joint_list_begin(xdg_added_apps_lookup(mimeType)))
			do
				if (icon_path = xdg_app_icon_lookup(xdg_joint_list_item_app(apps), theme, size))
					return icon_path;
			while (apps = xdg_joint_list_next(apps));

		if (apps = xdg_joint_list_begin(xdg_default_apps_lookup(mimeType)))
			do
				if (icon_path = xdg_app_icon_lookup(xdg_joint_list_item_app(apps), theme, size))
					return icon_path;
			while (apps = xdg_joint_list_next(apps));

		if (apps = xdg_joint_list_begin(xdg_known_apps_lookup(mimeType)))
			do
				if (icon_path = xdg_app_icon_lookup(xdg_joint_list_item_app(apps), theme, size))
					return icon_path;
			while (apps = xdg_joint_list_next(apps));
	}

	return 0;
}

inline static bool findProgram(const char *mimeType, const char *(&args)[3], const char *lang, const char *country, const char *modifier)
{
	const XdgApp *app;
	const XdgList *values;
	const XdgAppGroupEntries *group;
	const XdgJointList *apps;
	const XdgJointList *removed = xdg_removed_apps_lookup(mimeType);

	if (apps = xdg_joint_list_begin(xdg_added_apps_lookup(mimeType)))
		do
			if (!xdg_joint_list_contains_app(removed, app = xdg_joint_list_item_app(apps)))
				if (group = xdg_app_group_lookup(app, "Desktop Entry"))
					if (values = xdg_list_begin(xdg_app_entry_lookup(group, "Exec")))
					{
						args[0] = xdg_list_item_app_group_entry_value(values);

						if (values = xdg_list_begin(xdg_app_entry_lookup(group, "Icon")))
							args[1] = xdg_list_item_app_group_entry_value(values);

						if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Name", lang, country, modifier)))
							args[2] = xdg_list_item_app_group_entry_value(values);

						return true;
					}
		while (apps = xdg_joint_list_next(apps));

	if (apps = xdg_joint_list_begin(xdg_default_apps_lookup(mimeType)))
		do
			if (!xdg_joint_list_contains_app(removed, app = xdg_joint_list_item_app(apps)))
				if (group = xdg_app_group_lookup(app, "Desktop Entry"))
					if (values = xdg_list_begin(xdg_app_entry_lookup(group, "Exec")))
					{
						args[0] = xdg_list_item_app_group_entry_value(values);

						if (values = xdg_list_begin(xdg_app_entry_lookup(group, "Icon")))
							args[1] = xdg_list_item_app_group_entry_value(values);

						if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Name", lang, country, modifier)))
							args[2] = xdg_list_item_app_group_entry_value(values);

						return true;
					}
		while (apps = xdg_joint_list_next(apps));

	if (apps = xdg_joint_list_begin(xdg_known_apps_lookup(mimeType)))
		do
			if (!xdg_joint_list_contains_app(removed, app = xdg_joint_list_item_app(apps)))
				if (group = xdg_app_group_lookup(app, "Desktop Entry"))
					if (values = xdg_list_begin(xdg_app_entry_lookup(group, "Exec")))
					{
						args[0] = xdg_list_item_app_group_entry_value(values);

						if (values = xdg_list_begin(xdg_app_entry_lookup(group, "Icon")))
							args[1] = xdg_list_item_app_group_entry_value(values);

						if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Name", lang, country, modifier)))
							args[2] = xdg_list_item_app_group_entry_value(values);

						return true;
					}
		while (apps = xdg_joint_list_next(apps));

	return false;
}

inline static const char *resolveProgram(const char *name)
{
//	realpath();
	if (const char *path = getenv("PATH"))
	{
		struct stat st;

		if (const char *sep = strchr(path, ':'))
			do
			{

			}
			while (sep = strchr(sep, ':'));
		else
		{

		}
	}

	return NULL;
}


DE_NS_BEGIN

Service::Service() :
	m_type(DE_Unknown),
	m_locale(QString::fromUtf8(getenv("LANG")))
{
	LocaleCodes::initialize(m_locale.name().toUtf8());
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
					kde_version = atoi(var);
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

    xdg_init();
}

Service::~Service()
{
	xdg_shutdown();
	delete iconCache;
}

QIcon Service::processingIcon(int iconSize) const
{
	return findIcon("view-refresh", iconSize, Actions);
}

QIcon Service::cancelingIcon(int iconSize) const
{
	return findIcon("application-exit", iconSize, Actions);
}

QIcon Service::copyActionIcon(int iconSize) const
{
	return findIcon("edit-copy", iconSize, Actions);
}

QIcon Service::cutActionIcon(int iconSize) const
{
	return findIcon("edit-cut", iconSize, Actions);
}

QIcon Service::pasteActionIcon(int iconSize) const
{
	return findIcon("edit-paste", iconSize, Actions);
}

QIcon Service::propertiesActionIcon(int iconSize) const
{
	return findIcon("document-properties", iconSize, Actions);
}

QIcon Service::packActionIcon(int iconSize) const
{
	return findIcon("application-x-archive", iconSize, MimeTypes);
}

QIcon Service::unpackActionIcon(int iconSize) const
{
	return findIcon("archive-extract", iconSize, Actions);
}

::FileSystem::FileTypeInfo Service::fileTypeInfo(const QString &absoluteFilePath, bool isDir, int iconSize) const
{
	if (isDir)
		return fileTypeInfo(iconSize);
	else
	{
		struct stat st;
		QByteArray fileName = absoluteFilePath.toUtf8();
		const char *mimeType = xdg_mime_get_mime_type_from_file_name(fileName);

		if (mimeType == XDG_MIME_TYPE_UNKNOWN)
			mimeType = xdg_mime_get_mime_type_for_file(fileName, &st);

		return fileTypeInfo(mimeType, iconSize);
	}
}

::FileSystem::FileTypeInfo Service::fileTypeInfoFromFileName(const QString &fileName, bool isDir, int iconSize) const
{
	if (isDir)
		return fileTypeInfo(iconSize);
	else
		return fileTypeInfo(xdg_mime_get_mime_type_from_file_name(fileName.toUtf8()), iconSize);
}

void Service::open(const ::FileSystem::IFileContainer *container, const ::FileSystem::IFileInfo *file) const
{
	typedef QList<QByteArray> List;
	const char *args[3] = {NULL, NULL, NULL};

	if (findProgram(file->id().mime.toUtf8(), args, LocaleCodes::lang, LocaleCodes::country, NULL))
	{
		QByteArray workingDirectory = container->location().toUtf8();

		List arguments = QByteArray(args[0]).
				replace("%d", QByteArray()).
				replace("%D", QByteArray()).
				replace("%n", QByteArray()).
				replace("%N", QByteArray()).
				replace("%k", QByteArray()).
				replace("%v", QByteArray()).
				replace("%m", QByteArray()).
				trimmed().
				split(' ');

		QByteArray fileName = file->fileName().toUtf8().
				replace('"', "\\\"").
				replace('`', "\\`").
				replace('$', "\\$").
				replace('\\', "\\\\");

		for (List::size_type i = 0, size = arguments.size(); i < size;)
			if (arguments.at(i).indexOf('=') != -1)
				arguments.removeAt(i);
			else
			{
				arguments[i] = arguments.at(i).trimmed();

				if (arguments.at(i).indexOf("%i") != -1)
				{
					arguments[i].replace("%i", args[1]);
					arguments.insert(i, QByteArray("--icon"));
					++i;
				}
				else
				{
					arguments[i].replace("%f", fileName);
					arguments[i].replace("%F", fileName);
					arguments[i].replace("%u", fileName);
					arguments[i].replace("%U", fileName);
					arguments[i].replace("%c", args[2]);
				}

				++i;
			}

		if (pid_t pid = fork())
		{
			if (pid < 0)
			{
				/* The fork failed. */
			}
		}
		else
		{
			QVarLengthArray<char *, 8> argv(arguments.size() + 1);

			for (List::size_type i = 0, size = arguments.size(); i < size; ++i)
				if (arguments.at(i).indexOf('=') == -1)
				{
					arguments[i] = arguments.at(i).trimmed();
					argv.data()[i] = arguments[i].data();
				}

			argv.data()[arguments.size()] = NULL;

			setsid();
			chdir(workingDirectory);
			execvp(argv.data()[0], argv.data());
			exit(EXIT_FAILURE);
		}
	}
}

void Service::test() const
{
	const XdgList *values;
	const XdgAppGroupEntries *group;
	const XdgJointList *apps;

	if (apps = xdg_joint_list_begin(xdg_default_apps_lookup("video/x-msvideo")))
		do
		{
			group = xdg_app_group_lookup(xdg_joint_list_item_app(apps), "Desktop Entry");

			if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "GenericName", "ru", "RU", NULL)))
				do
				{
					qDebug() << QString::fromUtf8(xdg_list_item_app_group_entry_value(values));
				}
				while (values = xdg_list_next(values));

			if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Name", "ru", "RU", NULL)))
				do
				{
					qDebug() << QString::fromUtf8(xdg_list_item_app_group_entry_value(values));
				}
				while (values = xdg_list_next(values));
		}
		while (apps = xdg_joint_list_next(apps));

	if (apps = xdg_joint_list_begin(xdg_removed_apps_lookup("video/x-msvideo")))
		do
		{
			group = xdg_app_group_lookup(xdg_joint_list_item_app(apps), "Desktop Entry");

			if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "GenericName", "ru", "RU", NULL)))
				do
				{
					qDebug() << QString::fromUtf8(xdg_list_item_app_group_entry_value(values));
				}
				while (values = xdg_list_next(values));

			if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Name", "ru", "RU", NULL)))
				do
				{
					qDebug() << QString::fromUtf8(xdg_list_item_app_group_entry_value(values));
				}
				while (values = xdg_list_next(values));
		}
		while (apps = xdg_joint_list_next(apps));

	if (apps = xdg_joint_list_begin(xdg_added_apps_lookup("video/x-msvideo")))
		do
		{
			group = xdg_app_group_lookup(xdg_joint_list_item_app(apps), "Desktop Entry");

			if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "GenericName", "ru", "RU", NULL)))
				do
				{
					qDebug() << QString::fromUtf8(xdg_list_item_app_group_entry_value(values));
				}
				while (values = xdg_list_next(values));

			if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Name", "ru", "RU", NULL)))
				do
				{
					qDebug() << QString::fromUtf8(xdg_list_item_app_group_entry_value(values));
				}
				while (values = xdg_list_next(values));
		}
		while (apps = xdg_joint_list_next(apps));

	if (apps = xdg_joint_list_begin(xdg_known_apps_lookup("video/x-msvideo")))
		do
		{
			group = xdg_app_group_lookup(xdg_joint_list_item_app(apps), "Desktop Entry");

			if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "GenericName", "ru", "RU", NULL)))
				do
				{
					qDebug() << QString::fromUtf8(xdg_list_item_app_group_entry_value(values));
				}
				while (values = xdg_list_next(values));

			if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Name", "ru", "RU", NULL)))
				do
				{
					qDebug() << QString::fromUtf8(xdg_list_item_app_group_entry_value(values));
				}
				while (values = xdg_list_next(values));
		}
		while (apps = xdg_joint_list_next(apps));
}

QByteArray Service::themeName() const
{
#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
	return DesktopEnvironmentPrivate::iconThemeName(kde_version).toUtf8();
#else
	return DesktopEnvironmentPrivate::iconThemeName().toUtf8();
#endif
}

::FileSystem::FileTypeInfo Service::fileTypeInfo(int iconSize) const
{
	::FileSystem::FileTypeInfo info;

	info.icon = findIcon("folder", iconSize, Places);
	info.name = (info.id = fileTypeId(FileTypes::Folder)).mime;

	return info;
}

::FileSystem::FileTypeInfo Service::fileTypeInfo(const char *mimeType, int iconSize) const
{
	::FileSystem::FileTypeInfo info;

	if (strcmp(mimeType, XDG_MIME_TYPE_TEXTPLAIN) == 0 ||
		strcmp(mimeType, XDG_MIME_TYPE_UNKNOWN) == 0 ||
		strcmp(mimeType, XDG_MIME_TYPE_EMPTY) == 0)
	{
		info.icon = findIcon("text-plain", iconSize, MimeTypes);
		info.name = info.id.mime = QString::fromUtf8(XDG_MIME_TYPE_TEXTPLAIN);
	}
	else
	{
		info.icon = findMimeTypeIcon(mimeType, iconSize);
		info.name = info.id.mime = QString::fromUtf8(mimeType);
	}

	return info;
}

QIcon Service::findIcon(const char *name, int iconSize, int context) const
{
	QString nameString = QString::fromLatin1(name);

	if (QIcon *icon = iconCache->iconFromCache(nameString, iconSize, context))
		return *icon;
	else
	{
		QIcon res;

		if (char *icon_path = xdg_icon_lookup(name, iconSize, static_cast<Context>(context), themeName()))
		{
			res = *iconCache->iconToCache(nameString, iconSize, context, QString::fromUtf8(icon_path));
			free(icon_path);
		}

		return res;
	}
}

QIcon Service::findMimeTypeIcon(const char *mimeType, int iconSize) const
{
	QString nameString = QString::fromLatin1(mimeType);

	if (QIcon *icon = iconCache->iconFromCache(nameString, iconSize, MimeTypes))
		return *icon;
	else
	{
		QIcon res;
		QByteArray iconThemeName = themeName();

		if (char *icon_path = loadMimeTypeIcon(mimeType, iconSize, iconThemeName))
		{
			res = *iconCache->iconToCache(nameString, iconSize, MimeTypes, QString::fromUtf8(icon_path));
			free(icon_path);
		}
		else
			if (icon_path = xdg_mime_type_icon_lookup(XDG_MIME_TYPE_TEXTPLAIN, iconSize, iconThemeName))
			{
				res = *iconCache->iconToCache(nameString, iconSize, MimeTypes, QString::fromUtf8(icon_path));
				free(icon_path);
			}

		return res;
	}
}

DE_NS_END
