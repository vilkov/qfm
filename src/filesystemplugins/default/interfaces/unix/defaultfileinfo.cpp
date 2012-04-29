#include "../defaultfileinfo.h"
#include "../defaultfileaccessor.h"
#include "../../../../application.h"

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


static QString normalizeFilePath(const QString &filePath, bool &isRoot)
{
	QString res = filePath;

	if (res.startsWith(QChar('/')))
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
			res.prepend(QString::fromUtf8(name));
	}

	if (res.endsWith(QChar('/')))
		res.chop(1);

	return res;
}


Info::Info() :
	m_isRoot(false)
{}

template <>
Info::Info<0>(const QString &filePath, None) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(filePath.mid(filePath.lastIndexOf(QChar('/')) + 1))
{}

template <>
Info::Info<1>(const QString &filePath, Refresh) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(filePath.mid(filePath.lastIndexOf(QChar('/')) + 1))
{
	refresh();
}

template <>
Info::Info<2>(const QString &filePath, Identify) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(filePath.mid(filePath.lastIndexOf(QChar('/')) + 1))
{
	refresh();
	m_info.type = Application::desktopService()->fileTypeInfo(m_filePath, m_info.isDir, 16);
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
	return m_fileName;
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
	return m_info.type.descritpion;
}

void Info::refresh()
{
	struct stat st;
	QByteArray name = m_filePath.toUtf8();

	if (m_info.exists = (::lstat(name, &st) == 0))
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

				if (::readlink(name, buff, PATH_MAX) == 0)
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
