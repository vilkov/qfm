#include "../filesystemfilecontainer.h"
#include "../../tools/filesystemcommontools.h"
#include "../../../application.h"

#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

FileContainer::FileContainer(const Info &info) :
	m_info(info)
{}

bool FileContainer::isPhysical() const
{
	return true;
}

QString FileContainer::location() const
{
	return m_info.absoluteFilePath();
}

IFileInfo::size_type FileContainer::freeSpace() const
{
	return Tools::freeSpace(m_info.isDir() ? m_info.absoluteFilePath().toUtf8() : m_info.absolutePath().toUtf8());
}

bool FileContainer::contains(const QString &fileName) const
{
	struct stat st;
	return ::stat(m_info.absoluteFilePath(fileName).toUtf8(), &st) == 0;
}

bool FileContainer::remove(const QString &fileName, QString &error) const
{
	if (::unlink(m_info.absoluteFilePath(fileName).toUtf8()) == 0)
		return true;
	else
	{
		error = QString::fromUtf8(::strerror(errno));
		return false;
	}
}

IFile *FileContainer::open(const QString &fileName) const
{
	return new Info(m_info.absoluteFilePath(fileName));
}

IFileContainer *FileContainer::open(const QString &fileName, bool create, QString &error) const
{
	struct stat st;
	QString absoluteFileName = m_info.absoluteFilePath(fileName);
	QByteArray absoluteFileNameAsUtf8 = absoluteFileName.toUtf8();

	if (::stat(absoluteFileNameAsUtf8, &st) == 0)
		if (DIR *dir = ::opendir(absoluteFileNameAsUtf8))
		{
			::closedir(dir);
			return new FileContainer(absoluteFileName);
		}
	else
		if (errno == ENOENT &&
			create &&
			::mkdir(absoluteFileNameAsUtf8, S_IRWXU | (S_IRGRP | S_IXGRP) | (S_IROTH | S_IXOTH)) == 0)
		{
			return new FileContainer(absoluteFileName);
		}

	error = QString::fromUtf8(::strerror(errno));
	return NULL;
}

bool FileContainer::copy(const IFileInfo *source, QString &error) const
{
	if (::link(source->absoluteFilePath().toUtf8(), m_info.absoluteFilePath(source->fileName()).toUtf8()) == 0)
		return true;
	else
	{
		error = QString::fromUtf8(::strerror(errno));
		return false;
	}
}

FILE_SYSTEM_NS_END
