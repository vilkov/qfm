#include "../filesystemrootfilecontainer.h"
#include "../filesystemfilecontainer.h"
#include "../filesystemfileaccessor.h"
#include "../../../tools/filesystemcommontools.h"

#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


FILE_SYSTEM_NS_BEGIN

RootFileContainer::RootFileContainer() :
	m_path(QString::fromLatin1("/"))
{}

bool RootFileContainer::isPhysical() const
{
	return true;
}

QString RootFileContainer::location() const
{
	return m_path;
}

QString RootFileContainer::location(const QString &fileName) const
{
	return QString(m_path).append(fileName);
}

IFileInfo::size_type RootFileContainer::freeSpace() const
{
	return Tools::freeSpace(m_path.toUtf8());
}

bool RootFileContainer::contains(const QString &fileName) const
{
	struct stat st;
	return ::stat(QString(m_path).append(fileName).toUtf8(), &st) == 0;
}

bool RootFileContainer::remove(const QString &fileName, QString &error) const
{
	struct stat st;
	QByteArray name = QString(m_path).append(fileName).toUtf8();

	if (::stat(name, &st) == 0)
		if (S_ISDIR(st.st_mode))
		{
			if (::rmdir(name) == 0)
				return true;
		}
		else
			if (::unlink(name) == 0)
				return true;

	error = QString::fromUtf8(::strerror(errno));
	return false;
}

bool RootFileContainer::rename(const QString &oldName, const QString &newName, QString &error) const
{
	if (::rename(QString(m_path).append(oldName).toUtf8(), QString(m_path).append(newName).toUtf8()) == 0)
		return true;
	else
	{
		error = QString::fromUtf8(::strerror(errno));
		return false;
	}
}

IFileContainer *RootFileContainer::open() const
{
	return new RootFileContainer(*this);
}

IFileAccessor *RootFileContainer::open(const QString &fileName, int mode, QString &error) const
{
	FileAccesor::Holder file(new FileAccesor(QString(m_path).append(fileName), mode));

	if (file)
		if (static_cast<FileAccesor *>(file.data())->isValid())
			return file.take();
		else
			error = file->lastError();
	else
		error = QString::fromUtf8(::strerror(errno));

	return NULL;
}

IFileContainer *RootFileContainer::open(const QString &fileName, bool create, QString &error) const
{
	struct stat st;
	QByteArray name = QString(m_path).append(fileName).toUtf8();

	if (::stat(name, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			return new FileContainer(QString::fromUtf8(name));
		else
			errno = ENOTDIR;
	}
	else
		if (errno == ENOENT &&
			create &&
			::mkdir(name, S_IRWXU | (S_IRGRP | S_IXGRP) | (S_IROTH | S_IXOTH)) == 0)
		{
			return new FileContainer(QString::fromUtf8(name));
		}

	error = QString::fromUtf8(::strerror(errno));
	return NULL;
}

FILE_SYSTEM_NS_END
