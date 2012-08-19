#include "defaultmimeiconcache.h"
#include "../defaultfileinfo.h"
#include "../defaultfileaccessor.h"

#include <desktop/theme/desktop_theme.h>
#include <QtCore/QTextCodec>

#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>


DEFAULT_PLUGIN_NS_BEGIN

static uint userId = getuid();
static uint groupId = getgid();
static MimeIconCache iconCache;


static int translatePermissions(const struct stat &st)
{
	int res = 0;

	if ((st.st_mode & S_IROTH) ||
		(st.st_uid == userId && (st.st_mode & S_IRUSR)) ||
		(st.st_gid == groupId && (st.st_mode & S_IRGRP)))
		res |= IFileInfo::Read;

	if ((st.st_mode & S_IWOTH) ||
		(st.st_uid == userId  && (st.st_mode & S_IWUSR)) ||
		(st.st_gid == groupId && (st.st_mode & S_IWGRP)))
		res |= IFileInfo::Write;

	if ((st.st_mode & S_IXOTH) ||
		(st.st_uid == userId  && (st.st_mode & S_IXUSR)) ||
		(st.st_gid == groupId && (st.st_mode & S_IXGRP)))
		res |= IFileInfo::Exec;

	return res;
}


static QByteArray normalizeFilePath(const QByteArray &filePath, bool &isRoot)
{
	QByteArray res = filePath;

	if (res.startsWith('/'))
	{
		if (res.size() == 1)
		{
			isRoot = true;
			return res;
		}
	}
	else
	{
		char buff[PATH_MAX] = {};

		if (char *name = getcwd(buff, PATH_MAX))
			res.prepend(name);
	}

	if (res.endsWith('/'))
		res.chop(1);

	return res;
}


Info::Info() :
	m_isRoot(false)
{}

Info::Info(const Info &other) :
	m_isRoot(other.m_isRoot),
	m_filePath(other.m_filePath),
	m_fileName(other.m_fileName),
	m_info(other.m_info)
{}

template <>
Info::Info<Info::None>(const QByteArray &filePath, None) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(location(filePath.mid(filePath.lastIndexOf('/') + 1), codec()))
{}

template <>
Info::Info<Info::Refresh>(const QByteArray &filePath, Refresh) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(location(filePath.mid(filePath.lastIndexOf('/') + 1), codec()))
{
	refresh();
}

template <>
Info::Info<Info::Identify>(const QByteArray &filePath, Identify) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(location(filePath.mid(filePath.lastIndexOf('/') + 1), codec()))
{
	refresh();
	identify();
}

template <>
Info::Info<Info::None>(const Info &other, None) :
	m_isRoot(other.m_isRoot),
	m_filePath(other.m_filePath),
	m_fileName(other.m_fileName),
	m_info(other.m_info)
{}

template <>
Info::Info<Info::Refresh>(const Info &other, Refresh) :
	m_isRoot(other.m_isRoot),
	m_filePath(other.m_filePath),
	m_fileName(other.m_fileName),
	m_info(other.m_info)
{
	refresh();
}

template <>
Info::Info<Info::Identify>(const Info &other, Identify) :
	m_isRoot(other.m_isRoot),
	m_filePath(other.m_filePath),
	m_fileName(other.m_fileName),
	m_info(other.m_info)
{
	identify();
}

bool Info::isDir() const
{
	return m_info.isDir;
}

bool Info::isFile() const
{
	return m_info.isFile;
}

bool Info::isLink() const
{
	return m_info.isLink;
}

Info::size_type Info::fileSize() const
{
	return m_info.size;
}

const Location &Info::fileName() const
{
	return m_fileName;
}

const IFileType *Info::fileType() const
{
	return this;
}

const QDateTime &Info::lastModified() const
{
	return m_info.lastModified;
}

int Info::permissions() const
{
	return m_info.permissions;
}

const FileTypeId &Info::id() const
{
	return m_info.type.id;
}

const QIcon &Info::icon() const
{
	return m_info.type.icon;
}

