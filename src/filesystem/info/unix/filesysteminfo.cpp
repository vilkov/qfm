#include "../filesysteminfo.h"
#include "../filesystemfileaccessor.h"
#include "../../../application.h"

#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>


FILE_SYSTEM_NS_BEGIN

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

Info::Info(const QString &filePath) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(filePath.mid(filePath.lastIndexOf(QChar('/')) + 1))
{
	refresh();
}

Info::Info(const QString &filePath, bool identifyType) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(filePath.mid(filePath.lastIndexOf(QChar('/')) + 1))
{
	refresh();

	if (identifyType)
		m_info.type = Application::desktopService()->fileTypeInfo(m_filePath, m_info.isDir, 16);
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

bool Info::exists() const
{
	struct stat st;
	return stat(m_filePath.toUtf8(), &st) == 0;
}

Info::size_type Info::fileSize() const
{
	return m_info.size;
}

QString Info::fileName() const
{
	return m_fileName;
}

QString Info::absolutePath() const
{
	return m_filePath.mid(0, m_filePath.lastIndexOf(QChar('/')));
}

QString Info::absoluteFilePath() const
{
	return m_filePath;
}

QString Info::absoluteFilePath(const QString &fileName) const
{
#ifdef Q_OS_WIN
	QString str = m_info.isDir() ? absoluteFilePath() : absolutePath();

	if (str.isEmpty())
		return fileName;
	else
		if (str.endsWith(QChar('/')))
			return str.append(fileName);
		else
			return str.append(QChar('/')).append(fileName);
#else

	if (m_isRoot)
		return QString(m_filePath).append(fileName);
	else
	{
		QString str = m_info.isDir ? absoluteFilePath() : absolutePath();
		return str.append(QChar('/')).append(fileName);
	}
#endif
}

QDateTime Info::lastModified() const
{
	return m_info.lastModified;
}

int Info::permissions() const
{
	return m_info.permissions;
}

void Info::refresh()
{
	int res;
	struct stat st;
	QByteArray name = m_filePath.toUtf8();

	if ((res = ::lstat(name, &st)) == 0)
		if ((m_info.isFile = S_ISREG(st.st_mode)) || (m_info.isDir = S_ISDIR(st.st_mode)))
		{
			m_info.permissions = translatePermissions(st);
			m_info.size = st.st_size;
			m_info.lastModified = QDateTime::fromTime_t(st.st_mtime);
		}
		else
			if (m_info.isLink = S_ISLNK(st.st_mode))
			{
				char buff[PATH_MAX] = {};

				if ((res = ::readlink(name, buff, PATH_MAX)) == 0)
					if (char *realName = ::canonicalize_file_name(buff))
					{
						if ((res = stat(realName, &st)) == 0)
							if ((m_info.isFile = S_ISREG(st.st_mode)) || (m_info.isDir = S_ISDIR(st.st_mode)))
							{
								m_info.permissions = translatePermissions(st);
								m_info.size = st.st_size;
								m_info.lastModified = QDateTime::fromTime_t(st.st_mtime);
							}

						free(realName);
					}
			}
}

FILE_SYSTEM_NS_END
