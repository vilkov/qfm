#include "../defaultfileinfo.h"
#include "../defaultfileaccessor.h"
#include "../../../../application.h"

#include <QtCore/QTextCodec>

#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>


DEFAULT_PLUGIN_NS_BEGIN

static uint userId = getuid();
static uint groupId = getgid();


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
	m_fileNameString(other.m_fileNameString),
	m_info(other.m_info)
{}

template <>
Info::Info<Info::None>(const QByteArray &filePath, None) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(filePath.mid(filePath.lastIndexOf('/') + 1)),
    m_fileNameString(codec()->toUnicode(m_fileName))
{}

template <>
Info::Info<Info::Refresh>(const QByteArray &filePath, Refresh) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(m_filePath.mid(m_filePath.lastIndexOf('/') + 1)),
    m_fileNameString(codec()->toUnicode(m_fileName))
{
	refresh();
}

template <>
Info::Info<Info::Identify>(const QByteArray &filePath, Identify) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(m_filePath.mid(m_filePath.lastIndexOf('/') + 1)),
    m_fileNameString(codec()->toUnicode(m_fileName))
{
	refresh();
	m_info.type = Application::desktopService()->fileTypeInfo(m_filePath, m_info.isDir);
}

template <>
Info::Info<Info::None>(const Info &other, None) :
	m_isRoot(other.m_isRoot),
	m_filePath(other.m_filePath),
	m_fileName(other.m_fileName),
    m_fileNameString(other.m_fileNameString),
	m_info(other.m_info)
{}

template <>
Info::Info<Info::Refresh>(const Info &other, Refresh) :
	m_isRoot(other.m_isRoot),
	m_filePath(other.m_filePath),
	m_fileName(other.m_fileName),
    m_fileNameString(other.m_fileNameString),
	m_info(other.m_info)
{
	refresh();
}

template <>
Info::Info<Info::Identify>(const Info &other, Identify) :
	m_isRoot(other.m_isRoot),
	m_filePath(other.m_filePath),
	m_fileName(other.m_fileName),
    m_fileNameString(other.m_fileNameString),
	m_info(other.m_info)
{
	m_info.type = Application::desktopService()->fileTypeInfo(m_filePath, m_info.isDir);
}

QString Info::operator-(const Info &o) const
{
	Path current(m_filePath);
	Path other(o.m_filePath);

	for (Path::Iterator otherIt = other.begin(), currentIt = current.begin();
		!otherIt.atEnd() && !currentIt.atEnd();
		++otherIt)
	{
		if (*currentIt == *otherIt)
			currentIt = current.erase(currentIt);
		else
			break;
	}

	return current.toString();
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

QString Info::fileName() const
{
	return m_fileNameString;
}

const IFileType *Info::fileType() const
{
	return this;
}

QDateTime Info::lastModified() const
{
	return m_info.lastModified;
}

int Info::permissions() const
{
	return m_info.permissions;
}

FileTypeId Info::id() const
{
	return m_info.type.id;
}

QIcon Info::icon() const
{
	return m_info.type.icon;
}

QString Info::name() const
{
	return m_info.type.name;
}

QString Info::description() const
{
	return m_info.type.description;
}

QTextCodec *Info::codec()
{
	return QTextCodec::codecForLocale();
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

DEFAULT_PLUGIN_NS_END