const QString &Info::name() const
{
	return m_info.type.name;
}

const QString &Info::description() const
{
	return m_info.type.description;
}

QTextCodec *Info::codec()
{
	return QTextCodec::codecForLocale();
}

FileTypeInfo Info::fileTypeInfoFromFileName(const QByteArray &fileName, bool isDir)
{
	FileTypeInfo type;

	if (isDir)
	{
		type.icon = iconCache.findIcon("inode/directory", ::Desktop::Theme::Small, ::Desktop::Theme::current()->name());
		type.id.mime = QString::fromLatin1("inode/directory");
		type.name = QString::fromLatin1("<DIR>");
	}
	else
	{
		const char *mimeType = xdg_mime_get_mime_type_from_file_name(fileName);

		if (strcmp(mimeType, XDG_MIME_TYPE_TEXTPLAIN) == 0 ||
			strcmp(mimeType, XDG_MIME_TYPE_UNKNOWN) == 0 ||
			strcmp(mimeType, XDG_MIME_TYPE_EMPTY) == 0)
		{
			mimeType = XDG_MIME_TYPE_TEXTPLAIN;
		}

		type.icon = iconCache.findIcon(mimeType, ::Desktop::Theme::Small, ::Desktop::Theme::current()->name());
		type.name = type.id.mime = QString::fromUtf8(mimeType);
	}

	return type;
}

QIcon Info::fileTypeIcon(const char *mimeType, int size)
{
	return iconCache.findIcon(mimeType, size, ::Desktop::Theme::current()->name());
}

void Info::refresh()
{
	struct stat st;

	if (m_info.exists = (::lstat(m_filePath, &st) == 0))
		if ((m_info.isFile = S_ISREG(st.st_mode)) || (m_info.isDir = S_ISDIR(st.st_mode)))
		{
			m_info.size = st.st_size;
			m_info.permissions = translatePermissions(st);
			m_info.lastModified = QDateTime::fromTime_t(st.st_mtime);
		}
		else
			if (m_info.isLink = S_ISLNK(st.st_mode))
			{
				char buff[PATH_MAX] = {};

				if (::readlink(m_filePath, buff, PATH_MAX) == 0)
					if (char *realName = ::canonicalize_file_name(buff))
					{
						if (m_info.exists = (stat(realName, &st) == 0))
							if ((m_info.isFile = S_ISREG(st.st_mode)) || (m_info.isDir = S_ISDIR(st.st_mode)))
							{
								m_info.size = st.st_size;
								m_info.permissions = translatePermissions(st);
								m_info.lastModified = QDateTime::fromTime_t(st.st_mtime);
							}

						free(realName);
					}
			}
}

void Info::identify()
{
	if (m_info.isDir)
	{
		FileTypeInfo type;

		type.icon = iconCache.findIcon("inode/directory", ::Desktop::Theme::Small, ::Desktop::Theme::current()->name());
		type.id.mime = QString::fromLatin1("inode/directory");
		type.name = QString::fromLatin1("<DIR>");

		 m_info.type = type;
	}
	else
	{
		FileTypeInfo type;
		const char *mimeType = xdg_mime_get_mime_type_from_file_name(m_filePath);

		if (mimeType == XDG_MIME_TYPE_UNKNOWN)
			mimeType = xdg_mime_get_mime_type_for_file(m_filePath, NULL);

		if (strcmp(mimeType, XDG_MIME_TYPE_TEXTPLAIN) == 0 ||
			strcmp(mimeType, XDG_MIME_TYPE_UNKNOWN) == 0 ||
			strcmp(mimeType, XDG_MIME_TYPE_EMPTY) == 0)
		{
			mimeType = XDG_MIME_TYPE_TEXTPLAIN;
		}

		type.icon = iconCache.findIcon(mimeType, ::Desktop::Theme::Small, ::Desktop::Theme::current()->name());
		type.name = type.id.mime = QString::fromUtf8(mimeType);

		m_info.type = type;
	}
}

DEFAULT_PLUGIN_NS_END
