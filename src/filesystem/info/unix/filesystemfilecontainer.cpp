#include "../filesystemfilecontainer.h"
#include "../filesystemfileaccessor.h"
#include "../../tools/filesystemcommontools.h"
#include "../../../application.h"

#ifdef Q_OS_WIN32
#	include <QtCore/qt_windows.h>
#endif

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

QString FileContainer::location(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
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
#ifdef Q_OS_WIN32
	DWORD attr = GetFileAttributesW((const wchar_t*)filePath.utf16());
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		SetFileAttributesW((const wchar_t*)filePath.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#endif

	if (::unlink(m_info.absoluteFilePath(fileName).toUtf8()) == 0)
		return true;
	else
	{
		error = QString::fromUtf8(::strerror(errno));
		return false;
	}
}

bool FileContainer::rename(const QString &oldName, const QString &newName, QString &error)
{
	if (::rename(m_info.absoluteFilePath(oldName).toUtf8(), m_info.absoluteFilePath(newName).toUtf8()) == 0)
		return true;
	else
	{
		error = QString::fromUtf8(::strerror(errno));
		return false;
	}
}

IFileAccessor *FileContainer::open(const QString &fileName, int mode, QString &error) const
{
	FileAccesor::Holder file(new FileAccesor(m_info.absoluteFilePath(fileName), mode));

	if (file)
		if (static_cast<FileAccesor *>(file.data())->isValid())
			return file.take();
		else
			error = file->lastError();
	else
		error = QString::fromUtf8(::strerror(errno));

	return NULL;
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
