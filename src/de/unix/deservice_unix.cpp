#include "../deservice.h"

#include <vfs/interfaces/vfs_ifileinfo.h>
#include <xdg/xdg.h>

#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
#	include "kde/kde_de_p.h"
#elif defined(DESKTOP_ENVIRONMENT_IS_GTK)
#	include "gtk/gtk_de_p.h"
#endif

#include <QtCore/QCache>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVarLengthArray>

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


inline static char *loadMimeTypeIcon(const char *mimeType, int size, const char *theme)
{
	if (char *icon_path = xdg_mime_type_icon_lookup(mimeType, size, theme))
		return icon_path;
	else
	{
		const XdgApp *app;
		const XdgJointList *apps;
		const XdgJointList *removed = xdg_removed_apps_lookup(mimeType);

		if (apps = xdg_joint_list_begin(xdg_added_apps_lookup(mimeType)))
			do
				if (!xdg_joint_list_contains_app(removed, app = xdg_joint_list_item_app(apps)))
					if (icon_path = xdg_app_icon_lookup(app, theme, size))
						return icon_path;
			while (apps = xdg_joint_list_next(apps));

		if (apps = xdg_joint_list_begin(xdg_default_apps_lookup(mimeType)))
			do
				if (!xdg_joint_list_contains_app(removed, app = xdg_joint_list_item_app(apps)))
					if (icon_path = xdg_app_icon_lookup(app, theme, size))
						return icon_path;
			while (apps = xdg_joint_list_next(apps));

		if (apps = xdg_joint_list_begin(xdg_known_apps_lookup(mimeType)))
			do
				if (!xdg_joint_list_contains_app(removed, app = xdg_joint_list_item_app(apps)))
					if (icon_path = xdg_app_icon_lookup(app, theme, size))
						return icon_path;
			while (apps = xdg_joint_list_next(apps));
	}

	return 0;
}


class IconCache
{
public:
	IconCache(const QByteArray &themeName) :
		m_themeName(themeName)
	{}

	void setThemeName(const QByteArray &themeName) { m_themeName = themeName; }

	QIcon findIcon(const char *name, int iconSize, int context)
	{
		IconIndex index(QString::fromLatin1(name), iconSize, context);

		if (QIcon *icon = lockedRead(index))
			return *icon;
		else
		{
			QWriteLocker lock(&m_cacheLock);

			if (icon = read(index))
				return *icon;
			else
			{
				QIcon res;

				if (char *icon_path = xdg_icon_lookup(name, iconSize, static_cast<Context>(context), m_themeName))
				{
					res = *write(index, QString::fromUtf8(icon_path));
					free(icon_path);
				}

				return res;
			}
		}
	}

	QIcon findMimeTypeIcon(const char *mimeType, int iconSize)
	{
		IconIndex index(QString::fromLatin1(mimeType), iconSize, XdgThemeMimeTypes);

		if (QIcon *icon = lockedRead(index))
			return *icon;
		else
		{
			QWriteLocker lock(&m_cacheLock);

			if (icon = read(index))
				return *icon;
			else
			{
				QIcon res;

				if (char *icon_path = loadMimeTypeIcon(mimeType, iconSize, m_themeName))
				{
					res = *write(index, QString::fromUtf8(icon_path));
					free(icon_path);
				}
				else
					if (icon_path = xdg_mime_type_icon_lookup(XDG_MIME_TYPE_TEXTPLAIN, iconSize, m_themeName))
					{
						res = *write(index, QString::fromUtf8(icon_path));
						free(icon_path);
					}

				return res;
			}
		}
	}

private:
	QIcon *read(const IconIndex &index)
	{
		return m_cache.object(index);
	}

	QIcon *lockedRead(const IconIndex &index)
	{
		QReadLocker lock(&m_cacheLock);
		return read(index);
	}

	QIcon *write(const IconIndex &index, const QString &fileName)
	{
		QScopedPointer<QIcon> icon(new QIcon());

		icon->addFile(fileName, QSize(index.size, index.size));
		m_cache.insert(index, icon.data());

		return icon.take();
	}

private:
	QByteArray m_themeName;
	QReadWriteLock m_cacheLock;
	QCache<IconIndex, QIcon> m_cache;
};
static IconCache *iconCache = 0;


inline static bool findProgram(const char *mimeType, const char *(&args)[3], const char *lang, const char *country, const char *modifier)
{
	const XdgApp *app;
	const XdgList *values;
	const XdgAppGroup *group;
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


DE_NS_BEGIN

Service::Service() :
	m_type(DE_Unknown),
	m_locale(QString::fromUtf8(getenv("LANG")))
{
	LocaleCodes::initialize(m_locale.name().toUtf8());

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

	iconCache = new IconCache(themeName());

	xdg_init();
}

Service::~Service()
{
	xdg_shutdown();
	delete iconCache;
}

QIcon Service::processingIcon(int iconSize) const
{
	return iconCache->findIcon("view-refresh", iconSize, XdgThemeActions);
}

QIcon Service::cancelingIcon(int iconSize) const
{
	return iconCache->findIcon("application-exit", iconSize, XdgThemeActions);
}

QIcon Service::copyActionIcon(int iconSize) const
{
	return iconCache->findIcon("edit-copy", iconSize, XdgThemeActions);
}

QIcon Service::cutActionIcon(int iconSize) const
{
	return iconCache->findIcon("edit-cut", iconSize, XdgThemeActions);
}

QIcon Service::pasteActionIcon(int iconSize) const
{
	return iconCache->findIcon("edit-paste", iconSize, XdgThemeActions);
}

QIcon Service::propertiesActionIcon(int iconSize) const
{
	return iconCache->findIcon("document-properties", iconSize, XdgThemeActions);
}

QIcon Service::packActionIcon(int iconSize) const
{
	return iconCache->findIcon("application-x-archive", iconSize, XdgThemeMimeTypes);
}

QIcon Service::unpackActionIcon(int iconSize) const
{
	return iconCache->findIcon("archive-extract", iconSize, XdgThemeActions);
}

QIcon Service::searchIcon(int iconSize) const
{
	return iconCache->findIcon("system-search", iconSize, XdgThemeActions);
}

QIcon Service::openDataIcon(int iconSize) const
{
	return iconCache->findIcon("document-open-data", iconSize, XdgThemeActions);
}

QIcon Service::missingIcon(int iconSize) const
{
	return iconCache->findIcon("image-missing", iconSize, XdgThemeStatus);
}

::VFS::FileTypeInfo Service::fileTypeInfo(const QByteArray &absoluteFilePath, bool isDir, int iconSize) const
{
	if (isDir)
		return fileTypeInfo(iconSize);
	else
	{
		const char *mimeType = xdg_mime_get_mime_type_from_file_name(absoluteFilePath);

		if (mimeType == XDG_MIME_TYPE_UNKNOWN)
			mimeType = xdg_mime_get_mime_type_for_file(absoluteFilePath, NULL);

		return fileTypeInfo(mimeType, iconSize);
	}
}

::VFS::FileTypeInfo Service::fileTypeInfoFromFileName(const QString &fileName, bool isDir, int iconSize) const
{
	if (isDir)
		return fileTypeInfo(iconSize);
	else
		return fileTypeInfo(xdg_mime_get_mime_type_from_file_name(fileName.toUtf8()), iconSize);
}

void Service::open(const ::VFS::IFileContainer *container, const ::VFS::IFileInfo *file) const
{
	typedef QList<QByteArray> List;
	const char *args[3] = {NULL, NULL, NULL};

	if (findProgram(file->fileType()->id().mime.toUtf8(), args, LocaleCodes::lang, LocaleCodes::country, NULL))
	{
		QByteArray absoluteFilePath = container->location(file);
		QByteArray workingDirectory = absoluteFilePath.mid(0, absoluteFilePath.lastIndexOf(QChar(L'/')));

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

		QByteArray fileName = absoluteFilePath.mid(absoluteFilePath.lastIndexOf(QChar(L'/')) + 1).
				replace('"', "\\\"").
				replace('`', "\\`").
				replace('$', "\\$").
				replace('\\', "\\\\");

		for (List::size_type i = 0, size = arguments.size(); i < size;)
			if (arguments.at(i).indexOf('=') != -1)
			{
				arguments.removeAt(i);
				--size;
			}
			else
			{
				arguments[i] = arguments.at(i).trimmed();

				if (arguments.at(i).indexOf("%i") != -1)
				{
					arguments[i].replace("%i", args[1]);
					arguments.insert(i, QByteArray("--icon"));
					++i;
					++size;
				}
				else
					arguments[i].
						replace("%f", fileName).
						replace("%F", fileName).
						replace("%u", fileName).
						replace("%U", fileName).
						replace("%c", args[2]);

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

QByteArray Service::themeName() const
{
#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
	return DesktopEnvironmentPrivate::iconThemeName(kde_version).toUtf8();
#else
	return DesktopEnvironmentPrivate::iconThemeName().toUtf8();
#endif
}

::VFS::FileTypeInfo Service::fileTypeInfo(int iconSize) const
{
	::VFS::FileTypeInfo info;

	info.icon = iconCache->findMimeTypeIcon("inode/directory", iconSize);
	info.id.mime = QString::fromLatin1("inode/directory");
	info.name = QString::fromLatin1("<DIR>");

	return info;
}

::VFS::FileTypeInfo Service::fileTypeInfo(const char *mimeType, int iconSize) const
{
	::VFS::FileTypeInfo info;

	if (strcmp(mimeType, XDG_MIME_TYPE_TEXTPLAIN) == 0 ||
		strcmp(mimeType, XDG_MIME_TYPE_UNKNOWN) == 0 ||
		strcmp(mimeType, XDG_MIME_TYPE_EMPTY) == 0)
	{
		info.icon = iconCache->findIcon("text-plain", iconSize, XdgThemeMimeTypes);
		info.name = info.id.mime = QString::fromUtf8(XDG_MIME_TYPE_TEXTPLAIN);
	}
	else
	{
		info.icon = iconCache->findMimeTypeIcon(mimeType, iconSize);
		info.name = info.id.mime = QString::fromUtf8(mimeType);
	}

	return info;
}

DE_NS_END
